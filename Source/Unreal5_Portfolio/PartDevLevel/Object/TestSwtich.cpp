// Fill out your copyright notice in the Description page of Project Settings.


#include "PartDevLevel/Object/TestSwtich.h"
#include "Components/CapsuleComponent.h"
#include "TestLevel/Character/TestCharacter.h"
#include "PartDevLevel/Object/TestDoor.h"
#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/ContentsLog.h"
#include "Global/DataTable/MapObjDataRow.h"


// Sets default values
ATestSwtich::ATestSwtich()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwitchCollision"));
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	Mesh->SetupAttachment(Collision);

}

// Called when the game starts or when spawned
void ATestSwtich::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* Inst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());

	if (nullptr == Inst)
	{
		UE_LOG(ObjectLog, Fatal, TEXT("%S(%u)> if (nullptr == Inst)"), __FUNCTION__, __LINE__);
		return;
	}

	const FMapObjDataRow* TableData = Inst->GetMapObjDataTable(FName(TEXT("Armory_Switch")));
	Mesh->SetStaticMesh(TableData->GetMesh());
}

// Called every frame
void ATestSwtich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestSwtich::CharactorCollision(AActor* _OtherActor, UPrimitiveComponent* _Collision)
{
	ATestCharacter* CharactorCheck = Cast<ATestCharacter>(_OtherActor);
	if (nullptr == CharactorCheck)
	{
		return;
	}

	CharctorOn = true;
}

void ATestSwtich::DoorSwitch(AActor* _OtherActor)
{
	bool IsDoorOpen = ConnectedDoor->GetIsDoorOpen();
	
	ATestDoor* TestDoor = Cast<ATestDoor>(_OtherActor);
	// ����
	if (true == CharctorOn && false == IsDoorOpen)
	{
		// ���� ������ �ͼ� cast���ְ�, Doortime�� 0.0���� set���ش�.
		ConnectedDoor->SetIsDoorOpen(true);
		int a = 0;
	}
	// �ݱ�
	else if (true == CharctorOn && true == IsDoorOpen)
	{
		// ���� ������ �ͼ� cast���ְ�, Doortime�� 0.0���� set���ش�.
		ConnectedDoor->SetIsDoorOpen(false);
		int a = 0;
	}

}

