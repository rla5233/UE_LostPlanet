// Fill out your copyright notice in the Description page of Project Settings.


#include "PartDevLevel/Character/TestFPVPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Global/DataAssets/InputDatas.h"
#include "TestFPVCharacter.h"

ATestFPVPlayerController::ATestFPVPlayerController()
{
	TeamId = FGenericTeamId(0);
	{
		FString RefPathString = TEXT("InputDatas'/Game/Resources/Global/DataAssets/DA_InputDatas.DA_InputDatas'");
		ConstructorHelpers::FObjectFinder<UInputDatas> ResPath(*RefPathString);

		if (false == ResPath.Succeeded())
		{
			return;
		}

		InputData = ResPath.Object;
	}
}

void ATestFPVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FGetItemToWidget_Test_FPV.BindUObject(this, &ATestFPVPlayerController::CallGetItem);

	// Camera(Controller) Shake
	Stream.GenerateNewSeed();
}

void ATestFPVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

		InputSystem->ClearAllMappings();
		InputSystem->AddMappingContext(InputData->InputMapping, 0);

		if (nullptr != InputData->InputMapping)
		{
			EnhancedInputComponent->BindAction(InputData->Actions[0], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MouseRotation);
			EnhancedInputComponent->BindAction(InputData->Actions[3], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveRight);
			EnhancedInputComponent->BindAction(InputData->Actions[4], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveLeft);
			EnhancedInputComponent->BindAction(InputData->Actions[1], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveFront);
			EnhancedInputComponent->BindAction(InputData->Actions[2], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::MoveBack);
			//EnhancedInputComponent->BindAction(InputData->Actions[5], ETriggerEvent::Started, this, &ATestPlayerController::Jump);
			//EnhancedInputComponent->BindAction(InputData->Actions[5], ETriggerEvent::Completed, this, &ATestPlayerController::JumpEnd);
			//EnhancedInputComponent->BindAction(InputData->Actions[6], ETriggerEvent::Started, this, &ATestPlayerController::FireStart);
			//EnhancedInputComponent->BindAction(InputData->Actions[6], ETriggerEvent::Triggered, this, &ATestPlayerController::FireTick);
			//EnhancedInputComponent->BindAction(InputData->Actions[6], ETriggerEvent::Completed, this, &ATestPlayerController::FireEnd);
			EnhancedInputComponent->BindAction(InputData->Actions[7], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePosture_Con, 0);	// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������)
			EnhancedInputComponent->BindAction(InputData->Actions[8], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePosture_Con, 1);	// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������)
			EnhancedInputComponent->BindAction(InputData->Actions[9], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePosture_Con, 2);	// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������)
			EnhancedInputComponent->BindAction(InputData->Actions[10], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::Drink_Con);			// => ���� ���� �ʿ� (24.07.30 �ش� Ű�� ������ �Լ� �����)
			EnhancedInputComponent->BindAction(InputData->Actions[11], ETriggerEvent::Started, this, &ATestFPVPlayerController::BombSetStart_Con);		// => ���� ���� �ʿ� (24.07.30 �ش� Ű�� ������ �Լ� �����)
			EnhancedInputComponent->BindAction(InputData->Actions[11], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::BombSetTick_Con);	// => ���� ���� �ʿ� (24.07.31 �߰���)
			EnhancedInputComponent->BindAction(InputData->Actions[11], ETriggerEvent::Completed, this, &ATestFPVPlayerController::BombSetCancel_Con);	// => ���� ���� �ʿ� (24.07.30 �ش� Ű�� ������ �Լ� �����)
			EnhancedInputComponent->BindAction(InputData->Actions[20], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePosture_Con, -1);	// => ���� ���� �ʿ� (24.07.30 �ش� �Լ��� ������ Ű �����) ('0' -> 'X')
			EnhancedInputComponent->BindAction(InputData->Actions[13], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::CheckItem_Con);		// => ���� ���� �ʿ� (24.07.29 �ش� Ű�� ������ �Լ� �����) (PickUpItem -> CheckItem)
			EnhancedInputComponent->BindAction(InputData->Actions[14], ETriggerEvent::Triggered, this, &ATestFPVPlayerController::ChangePOV_Con);		// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������)
			EnhancedInputComponent->BindAction(InputData->Actions[15], ETriggerEvent::Started, this, &ATestFPVPlayerController::Crouch);
			EnhancedInputComponent->BindAction(InputData->Actions[16], ETriggerEvent::Started, this, &ATestFPVPlayerController::IAReload);
		}
	}
}

void ATestFPVPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void ATestFPVPlayerController::MouseRotation(const FInputActionValue& Value)
{
	FVector2D MouseXY = Value.Get<FVector2D>();
	AddYawInput(MouseXY.X);
	AddPitchInput(-MouseXY.Y);
}

void ATestFPVPlayerController::MoveFront(const FInputActionValue& Value)
{
	FVector Forward = GetPawn()->GetActorForwardVector();
	GetPawn()->AddMovementInput(Forward);
	ChangePlayerDir(EPlayerMoveDir::Forward);
}

void ATestFPVPlayerController::MoveBack(const FInputActionValue& Value)
{
	FVector Backward = GetPawn()->GetActorForwardVector();
	GetPawn()->AddMovementInput(-Backward);
	ChangePlayerDir(EPlayerMoveDir::Back);
}

void ATestFPVPlayerController::MoveRight(const FInputActionValue& Value)
{
	FVector Rightward = GetPawn()->GetActorRightVector();
	GetPawn()->AddMovementInput(Rightward);
	ChangePlayerDir(EPlayerMoveDir::Right);
}

void ATestFPVPlayerController::MoveLeft(const FInputActionValue& Value)
{
	FVector Leftward = GetPawn()->GetActorRightVector();
	GetPawn()->AddMovementInput(-Leftward);
	ChangePlayerDir(EPlayerMoveDir::Left);
}

void ATestFPVPlayerController::Crouch(const FInputActionValue& Value)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	if (nullptr == Ch)
	{
		return;
	}

	Ch->CrouchCameraMove();
}

//void ATestFPVPlayerController::Jump(const FInputActionValue& Value)
//{
//	ChangeStateController(EPlayerState::Jump);
//	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
//	Ch->Jump();
//}
//
//void ATestFPVPlayerController::JumpEnd()
//{
//	ChangeStateController(EPlayerState::Idle);
//	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
//	Ch->StopJumping();
//}

void ATestFPVPlayerController::FireStart(float _DeltaTime)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	if (nullptr == Ch)
	{
		return;
	}
	Ch->AttackCheck();
	IsGunFire = true;
}

void ATestFPVPlayerController::FireTick(float _DeltaTime)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	if (nullptr == Ch)
	{
		return;
	}

	// ���� ���϶���
	if (true == IsGunFire || Ch->IdleDefault == EPlayerUpperState::Rifle_Idle)
	{
		Ch->AttackCheck();
	}

	// Camera(Controller) Shake
	float ShakeX = Stream.FRandRange(-0.2f, 0.2f);
	float ShakeY = Stream.FRandRange(0.0f, 0.2f);
	FRotator(ShakeX, ShakeY, 0.0f);

	//FRotator ShakeRotator = FRotator(FQuat::Slerp(Rotation1.ToQuat(), Rotation2.ToQuat(), Alpha));
	//SetControlRotation(ShakeRotator);

	MouseRotation(FInputActionValue(FVector2D(ShakeX, ShakeY)));
	
}

void ATestFPVPlayerController::FireEnd()
{
	IsGunFire = false;
//	GetWorld()->GetTimerManager().ClearTimer(MyTimeHandle);
//	
//	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
//	if (nullptr == Ch)
//	{
//		return;
//	}
//
//	Ch->AttackEndCheck();
//
//#ifdef WITH_EDITOR
//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString(TEXT("Attack End")));
//#endif // WITH_EDITOR
}

