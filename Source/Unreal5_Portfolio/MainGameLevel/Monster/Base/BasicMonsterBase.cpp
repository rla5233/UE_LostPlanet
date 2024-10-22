// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"
#include "MainGameLevel/Monster/Animation/MonsterRandomAnimInstance.h"
#include "MainGameLevel/Monster/BasicMonster/AI/BasicMonsterAIController.h"

#include "GameFrameWork/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "BrainComponent.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/MainGameInstance.h"
#include "Global/MainGameState.h"
#include "Global/ContentsLog.h"

#include "PartDevLevel/Character/ParentsCharacter.h"
#include "TestLevel/Character/TestPlayerState.h"

ABasicMonsterBase::ABasicMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Attack Component
	AttackComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Component"));
	AttackComponent->SetupAttachment(RootComponent);

	// Dissolve
	DeadTimelineFinish.BindUFunction(this, "DeadFinish");
	DeadDissolveCallBack.BindUFunction(this, "DeadDissolveInterp");
}

void ABasicMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicMonsterBase, AnimIndex);
	DOREPLIFETIME(ABasicMonsterBase, AnimType);
}

void ABasicMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (UMainGameInstance* MainGameInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld()))
	{
		if (const FMonsterDataRow* BaseData = MainGameInst->GetMonsterData(BaseDataRowName))
		{
			// 데이터 세팅
			InitData(BaseData);
			if (SettingData)
			{
				SettingData->OriginPos = GetActorLocation();
				SettingData->Hp = BaseData->MaxHp;
				SettingData->BaseData = BaseData;
			}

			// 애니메이션 세팅
			AnimInst = Cast<UMonsterRandomAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInst)
			{
				for (TPair<EBasicMonsterAnim, FAnimMontageGroup> AnimMontageGroup : BaseData->AllAnimMontages)
				{
					AnimInst->PushAnimation(AnimMontageGroup.Key, AnimMontageGroup.Value);
				}
			}
		}
	}

	// Binding
	AttackComponent->OnComponentEndOverlap.AddDynamic(this, &ABasicMonsterBase::OnAttackOverlapEnd);
	AttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 서버 체크
	if (true == HasAuthority())
	{
		// AI 컨트롤러 세팅
		AIController = GetController<ABasicMonsterAIController>();
	}
}

void ABasicMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInst->ChangeAnimation(AnimType, AnimIndex);
	DeadTimeLine.TickTimeline(DeltaTime);
}

void ABasicMonsterBase::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AParentsCharacter* HitCharacter = Cast<AParentsCharacter>(OtherActor);
	if (nullptr != HitCharacter)
	{
		ATestPlayerState* HitPlayerState = Cast<ATestPlayerState>(HitCharacter->GetPlayerState());
		if (nullptr == HitPlayerState)
		{
			return;
		}
		
		if (true == HasAuthority())
		{
			HitPlayerState->AddDamage(SettingData->BaseData->AttackDamage);
		}
	}
}

void ABasicMonsterBase::ChangeRandomAnimation(uint8 Type)
{
	AnimType = Type;
	AnimInst->SetRandomAnimIndex(Type, AnimIndex);
}

void ABasicMonsterBase::Damaged(float Damage)
{
	Super::Damaged(Damage);

	// Server Only
	if (false == HasAuthority() || 0.0f >= SettingData->Hp)
	{
		return;
	}
	
	SettingData->Hp -= Damage;

	// Dead
	if (0.0f >= SettingData->Hp)
	{
		SetDead();
		ChangeRandomAnimation(EBasicMonsterAnim::Dead);
		AIController->GetBrainComponent()->StopLogic(TEXT("Dead"));
	}
}

void ABasicMonsterBase::SetChasePlayer()
{
	if (true == HasAuthority())
	{
		if (AMainGameState* MainGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld()))
		{
			if (UActorGroup* PlayerGroup = MainGameState->GetActorGroup(EObjectType::Player))
			{
				if (UMainGameInstance* MainGameInstance = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld()))
				{
					// Find Player
					int32 PlayerIndex = MainGameInstance->Random.RandRange(0, PlayerGroup->Actors.Num() - 1);

					AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PlayerGroup->Actors[PlayerIndex]);
					AIController->GetBlackboardComponent()->SetValueAsEnum(TEXT("State"), static_cast<uint8>(EBasicMonsterState::Chase));
				}
			}
		}
	}
}

void ABasicMonsterBase::SetDead_Implementation()
{
	// Effect Setting
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, GetActorTransform());

	// Dissolve Setting
	TArray<UMaterialInterface*> MaterialsInterface = GetMesh()->GetMaterials();

	DynamicMaterials.Empty();
	for (int32 i = 0; i < MaterialsInterface.Num(); i++)
	{
		UMaterialInstanceDynamic* MatInstDynamic = GetMesh()->CreateDynamicMaterialInstance(i, MaterialsInterface[i]);
		DynamicMaterials.Add(MatInstDynamic);
	}

	DeadTimeLine.AddInterpFloat(DeadDissolveCurve, DeadDissolveCallBack);
	DeadTimeLine.SetTimelineFinishedFunc(DeadTimelineFinish);
	DeadTimeLine.SetTimelineLength(3.0f);
	DeadTimeLine.SetLooping(false);
	DeadTimeLine.PlayFromStart();

	// Collision Setting
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	// MoveMoment Setting
	GetCharacterMovement()->SetActive(false);
}

void ABasicMonsterBase::DeadDissolveInterp(float _Value)
{
	for (UMaterialInstanceDynamic* DynamicMat : DynamicMaterials)
	{
		DynamicMat->SetScalarParameterValue("Dissolve", _Value);
	}
}