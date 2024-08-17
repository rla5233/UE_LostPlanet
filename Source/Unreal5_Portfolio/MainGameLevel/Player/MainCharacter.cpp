// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Player/MainCharacter.h"
#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Global/MainGameInstance.h"
#include "Global/DataTable/ItemDataRow.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "MainPlayerController.h"
#include "PlayerItemInformation.h"
#include "PartDevLevel/Character/PlayerAnimInstance.h"
#include "Components/SphereComponent.h"
#include "MainGameLevel/Player/MainPlayerState.h"

#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"

#include "MainGameLevel/Object/MapObjectBase.h"
#include "MainGameLevel/Object/DoorObject.h"
#include "MainGameLevel/Object/Bomb.h"
#include "MainGameLevel/Object/AreaObject.h"

#include "MainGameLevel/UI/InGame/HeadNameWidgetComponent.h"
#include "MainGameLevel/UI/InGame/MainGameHUD.h"
#include "MainGameLevel/UI/Title/MainTitleHUD.h"
#include "PartDevLevel/UI/GetItem/GetItem_UserWidget.h"
#include "TestLevel/UI/TestMinimapIconComponent.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// == Init ==

	// character Mesh
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->bHiddenInSceneCapture = true;

	// ������ ���� ���� �ʱ�ȭ.
	ItemSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemSocketMesh"));
	ItemSocketMesh->SetupAttachment(GetMesh(), FName("ItemSocket"));
	ItemSocketMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ItemSocketMesh->SetGenerateOverlapEvents(true);
	ItemSocketMesh->SetOwnerNoSee(true);
	ItemSocketMesh->SetVisibility(false);
	ItemSocketMesh->SetIsReplicated(true);
	ItemSocketMesh->bHiddenInSceneCapture = true;

	// SpringArm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FPVCameraRelLoc);
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bDoCollisionTest = true;

	// Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Perspective);

	// FPV Character Mesh
	FPVMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FPVMesh->SetupAttachment(CameraComponent);
	FPVMesh->SetRelativeLocation(FVector(-10.0f, 0.0f, -160.0f));
	FPVMesh->SetOwnerNoSee(false);
	FPVMesh->SetOnlyOwnerSee(true);
	FPVMesh->bCastDynamicShadow = false;
	FPVMesh->CastShadow = false;

	// 1��Ī ������ ���� ���� �ʱ�ȭ.
	FPVItemSocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FPVItemSocketMesh"));
	FPVItemSocketMesh->SetupAttachment(FPVMesh, FName("FPVItemSocket"));
	FPVItemSocketMesh->SetCollisionProfileName(TEXT("NoCollision"));
	FPVItemSocketMesh->SetGenerateOverlapEvents(true);
	FPVItemSocketMesh->SetOnlyOwnerSee(true);
	FPVItemSocketMesh->SetVisibility(false);
	FPVItemSocketMesh->SetIsReplicated(true);
	FPVItemSocketMesh->bCastDynamicShadow = false;
	FPVItemSocketMesh->CastShadow = false;

	// Map Item 
	GetMapItemCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GetMapItemCollisionComponent"));
	GetMapItemCollisionComponent->SetupAttachment(RootComponent);
	GetMapItemCollisionComponent->SetRelativeLocation(FVector(60.0, 0.0, -5.0f));
	GetMapItemCollisionComponent->SetBoxExtent(FVector(60.0f, 30.0f, 100.0f));
	GetMapItemCollisionComponent->SetCollisionProfileName(FName("MapItemSearch"));

	// Hand Attack Component
	HandAttackComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Hand Attack Comp"));
	HandAttackComponent->SetupAttachment(GetMesh());
	HandAttackComponent->SetRelativeLocation({ 0.0f, 100.0f, 120.0f });
	HandAttackComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// MinimapIcon Component
	MinimapIconComponent = CreateDefaultSubobject<UTestMinimapIconComponent>(TEXT("MinimapPlayerIcon"));
	MinimapIconComponent->SetupAttachment(RootComponent);
	MinimapIconComponent->bVisibleInSceneCaptureOnly = true;
	
	// HeadName Component
	HeadNameComponent = CreateDefaultSubobject<UHeadNameWidgetComponent>(TEXT("HeadNameWidgetComponent"));
	HeadNameComponent->SetupAttachment(RootComponent);
	HeadNameComponent->SetOwnerNoSee(true);
	HeadNameComponent->bHiddenInSceneCapture = true;

	// Inventory
	for (size_t i = 0; i < 4; i++)
	{
		FPlayerItemInformation NewSlot;
		ItemSlot.Push(NewSlot);
	}
}