void ATestFPVPlayerController::Drink_Con()	// => ���ο� ���� �����ؾ� �� (24.07.29 �߰� �� �׽��� ��) => ���� ����(�ּ�)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->Drink();
}

void ATestFPVPlayerController::BombSetStart_Con()	// => ���ο� ���� �����ؾ� �� (24.07.29 �߰� �� �׽��� ��) => ���� ����(�ּ�)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->BombSetStart();
}

void ATestFPVPlayerController::BombSetTick_Con()	// => ���ο� ���� �����ؾ� �� (24.07.31 �߰� �� �׽��� ��)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->BombSetTick();
}

void ATestFPVPlayerController::BombSetCancel_Con()	// => ���ο� ���� �����ؾ� �� (24.07.29 �߰� �� �׽��� ��) => ���� ����(�ּ�)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->BombSetCancel();
}

void ATestFPVPlayerController::CheckItem_Con()	// => �������� ���� �ʿ� (24.07.29 �߰���) => ���� ����
{
	// E Key
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->CheckItem();
}

void ATestFPVPlayerController::ChangePosture_Con(int _InputKey)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();

	if (_InputKey == 0) // ��
	{
		Ch->ChangeMontage(EPlayerUpperState::Rifle_Idle, true);
		//ChangePostureToWidget(0); // BP To Event 
		//ChangePostureToWidget(EPlayerUpperState::Rifle_Idle); // �Ƹ�?
	}
	else if (_InputKey == 1) // Į
	{
		Ch->ChangeMontage(EPlayerUpperState::Melee_Idle, true);
		//ChangePostureToWidget(EPlayerPosture::Rifle2); // BP To Event
		//ChangePostureToWidget(EPlayerUpperState::Rifle_Idle);
	}
	else if (_InputKey == -1) // �ָ�
	{
		Ch->ChangeMontage(EPlayerUpperState::UArm_Idle, true);
	}

	Ch->SettingItemSocket(_InputKey);
}

void ATestFPVPlayerController::ChangePOV_Con()
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->ChangePOV();
}

void ATestFPVPlayerController::IAReload()
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->CharacterReload();
}

// ĳ������ ChangeLowerState_Implementation�� ����ϰ� �����Ƿ� ���⼭�� ������� �ʴ� �Լ�.
//void ATestFPVPlayerController::ChangeLowerState(EPlayerLowerState _State)
//{
//	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
//	Ch->ChangeLowerState(_State);
//}

void ATestFPVPlayerController::ChangePlayerDir(EPlayerMoveDir _Dir)
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->ChangePlayerDir(_Dir);
}

//void ATestFPVPlayerController::PlayerMontagePlay()
//{
//	// ���� ���� �ʿ�
//	// ����, ��ź ��ġ ���� ��Ÿ�� ���� �Ұ���
//	if (PlayerIsFaint == true || PlayerIsBombSetting == true)
//	{
//		return;
//	}
//
//	//ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
//	//Ch->ChangeMontage(false);
//}

//void ATestFPVPlayerController::FireEndMontagePlay()
//{
//	// ���� ���� �ʿ�
//	// ����, ��ź ��ġ ���� ��Ÿ�� ���� �Ұ���
//	if (PlayerIsFaint == true || PlayerIsBombSetting == true)
//	{
//		return;
//	}
//
//	//ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
//	//Ch->ChangeMontage(true);
//}

void ATestFPVPlayerController::SetFaint()
{
	ATestFPVCharacter* Ch = GetPawn<ATestFPVCharacter>();
	Ch->ChangeIsFaint();
}

FGenericTeamId ATestFPVPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void ATestFPVPlayerController::CallGetItem()
{
	CallGetItemToWidget();
}








/*
* ChangePOV (Client)
* ChangePosture
* ChangeMontage
* ChangeAnimation
* ChangeLowerState
* ChangePlayerDir
* ChangeIsFaint
*/

/*
* EPlayerUpperState -> Mon
* 
* EPlayerPosture -> Seq
* PlayerDir
* PlayerLowerState
*/
