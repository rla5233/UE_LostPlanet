// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/BasicMonster/BasicMonsterSpawner.h"
#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"
#include "MainGameLevel/Player/MainCharacter.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/MainGameInstance.h"
#include "Global/MainGameState.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

#include "TestLevel/Character/TestCharacter.h"

ABasicMonsterSpawner::ABasicMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	UBillboardComponent* Root = CreateDefaultSubobject<UBillboardComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	// Create TriggerBox
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnTriggerBox"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABasicMonsterSpawner::TriggerBoxBeginOverlap);
	TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	TriggerBox->SetCanEverAffectNavigation(false);
	TriggerBox->SetupAttachment(RootComponent);

}

void ABasicMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	TimeCount = InitSpawnDelayTime;
	TriggerBox->SetActive(TriggerIsActive);
}

void ABasicMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ʈ���� �ڽ� Ȱ��ȭ �Ǵ� Ŭ���̾�Ʈ �Ͻ� return
	if (true == TriggerIsActive || false == HasAuthority())
	{
		return;
	}

	if (0.0f < TimeCount)
	{
		TimeCount -= DeltaTime;
		return;
	}

	if (0 >= TotalSpawnCount || EGameStage::MissionClear == UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld())->GetCurStage())
	{
		Destroy();
		return;
	}

	SpawnBasicMonster();
	TimeCount = SpawnDelayTime;
}

void ABasicMonsterSpawner::SpawnBasicMonster()
{
	UMainGameInstance* MainInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
	FVector CurPos = GetActorLocation();

	// ���� ����
	for (FFixedSpawn& CurSpawnInfo : FixedSpawn)
	{
		for (int i = 0; i < CurSpawnInfo.SpawnFixedMonsterCount; ++i)
		{
			float SpawnRadius = MainInst->Random.FRandRange(0, MaxSpawnRadius);
			FVector SpawnLocation = CurPos + MainInst->Random.GetUnitVector().GetSafeNormal2D() * SpawnRadius;
			ABasicMonsterBase* NewMonster = GetWorld()->SpawnActor<ABasicMonsterBase>(CurSpawnInfo.MonsterUClass, SpawnLocation, FRotator::ZeroRotator);
		
			if (nullptr != NewMonster && true == IsChasePlayer)
			{
				NewMonster->SetChasePlayer();
			}
		}
	}

	// ���� ����
	int Size = RandomMonsterUClass.Num() - 1;
	for (int32 i = 0; i < SpawnRandomMonsterCount; ++i)
	{
		int TypeIndex = MainInst->Random.RandRange(0, Size);
		float SpawnRadius = MainInst->Random.FRandRange(0, MaxSpawnRadius);
		FVector SpawnLocation = CurPos + MainInst->Random.GetUnitVector().GetSafeNormal2D() * SpawnRadius;
		ABasicMonsterBase* NewMonster = GetWorld()->SpawnActor<ABasicMonsterBase>(RandomMonsterUClass[TypeIndex], SpawnLocation, FRotator::ZeroRotator);

		if (nullptr != NewMonster && true == IsChasePlayer)
		{
			NewMonster->SetChasePlayer();
		}
	}

	--TotalSpawnCount;
}

void ABasicMonsterSpawner::TriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATestCharacter* Player = Cast<ATestCharacter>(OtherActor);
	if (nullptr == Player)
	{
		return;
	}

	AMainGameState* MainGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld());
	if (nullptr == MainGameState)
	{
		return;
	}

	EGameStage CurStage = MainGameState->GetCurStage();
	if (TriggerCheckStage == CurStage)
	{
		TriggerIsActive = false;
		TriggerBox->SetActive(TriggerIsActive);
	}
}