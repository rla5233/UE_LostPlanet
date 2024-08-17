// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/ContentsEnum.h"
#include "Net/UnrealNetwork.h"
#include "PartDevLevel/Character/ParentsCharacter.h"
#include "MainCharacter.generated.h"

UCLASS()
class UNREAL5_PORTFOLIO_API AMainCharacter : public AParentsCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:

	/// <summary>
	/// Component �ʱ�ȭ ���� ȣ��.
	/// </summary>
	void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void AnimationEnd() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AnimInstance
public :
	// ��ü ���� (Controller ���� ȣ����. -> ���߿� ���� �ʿ�.)
	UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerLowerState LowerStateValue = EPlayerLowerState::Idle;

	// ��ü ����
	UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerUpperState UpperStateValue = EPlayerUpperState::UArm_Idle;
		
	// ĳ���� ���� ����
	UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerMoveDir DirValue = EPlayerMoveDir::Forward;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerUpperState IdleDefault = EPlayerUpperState::UArm_Idle;

	// ĳ���� ���� ����.
	UPROPERTY(Category = "Contents"/*, Replicated*/, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsFaint = false;

private : // ���� �߻� ���� ���� �߻��ϸ� �׳� ������ ��.
	// == Components ==
	
	// ��������
	// ī�޶�
	// ����Ī �޽�
	// �̴ϸ� ������
	// �ʿ� �ִ� ������ Ž�� ���� �ݸ���
	// ������ ���� ����
	// 1��Ī ������ ���� ����
	// ���� ���ݿ� ���

		
	// == ��Ī ���� ���� ==
	UPROPERTY()
	EPlayerFPSTPSState PointOfView = EPlayerFPSTPSState::FPS;

	// == Inventory ==
	// ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<bool> IsItemIn;
	// ���� ������ ����.
	UPROPERTY(VisibleAnywhere)
	TArray<struct FPlayerItemInformation> ItemSlot;
	// ���� ������ Index
	UPROPERTY(VisibleAnywhere)
	int CurItemIndex = -1;

	// ��ź ��ġ ���� ��Ȳ
	UPROPERTY()
	bool IsBombSetting = false;


	// �ʿ� �ִ� ���� Data
	UPROPERTY(Category = "Contents", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* GetMapItemData = nullptr;

	// ��ü ����
	UPROPERTY()
	class UPlayerAnimInstance* PlayerAnimInst = nullptr;
	UPROPERTY()
	class UPlayerAnimInstance* FPVPlayerAnimInst = nullptr;
	
	UPROPERTY(Replicated)
	FName UIToSelectCharacter = "";

	// == Server ==
public :
	// ���� ����
	UFUNCTION(Reliable, Server)
	void ChangeLowerState(EPlayerLowerState _LowerState);
	void ChangeLowerState_Implementation(EPlayerLowerState _LowerState);

	// ĳ���� ����
	UFUNCTION(Reliable, Server)
	void ChangePlayerDir(EPlayerMoveDir _Dir);
	void ChangePlayerDir_Implementation(EPlayerMoveDir _Dir);

	// Fire Ray Cast
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void FireRayCast();
	void FireRayCast_Implementation();

	UFUNCTION(Reliable, Server)
	void ChangeMontage(EPlayerUpperState _UpperState, bool IsSet = false);
	void ChangeMontage_Implementation(EPlayerUpperState _UpperState, bool IsSet = false);

	UFUNCTION(Reliable, NetMulticast)
	void ClientChangeMontage(EPlayerUpperState _UpperState);
	void ClientChangeMontage_Implementation(EPlayerUpperState _UpperState);

	UFUNCTION(Reliable, Server)
	void SettingPlayerState();
	void SettingPlayerState_Implementation();

	UFUNCTION(Reliable, Server)
	void ChangeIsFaint();
	void ChangeIsFaint_Implementation();

	UFUNCTION(Reliable, Server)
	void InteractObject(AMapObjectBase* _MapObject);
	void InteractObject_Implementation(AMapObjectBase* _MapObject);

	UFUNCTION(Reliable, Server)
	void BombSetStart();
	void BombSetStart_Implementation();
	UFUNCTION(Reliable, Server)
	void BombSetTick();
	void BombSetTick_Implementation();
	UFUNCTION(Reliable, Server)
	void BombSetCancel();
	void BombSetCancel_Implementation();
	UFUNCTION(Reliable, Server)
	void BombSetEnd();
	void BombSetEnd_Implementation();

	/// <summary>
	/// Crouch �� ���� ī�޶� �̵�
	/// </summary>
	UFUNCTION()
	void CrouchCameraMove();

	// == Client ==
private :
	const FVector CameraRelLoc = FVector(0.0f, 60.0f, 110.0f);
	const FVector FPVCameraRelLoc = FVector(0.0f, 0.0f, 72.0f);
	const FVector FPVCameraRelLoc_Crouch = FVector(10.0f, 0.0f, 10.0f);

	UFUNCTION(BlueprintCallable)
	void MapItemOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void MapItemOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHp(float _DeltaTime);

	UFUNCTION(BlueprintCallable)
	void DeleteItem(int _Index);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void GetSetSelectCharacter(class UMainGameInstance* _MainGameInstance);
	void GetSetSelectCharacter_Implementation(class UMainGameInstance* _MainGameInstance);

	UFUNCTION(Reliable, Server)
	void DestroyItem(AItemBase* _Item);
	void DestroyItem_Implementation(AItemBase* _Item);

	UFUNCTION(Reliable, Server)
	void SetItemSocketVisibility(bool _Visibility);
	void SetItemSocketVisibility_Implementation(bool _Visibility);

	UFUNCTION(Reliable, Server)
	void SetItemSocketMesh(UStaticMesh* _ItemMeshRes, FVector _ItemRelLoc, FRotator _ItemRelRot, FVector _ItemRelScale);
	void SetItemSocketMesh_Implementation(UStaticMesh* _ItemMeshRes, FVector _ItemRelLoc, FRotator _ItemRelRot, FVector _ItemRelScale);

	UFUNCTION(Reliable, Server)
	void SpawnItem(FName _ItemName, FTransform _SpawnTrans);
	void SpawnItem_Implementation(FName _ItemName, FTransform _SpawnTrans);

	// ������ ����
	UFUNCTION(BlueprintCallable)
	void PickUpItem(class AItemBase* _Item);

	// ������ ���� -> ���
	UFUNCTION(BlueprintCallable)
	void DropItem(int _SlotIndex);

	UFUNCTION()
	void DeleteItemInfo(int _Index);

	UFUNCTION()
	bool IsItemInItemSlot(int _Index);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsServer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsClient = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsCanControlled = false;

	UPROPERTY(Category = "PlayerNet", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Token = -1;

	// == Client ==
public :
	UFUNCTION(BlueprintCallable)
	void CheckItem();

	UFUNCTION()
	void AttackCheck();

	UFUNCTION()
	void Drink();

	// ���Ͽ� ������ ����.
	UFUNCTION()
	void SettingItemSocket(int _InputKey);

	UFUNCTION()
	FORCEINLINE EPlayerUpperState GetIdleDefault() const
	{
		return IdleDefault;
	}

public :
	// == ��Ī ���� �Լ� ==
	UFUNCTION()
	void ChangePOV();

	// ���ε�.
	UFUNCTION()
	void CharacterReload();

	UFUNCTION(BlueprintCallable)
	void HandAttackCollision(AActor* _OtherActor, UPrimitiveComponent* _Collision);

	UFUNCTION()
	void NetCheck();

	UFUNCTION()
	void SendTokenToHpBarWidget();

	// ĳ���� ���(�κ��丮) ����.
	UFUNCTION(BlueprintCallable)
	TArray<struct FPlayerItemInformation> GetItemSlot();

protected :
	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UGetItem_UserWidget* Reload_Widget = nullptr;
};

/** BP
* MovementComponent -> WalkableFloorAngle �� ���� 60.0���� ����.
* ĸ�� �ݸ��� Player�� ����.
*/