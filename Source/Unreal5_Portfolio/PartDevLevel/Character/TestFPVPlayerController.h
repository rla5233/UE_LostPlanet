// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Global/ContentsEnum.h"
#include "GenericTeamAgentInterface.h"
#include "TestFPVPlayerController.generated.h"

DECLARE_DELEGATE(FDelegate_GetItem_Test_FPV);

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API ATestFPVPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ATestFPVPlayerController();

	FDelegate_GetItem_Test_FPV FGetItemToWidget_Test_FPV;

protected:
	void BeginPlay() override;

public :

	// Input
	UPROPERTY()
	class UInputDatas* InputData;

	UFUNCTION(BlueprintCallable)
	void SetupInputComponent() override;

	UFUNCTION()
	void PlayerTick(float DeltaTime) override;

	// Actions
	UFUNCTION(BlueprintCallable)
	void MouseRotation(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void MoveFront(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void MoveBack(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void MoveRight(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void MoveLeft(const FInputActionValue& Value);
	
	//UFUNCTION(BlueprintCallable)
	//void Jump(const FInputActionValue& Value);
	//UFUNCTION(BlueprintCallable)
	//void JumpEnd(const FInputActionValue& Value);

	// ��� �� (��ȯ)
	UFUNCTION(BlueprintCallable)
	void Crouch(const FInputActionValue& Value);

	// Fire
	UFUNCTION(BlueprintCallable)
	void FireStart(float _DeltaTime);
	UFUNCTION(BlueprintCallable)
	void FireTick(float _DeltaTime);
	UFUNCTION(BlueprintCallable)
	void FireEnd();

	// Drink
	UFUNCTION(BlueprintCallable)
	void Drink_Con();	// => ���ο� ���� �����ؾ� �� (24.07.30 �Լ� �̸� ����, �׽��� ��) => ���� ����(�ּ�)

	// Bomb Setting
	UFUNCTION(BlueprintCallable)
	void BombSetStart_Con();	// => ���ο� ���� �����ؾ� �� (24.07.30 �Լ� �̸� ����, �׽��� ��) => ���� ����(�ּ�)
	UFUNCTION(BlueprintCallable)
	void BombSetTick_Con();		// => ���ο� ���� �����ؾ� �� (24.07.31 �߰���, �׽��� ��)
	UFUNCTION(BlueprintCallable)
	void BombSetCancel_Con();	// => ���ο� ���� �����ؾ� �� (24.07.30 �Լ� �̸� ����, �׽��� ��) => ���� ����(�ּ�)

	// Item
	UFUNCTION(BlueprintCallable)
	void CheckItem_Con();	// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������) (PickUpItem �Լ� ��ü) => ���� ����

	// Posture
	UFUNCTION(BlueprintCallable)
	void ChangePosture_Con(int _InputKey);
	//void ChangePosture_Con(EPlayerPosture _Posture);	// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������) => ���� ����

	// POV
	UFUNCTION(BlueprintCallable)
	void ChangePOV_Con();	// => ���� ���� �ʿ� (24.07.30 �÷��̾� �Լ����� ȥ���� �������� ���� �̸� ������) => ���� ����

	// Reload (k)
	UFUNCTION(BlueprintCallable)
	void IAReload();

	// LowerStateChange �Լ� (��ȯ) => ���� ����
	//UFUNCTION(BlueprintCallable)
	//void ChangeLowerState(EPlayerLowerState _State);

	// PlayerDirChange �Լ� (��ȯ) => ���� ����
	UFUNCTION(BlueprintCallable)
	void ChangePlayerDir(EPlayerMoveDir _Dir);

	// ���� ��Ÿ�� ���� �Լ�  => ���� ����
	//UFUNCTION(BlueprintCallable)
	//void PlayerMontagePlay();

	// ���� ���� ��Ÿ�� ���� �Լ�  => ���� ����
	//UFUNCTION(BlueprintCallable)
	//void FireEndMontagePlay();

	//�׽�Ʈ��
	UFUNCTION(BlueprintCallable)
	void SetFaint();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TeamId", meta = (AllowPrivateAccess = true))
	FGenericTeamId TeamId; // => ���� ����
	virtual FGenericTeamId GetGenericTeamId() const override; // => ���� ����

	FTimerHandle MyTimeHandle; // => ���� ����
	int Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool PlayerIsFaint = false; // => ���� ����(Delegate)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool PlayerIsBombSetting = false; // ���� ���� �ʿ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsGunFire = false;

	// Camera(Controller) Shake
	UPROPERTY()
	FRandomStream Stream;

	// HUD / Widget
	UFUNCTION()
	void CallGetItem(); // => ���� ����(Delegate)

protected: // => ���� ����(Delegate)
	//UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = true))
	//void ChangePostureToWidget(EPlayerPosture _Posture);

	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = true))
	void GetItemToWidget();

	UFUNCTION(BlueprintImplementableEvent, meta = (CallInEditor = true))
	void CallGetItemToWidget();
};
