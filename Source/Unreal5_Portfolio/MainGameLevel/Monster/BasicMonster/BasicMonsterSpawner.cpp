// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/BasicMonster/BasicMonsterSpawner.h"
#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"
#include "PartDevLevel/Character/ParentsCharacter.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/MainGameInstance.h"
#include "Global/MainGameState.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"


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

	// 서버이고 트리거 비활성화시
	if (true == HasAuthority() && false == TriggerIsActive)
	{
		if (0.0f < TimeCount)
		{
			TimeCount -= DeltaTime;
			return;
		}

		EGameStage CurGameStage = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld())->GetCurStage();
		if (0 >= TotalSpawnCount || EGameStage::MissionClear == CurGameStage)
		{
			Destroy();
			return;
		}

		SpawnBasicMonster();
		TimeCount = SpawnDelayTime;
	}
}

void ABasicMonsterSpawner::SpawnBasicMonster()
{
	UMainGameInstance* MainInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
	FVector CurPos = GetActorLocation();

	// 고정 스폰
	for (FFixedSpawn& CurSpawnInfo : FixedSpawnInfo)
	{
		for (int i = 0; i < CurSpawnInfo.SpawnFixedMonsterCount; ++i)
		{
			float RandomX = MainInst->Random.FRandRange(-1.0, 1.0);
			float RandomY = MainInst->Random.FRandRange(-1.0, 1.0);
			FVector RandomUnitVector = FVector(RandomX, RandomY, 0.0).GetSafeNormal2D();

			float SpawnRadius = MainInst->Random.FRandRange(0, MaxSpawnRadius);
			FVector SpawnLocation = CurPos + RandomUnitVector * SpawnRadius;

			ABasicMonsterBase* NewMonster = GetWorld()->SpawnActor<ABasicMonsterBase>(CurSpawnInfo.MonsterUClass, SpawnLocation, FRotator::ZeroRotator);
		
			if (nullptr != NewMonster && true == IsChasePlayer)
			{
				NewMonster->SetChasePlayer();
			}
		}
	}

	// 랜덤 스폰
	int Size = RandomMonsterUClass.Num() - 1;
	for (int32 i = 0; i < SpawnRandomMonsterCount; ++i)
	{
		float RandomX = MainInst->Random.FRandRange(-1.0, 1.0);
		float RandomY = MainInst->Random.FRandRange(-1.0, 1.0);
		FVector RandomUnitVector = FVector(RandomX, RandomY, 0.0).GetSafeNormal2D();

		float SpawnRadius = MainInst->Random.FRandRange(0, MaxSpawnRadius);
		FVector SpawnLocation = CurPos + RandomUnitVector * SpawnRadius;

		int MonsterIndex = MainInst->Random.RandRange(0, Size);
		ABasicMonsterBase* NewMonster = GetWorld()->SpawnActor<ABasicMonsterBase>(RandomMonsterUClass[MonsterIndex], SpawnLocation, FRotator::ZeroRotator);

		if (nullptr != NewMonster && true == IsChasePlayer)
		{
			NewMonster->SetChasePlayer();
		}
	}

	--TotalSpawnCount;
}

void ABasicMonsterSpawner::TriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AParentsCharacter* Player = Cast<AParentsCharacter>(OtherActor))
	{
		if (AMainGameState* MainGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld()))
		{
			EGameStage CurStage = MainGameState->GetCurStage();
			if (TriggerCheckStage == CurStage)
			{
				TriggerIsActive = false;
				TriggerBox->SetActive(TriggerIsActive);
			}
		}
	}
}