void AMainCharacter::PostInitializeComponents() // FName �κ� ���� �ʿ�.
{
	if (GetWorld()->WorldType == EWorldType::Game
		|| GetWorld()->WorldType == EWorldType::PIE)
	{
		UMainGameInstance* MainGameInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
		if (nullptr == MainGameInst)
		{
			return;
		}

		GetSetSelectCharacter(MainGameInst);
		if (UIToSelectCharacter == "")
		{
			UIToSelectCharacter = "TestPlayer"; // test
		}

		// ���̷�Ż �޽� ����
		//USkeletalMesh* PlayerSkeletalMesh = MainGameInst->GetPlayerData(FName("TestPlayer"))->GetPlayerSkeletalMesh();
		USkeletalMesh* PlayerSkeletalMesh = MainGameInst->GetPlayerData(UIToSelectCharacter)->GetPlayerSkeletalMesh();
		GetMesh()->SetSkeletalMesh(PlayerSkeletalMesh);


		//USkeletalMesh* FPVSkeletalMesh = MainGameInst->GetPlayerData(FName("TestPlayer"))->GetPlayerFPVPlayerSkeletalMesh();
		USkeletalMesh* FPVSkeletalMesh = MainGameInst->GetPlayerData(UIToSelectCharacter)->GetPlayerFPVPlayerSkeletalMesh();
		FPVMesh->SetSkeletalMesh(FPVSkeletalMesh);

		// ABP ����
		//UClass* AnimInst = Cast<UClass>(MainGameInst->GetPlayerData(FName("TestPlayer"))->GetPlayerAnimInstance());
		UClass* AnimInst = Cast<UClass>(MainGameInst->GetPlayerData(UIToSelectCharacter)->GetPlayerAnimInstance());
		GetMesh()->SetAnimInstanceClass(AnimInst);
	}

	Super::PostInitializeComponents();

	// ���ε� ����
	if (nullptr != Reload_Widget)
	{
		Reload_Widget->AddToViewport();
		Reload_Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	NetCheck();
	Super::BeginPlay();

	UMainGameBlueprintFunctionLibrary::PushActor(EObjectType::Player, this);

	// PlayerState �ʱ�ȭ
	SettingPlayerState();

	PlayerAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	FPVPlayerAnimInst = Cast<UPlayerAnimInstance>(FPVMesh->GetAnimInstance());

	// GetMapItemCollision Component�� ���� �Լ� Bind
	GetMapItemCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::MapItemOverlapStart);
	GetMapItemCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::MapItemOverlapEnd);

	ChangeMontage(EPlayerUpperState::UArm_Idle);
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ��ü ����
	DOREPLIFETIME(AMainCharacter, LowerStateValue);
	// �÷��̾� �ڼ� ����.
	DOREPLIFETIME(AMainCharacter, DirValue);
	DOREPLIFETIME(AMainCharacter, IdleDefault);

	DOREPLIFETIME(AMainCharacter, Token);
	//DOREPLIFETIME(AMainCharacter, IsFaint);

	DOREPLIFETIME(AMainCharacter, UIToSelectCharacter); // Test
}

void AMainCharacter::AnimationEnd()
{
	PlayerAnimInst->ChangeAnimation(IdleDefault);
	FPVPlayerAnimInst->ChangeAnimation(IdleDefault);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlayerHp(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::ChangeLowerState_Implementation(EPlayerLowerState _LowerState)
{
	LowerStateValue = _LowerState;
}

void AMainCharacter::ChangePlayerDir_Implementation(EPlayerMoveDir _Dir)
{
	if (IdleDefault == EPlayerUpperState::UArm_Idle)
	{
		switch (_Dir)
		{
		case EPlayerMoveDir::Forward:
			ChangeMontage(EPlayerUpperState::MoveForward);
			break;
		case EPlayerMoveDir::Back:
			ChangeMontage(EPlayerUpperState::MoveBack);
			break;
		case EPlayerMoveDir::Left:
			ChangeMontage(EPlayerUpperState::MoveLeft);
			break;
		case EPlayerMoveDir::Right:
			ChangeMontage(EPlayerUpperState::MoveRight);
			break;
		default:
			break;
		}
	}

	DirValue = _Dir;
}

void AMainCharacter::DestroyItem_Implementation(AItemBase* _Item)
{
	// �ʵ忡�� ���� ������ Destroy
	_Item->Destroy();
}

void AMainCharacter::SetItemSocketVisibility_Implementation(bool _Visibility)
{
	ItemSocketMesh->SetVisibility(_Visibility);
	FPVItemSocketMesh->SetVisibility(_Visibility);
}

void AMainCharacter::SetItemSocketMesh_Implementation(UStaticMesh* _ItemMeshRes, FVector _ItemRelLoc, FRotator _ItemRelRot, FVector _ItemRelScale)
{
	// static mesh ����
	ItemSocketMesh->SetStaticMesh(_ItemMeshRes);
	FPVItemSocketMesh->SetStaticMesh(_ItemMeshRes);

	// transform ����
	ItemSocketMesh->SetRelativeLocation(_ItemRelLoc);
	FPVItemSocketMesh->SetRelativeLocation(_ItemRelLoc);

	ItemSocketMesh->SetRelativeRotation(_ItemRelRot);
	FPVItemSocketMesh->SetRelativeRotation(_ItemRelRot);

	ItemSocketMesh->SetRelativeScale3D(_ItemRelScale);
	FPVItemSocketMesh->SetRelativeScale3D(_ItemRelScale);
}

void AMainCharacter::SettingItemSocket(int _InputKey)
{
	if (-1 == _InputKey)
	{
		// ItemSocket�� Visibility ����
		SetItemSocketVisibility(false);
		return;
	}

	UStaticMesh* ItemMeshRes = ItemSlot[_InputKey].MeshRes;
	FVector ItemRelLoc = ItemSlot[_InputKey].RelLoc;
	FRotator ItemRelRot = ItemSlot[_InputKey].RelRot;
	FVector ItemRelScale = ItemSlot[_InputKey].RelScale;

	// ItemSocket�� static mesh ����
	SetItemSocketMesh(ItemMeshRes, ItemRelLoc, ItemRelRot, ItemRelScale);

	// ItemSocket�� visibility �ѱ�
	SetItemSocketVisibility(true);
}

void AMainCharacter::SpawnItem_Implementation(FName _ItemName, FTransform _SpawnTrans)
{
	// ������ ������ ���� ��������
	UMainGameInstance* MainGameInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
	const FItemDataRow* ItemData = MainGameInst->GetItemData(_ItemName);

	// �ʵ忡 ������ Spawn
	AItemBase* DropItem = GetWorld()->SpawnActor<AItemBase>(ItemData->GetItemUClass(), _SpawnTrans);
	UStaticMeshComponent* DropItemMeshComp = Cast<AItemBase>(DropItem)->GetStaticMeshComponent();
	DropItemMeshComp->SetSimulatePhysics(true);
}

void AMainCharacter::PickUpItem(AItemBase* _Item)
{
	const FItemDataRow* ItemData = _Item->GetItemData();
	EItemType ItemType = ItemData->GetItemType();
	int ItemSlotIndex = static_cast<int>(ItemType);
	FName ItemName = *(ItemData->GetName());

	// �̹� ���� �̸��� ���� �������� �κ��丮�� ���� ��� return
	if (ItemName == ItemSlot[ItemSlotIndex].Name)
	{
		return;
	}

	// �̹� ���� Ÿ���� �������� �κ��丮�� ���� ��� ������ �ִ� �������� Drop
	if (true == ItemSlot[ItemSlotIndex].IsItemIn)
	{
		//DropItem(ItemSlotIndex);
	}

	// �κ��丮�� PickUp�� ������ ���� �ֱ�
	ItemSlot[ItemSlotIndex].IsItemIn = true;
	ItemSlot[ItemSlotIndex].Name = ItemName;
	ItemSlot[ItemSlotIndex].ReloadMaxNum = ItemData->GetReloadNum();		// ���� ���� ����	 (Max) (-1�� ��� �ѱ�� ���Ⱑ �ƴ�)
	ItemSlot[ItemSlotIndex].ReloadLeftNum = ItemData->GetReloadNum();		// ���� ���� ����	 (Left) (-1�� ��� �ѱ�� ���Ⱑ �ƴ�)
	ItemSlot[ItemSlotIndex].Damage = ItemData->GetDamage();					// ���� ���ݷ� (0�� ��� ���Ⱑ �ƴ�)
	ItemSlot[ItemSlotIndex].MeshRes = ItemData->GetResMesh();				// ����ƽ �޽� ���ҽ�
	if ("TestPlayer" == UIToSelectCharacter || "Vanguard" == UIToSelectCharacter)
	{
		ItemSlot[ItemSlotIndex].RelLoc = ItemData->GetRelLoc_E();			// ItemSocket, FPVItemSocket ����� ��ġ
		ItemSlot[ItemSlotIndex].RelRot = ItemData->GetRelRot_E();			// ItemSocket, FPVItemSocket ����� ȸ��
	}
	else if ("AlienSoldier" == UIToSelectCharacter || "Crypto" == UIToSelectCharacter)
	{
		ItemSlot[ItemSlotIndex].RelLoc = ItemData->GetRelLoc_A();			// ItemSocket, FPVItemSocket ����� ��ġ
		ItemSlot[ItemSlotIndex].RelRot = ItemData->GetRelRot_A();			// ItemSocket, FPVItemSocket ����� ȸ��
	}
	ItemSlot[ItemSlotIndex].RelScale = ItemData->GetRelScale();				// ItemSocket, FPVItemSocket ����� ũ��
	
	// �ʵ忡 �����ϴ� ������ ���� ����
	DestroyItem(_Item);

	// Change �ִϸ��̼�
	{
		if (ItemType == EItemType::Rifle)
		{
			IdleDefault = EPlayerUpperState::Rifle_Idle;
		}
		else if (ItemType == EItemType::Melee)
		{
			IdleDefault = EPlayerUpperState::Melee_Idle;
		}
		SettingItemSocket(static_cast<int>(ItemType));
		ChangeMontage(IdleDefault);
	}

	// To Controller -> To Widget
	//AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	//if (nullptr != Con)
	//{
	//	Con->FGetItemToWidget.Execute();
	//}
}

void AMainCharacter::DropItem(int _SlotIndex)
{
	CurItemIndex = 0;

	// DropItem �� �� ���� ��� 1: �Ǽ��� ��
	if (CurItemIndex == -1)
	{
#ifdef WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString(TEXT("There's no item to drop. (Current posture is 'Barehand')")));
#endif // WITH_EDITOR
		return;
	}

	// DropItem �� �� ���� ��� 2: (�׷����� ��������) ���� Posture�� �ش��ϴ� ���Ⱑ �κ��丮�� ���� ��
	if (false == ItemSlot[CurItemIndex].IsItemIn)
	{
#ifdef WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString(TEXT("There's no item to drop. (The item slot is empty)")));
#endif // WITH_EDITOR
		return;
	}

	// ����Ʈ�� �������� Actor�� ����
	FName ItemName = ItemSlot[_SlotIndex].Name;
	FTransform SpawnTrans = GetActorTransform();
	SpawnTrans.SetTranslation(GetActorLocation() + (GetActorForwardVector() * 100.0f) + (GetActorUpVector() * 50.0f));

	SpawnItem(ItemName, SpawnTrans);


	// �ڼ��� �Ǽ����� ����
	ChangeMontage(IdleDefault);
}

void AMainCharacter::FireRayCast_Implementation()
{
	if (CurItemIndex == -1 || CurItemIndex == 2)
	{
		return;
	}

	// ź���� ���ٸ� 
	if (ItemSlot[CurItemIndex].ReloadLeftNum <= 0)
	{
		//ItemSlot[CurItemIndex].ReloadLeftNum = ItemSlot[CurItemIndex].ReloadMaxNum;
		// �����϶�� Widget�� ����� ��.
		// ���� �Լ��� CharacterReload �̴�.
		Reload_Widget->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	FVector Start = GetMesh()->GetSocketLocation(FName("MuzzleSocket"));
	//Start.Z -= 20.0f;
	FVector End = (Con->GetControlRotation().Vector() * 2000.0f) + Start;
	
	FHitResult Hit;
	if (GetWorld())
	{
		// ź�� ���.
		ItemSlot[CurItemIndex].ReloadLeftNum -= 1;

		// Ray Cast
		TArray<AActor*> IgnoreActors; // ������ Actor��.
		bool ActorHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
		
		if (true == ActorHit && nullptr != Hit.GetActor())
		{
			FString BoneName = Hit.BoneName.ToString();
			//UE_LOG(LogTemp, Warning, TEXT("Bone Name : %s"), *BoneName);
			ABasicMonsterBase* Monster = Cast<ABasicMonsterBase>(Hit.GetActor());
			if (nullptr != Monster)
			{
				Monster->Damaged(ItemSlot[CurItemIndex].Damage);
			}

			//ATestBossMonsterBase* BossMonster = Cast<ATestBossMonsterBase>(Hit.GetActor());
			//if(nullptr != BossMonster)
			//{
			//	BossMonster->Damaged(ItemSlot[CurItemIndex].Damage);
			//}
		}
	}
}

void AMainCharacter::ChangeMontage_Implementation(EPlayerUpperState _UpperState, bool IsSet)
{
	if (true == IsSet)
	{
		IdleDefault = _UpperState;
	}

	PlayerAnimInst->ChangeAnimation(_UpperState);
	FPVPlayerAnimInst->ChangeAnimation(_UpperState);
	ClientChangeMontage(_UpperState);
}

void AMainCharacter::ClientChangeMontage_Implementation(EPlayerUpperState _UpperState)
{
	PlayerAnimInst->ChangeAnimation(_UpperState);
	FPVPlayerAnimInst->ChangeAnimation(_UpperState);
}

void AMainCharacter::SettingPlayerState_Implementation()
{
	AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	if (nullptr == Con)
	{
		int a = 0;
		return;
	}

	AMainPlayerState* ThisPlayerState = Cast<AMainPlayerState>(Con->PlayerState);
	if (nullptr == ThisPlayerState)
	{
		int a = 0;
		return;
	}

	ThisPlayerState->InitPlayerData();
}

void AMainCharacter::CrouchCameraMove()
{
	if (PointOfView == EPlayerFPSTPSState::FPS)
	{
		switch (LowerStateValue)
		{
		case EPlayerLowerState::Idle:
			SpringArmComponent->SetRelativeLocation(FPVCameraRelLoc_Crouch);
			ChangeLowerState(EPlayerLowerState::Crouch);
			break;
		case EPlayerLowerState::Crouch:
			SpringArmComponent->SetRelativeLocation(FPVCameraRelLoc);
			ChangeLowerState(EPlayerLowerState::Idle);
			break;
		default:
			break;
		}
	}
	else if (PointOfView == EPlayerFPSTPSState::TPS)
	{
		switch (LowerStateValue)
		{
		case EPlayerLowerState::Idle:
			ChangeLowerState(EPlayerLowerState::Crouch);
			break;
		case EPlayerLowerState::Crouch:
			ChangeLowerState(EPlayerLowerState::Idle);
			break;
		default:
			break;
		}
	}
}

void AMainCharacter::MapItemOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetMapItemData = OtherActor;

	AMainPlayerController* MyController = Cast<AMainPlayerController>(GetController());
	if (nullptr == MyController)
	{
		return;
	}

	AMainGameHUD* PlayHUD = Cast<AMainGameHUD>(MyController->GetHUD());
	if (nullptr == PlayHUD)
	{
		return;
	}

	AAreaObject* AreaObject = Cast<AAreaObject>(GetMapItemData);
	if (nullptr != AreaObject)
	{
		// Area�� ��� => "5��Ű�� ���� ��ȣ�ۿ�"
		PlayHUD->UIOn(EUserWidgetType::Num5_Key);
		return;
	}

	// �� ���� ��� => "EŰ�� ���� ��ȣ�ۿ�"
	PlayHUD->UIOn(EUserWidgetType::E_Key);
}

void AMainCharacter::MapItemOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (nullptr != GetMapItemData)
	{
		GetMapItemData = nullptr;
	}

	AMainPlayerController* MyController = Cast<AMainPlayerController>(GetController());
	if (nullptr == MyController)
	{
		return;
	}

	AMainGameHUD* PlayHUD = Cast<AMainGameHUD>(MyController->GetHUD());
	if (nullptr == PlayHUD)
	{
		return;
	}

	// ���� �ִ� ��ȣ�ۿ� ���� UI ��� ����
	PlayHUD->UIOff(EUserWidgetType::Num5_Key);
	PlayHUD->UIOff(EUserWidgetType::E_Key);
}

void AMainCharacter::UpdatePlayerHp(float _DeltaTime)
{
	AMainPlayerState* MyMainPlayerState = GetPlayerState<AMainPlayerState>();
	if (nullptr == MyMainPlayerState)
	{
		return;
	}

	AMainPlayerController* MyController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (nullptr == MyController)
	{
		return;
	}

	float GetHp = MyMainPlayerState->GetPlayerHp();

	// Get HUD
	AMainGameHUD* PlayHUD = Cast<AMainGameHUD>(MyController->GetHUD());
	// if(nullptr != PlayHUD)
	// {
	//     UHpBarUserWidget* MyHpWidget = Cast<UHpBarUserWidget>(PlayHUD->GetWidget(EInGameUIType::HpBar));
	//     MyHpWidget->NickNameUpdate(Token, FText::FromString(FString("")));
	//     MyHpWidget->HpbarUpdate(Token, CurHp, 100.0f);
	// {
}

void AMainCharacter::CheckItem()
{
	// �ʿ� �������� ���� ���.
	if (nullptr == GetMapItemData)
	{
		return;
	}

	AMapObjectBase* GetMapObject = Cast<AMapObjectBase>(GetMapItemData);
	if (nullptr != GetMapObject)
	{
		// 1. �ʿ�����Ʈ�� ���
		InteractObject(GetMapObject);
	}
	else
	{
		// 2. �ֿ� �� �ִ� �������� ���
		AItemBase* GetMapItem = Cast<AItemBase>(GetMapItemData);
		if (nullptr != GetMapItem)
		{
			PickUpItem(GetMapItem);
		}
	}
}

void AMainCharacter::AttackCheck()
{
	switch (IdleDefault)
	{
	case EPlayerUpperState::UArm_Idle:
		ChangeMontage(EPlayerUpperState::UArm_Attack);
		break;
	case EPlayerUpperState::Rifle_Idle:
		ChangeMontage(EPlayerUpperState::Rifle_Attack);
		// FireRayCast();
		break;
	case EPlayerUpperState::Melee_Idle:
		ChangeMontage(EPlayerUpperState::Melee_Attack);
		break;
	default:
		break;
	}
}

void AMainCharacter::Drink()
{
	// ���� üũ
	if (false == IsItemInItemSlot(static_cast<int>(EItemType::Drink)))
	{
		return;
	}

	// ���ᰡ ����!
	SettingItemSocket(static_cast<int>(EItemType::Drink));

	// �ִϸ��̼� ����.
	ChangeMontage(EPlayerUpperState::Drink);
}

void AMainCharacter::DeleteItem(int _Index)
{
	FPlayerItemInformation NewSlot;
	ItemSlot[_Index] = NewSlot;
	IsItemIn[_Index] = false;
}

void AMainCharacter::ChangeIsFaint_Implementation()
{
	AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	
	if (true == IsFaint)
	{
		IsFaint = false;
		if (nullptr != Con)
		{
			Con->FCharacterToFaint.Execute(IsFaint); // Execute -> Delegate ����.
			this->bUseControllerRotationYaw = true;
		}
	}
	else
	{
		IsFaint = true;
		if (nullptr != Con)
		{
			Con->FCharacterToFaint.Execute(IsFaint); // Execute -> Delegate ����.
			this->bUseControllerRotationYaw = false;
		}
	}
}

void AMainCharacter::InteractObject_Implementation(AMapObjectBase* _MapObject)
{
	// Door�� ��� : ��ȣ�ۿ��� Switch�� �ߵ���Ű�Ƿ� return
	ADoorObject* DoorObject = Cast<ADoorObject>(_MapObject);
	if (nullptr != DoorObject)
	{
		return;
	}

	// Area�� ��� : ��ȣ�ۿ��� �÷��̾��ʿ��� ó���ؾ� �ϹǷ� return
	AAreaObject* AreaObject = Cast<AAreaObject>(_MapObject);
	if (nullptr != AreaObject)
	{
		return;
	}

	// �� �� �ʿ�����Ʈ(Switch ��)�� ��� : ��ȣ�ۿ� �ߵ�
	_MapObject->InterAction();
}

void AMainCharacter::BombSetStart_Implementation()
{
	// ��ź ������ üũ
	if (false == ItemSlot[static_cast<int>(EItemType::Bomb)].IsItemIn)
	{
		return;
	}

	// ��ź ��ġ ������ Area üũ
	AAreaObject* AreaObject = Cast<AAreaObject>(GetMapItemData);
	if (nullptr == AreaObject)
	{
		return;
	}

	// ��ź ��ġ ����.
	IsBombSetting = true;
	AreaObject->ResetBombTime();
	ChangeMontage(EPlayerUpperState::Bomb);
}

void AMainCharacter::BombSetTick_Implementation()
{
	if (true == IsBombSetting)
	{
		// ��ź ��ġ ������ Area üũ
		AAreaObject* AreaObject = Cast<AAreaObject>(GetMapItemData);
		if (nullptr == AreaObject)
		{
			return;
		}

		// ��ġ �ð� ī��Ʈ�� ������ ���
		if (0 >= AreaObject->GetInstallBombTime())
		{
			BombSetEnd();
		}

		// ��ġ �ð� ī����
		AreaObject->InstallBomb(GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharacter::BombSetCancel_Implementation()
{
	if (true == IsBombSetting)
	{
		IsBombSetting = false;
		AAreaObject* AreaObject = Cast<AAreaObject>(GetMapItemData);
		if (nullptr != AreaObject)
		{
			AreaObject->ResetBombTime();
		}

		// ���� �ڼ��� �ִϸ��̼� ����
		ChangeMontage(IdleDefault);
	}
}

void AMainCharacter::BombSetEnd_Implementation()
{
	if (true == IsBombSetting)
	{
		// ��ź ��ġ �Ϸ�
		IsBombSetting = false;

		AAreaObject* AreaObject = Cast<AAreaObject>(GetMapItemData);
		if (nullptr != AreaObject)
		{
			AreaObject->InterAction();
		}

		// �κ��丮���� ��ź ������ ����
		DeleteItemInfo(static_cast<int>(EItemType::Bomb));

		// ���� �ڼ��� �ִϸ��̼� ����
		ChangeMontage(IdleDefault);
	}
}

void AMainCharacter::GetSetSelectCharacter_Implementation(UMainGameInstance* _MainGameInstance)
{
	UIToSelectCharacter = _MainGameInstance->GetUIToSelectCharacter();
}

void AMainCharacter::DeleteItemInfo(int _Index)
{
	FPlayerItemInformation DeleteSlot;
	ItemSlot[_Index] = DeleteSlot;
}

bool AMainCharacter::IsItemInItemSlot(int _Index)
{
	return ItemSlot[_Index].IsItemIn;
}

void AMainCharacter::ChangePOV()
{
	if (PointOfView == EPlayerFPSTPSState::FPS)
	{
		// SpringArm ��ġ ����
		SpringArmComponent->TargetArmLength = 300.0f;
		SpringArmComponent->SetRelativeLocation(CameraRelLoc);

		// Character Mesh ��ȯ
		GetMesh()->SetOwnerNoSee(false);
		FPVMesh->SetOwnerNoSee(true);

		// Item Mesh
		for (int i = 0; i < static_cast<int>(EPlayerUpperState::UArm_Attack); i++)
		{
			ItemSocketMesh->SetOwnerNoSee(false);
			FPVItemSocketMesh->SetOwnerNoSee(true);
		}

		// ����Ī -> ����Ī
		PointOfView = EPlayerFPSTPSState::TPS;
	}
	else if (PointOfView == EPlayerFPSTPSState::TPS)
	{
		// SpringArm ��ġ ����
		SpringArmComponent->TargetArmLength = 0.0f;
		switch (LowerStateValue)
		{
		case EPlayerLowerState::Idle:
			SpringArmComponent->SetRelativeLocation(FPVCameraRelLoc);
			break;
		case EPlayerLowerState::Crouch:
			SpringArmComponent->SetRelativeLocation(FPVCameraRelLoc_Crouch);
			break;
		default:
			break;
		}

		// Character Mesh ��ȯ.
		GetMesh()->SetOwnerNoSee(true);
		FPVMesh->SetOwnerNoSee(false);

		// Item Mesh
		for (int i = 0; i < static_cast<int>(EPlayerUpperState::UArm_Attack); i++)
		{
			ItemSocketMesh->SetOwnerNoSee(true);
			FPVItemSocketMesh->SetOwnerNoSee(false);
		}

		// ����Ī -> ����Ī
		PointOfView = EPlayerFPSTPSState::FPS;
	}
}

void AMainCharacter::CharacterReload()
{
	if (-1 == CurItemIndex)
	{
		return;
	}

	// Widget �����
	Reload_Widget->SetVisibility(ESlateVisibility::Hidden);

	// �Ѿ� ������ ����.
	ItemSlot[CurItemIndex].ReloadLeftNum = ItemSlot[CurItemIndex].ReloadMaxNum;

	// ����� �Ѿ� ������ ȣ��.
	AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	if (nullptr != Con)
	{
		Con->FCharacterToReload.Execute(); // Execute -> Delegate ����.
	}
}

void AMainCharacter::HandAttackCollision(AActor* _OtherActor, UPrimitiveComponent* _Collision)
{
	{
		ABasicMonsterBase* Monster = Cast<ABasicMonsterBase>(_OtherActor);
		if (nullptr != Monster)
		{
			Monster->Damaged(50.0f);
		}
	}

	{
		//ATestBossMonsterBase* BossMonster = Cast<ATestBossMonsterBase>(_OtherActor); // ���� Main���� �ٲ�� ��.
		//if (nullptr != BossMonster)
		//{
		//	BossMonster->Damaged(50.0f);
		//}
	}
}

void AMainCharacter::NetCheck()
{
	IsServer = GetWorld()->GetAuthGameMode() != nullptr;
	IsClient = !IsServer;

	if (true == IsServer)
	{
		IsCanControlled = (GetLocalRole() == ROLE_Authority) ? true : false;
	}
	else // client
	{
		IsCanControlled = (GetLocalRole() == ROLE_AutonomousProxy) ? true : false;
	}

	if (true == IsServer)
	{
		UMainGameInstance* Inst = GetGameInstance<UMainGameInstance>();
		if (nullptr == Inst)
		{
			return;
		}
		// ����ū�� �� �ε����� �ƴϴ�.
		Token = Inst->GetNetToken();
	}
}

void AMainCharacter::SendTokenToHpBarWidget()
{
	AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	if (nullptr == Con)
	{
		return;
	}

	AMainGameHUD* PlayHUD = Cast<AMainGameHUD>(Con->GetHUD());
	if (nullptr == PlayHUD)
	{
		return;
	}

	//UTestHpBarUserWidget* MyHpWidget = Cast<UTestHpBarUserWidget>(PlayHUD->GetWidget(EUserWidgetType::HpBar));
	//if (nullptr == MyHpWidget)
	//{
	//	return;
	//}

	//if (true == IsCanControlled && -1 != Token)
	//{
	//	MyHpWidget->HpbarInit_ForMainPlayer(Token);
	//}
}

TArray<struct FPlayerItemInformation> AMainCharacter::GetItemSlot()
{
	return ItemSlot;
}
