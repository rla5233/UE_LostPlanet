// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Global/ContentsEnum.h"
#include "ParentsCharacter.h"
#include "TestFPVCharacter.generated.h"

// For Character Part's Testing : First person point of view 

// Inventory				// => ����ĳ���ͷ� �����ؾ� ��
USTRUCT(BlueprintType)
struct FFPVItemInformation
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))	// => ���� ���� �ʿ� (24.08.06 �߰���)
	bool IsItemIn = false;	// �κ��丮 ĭ�� �������� ��� �ִ��� ����

	// ItemName
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName Name = "";

	// Bullet Count, Damage
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ReloadMaxNum = -1;	// �ѱ�� ���� ź �ִ� ����
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ReloadLeftNum = -1;	// �ѱ�� ���� ź ���� ����
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Damage = 0;			// ���� ���ݷ�

	// Static Mesh
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* MeshRes = nullptr;			// ����ƽ �޽� ���ҽ�
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector RelLoc = FVector(0.0f, 0.0f, 0.0f);		// ����ƽ �޽� ������Ʈ ����� ��ġ
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator RelRot = FRotator(0.0f, 0.0f, 0.0f);	// ����ƽ �޽� ������Ʈ ����� ȸ��
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector RelScale = FVector(1.0f, 1.0f, 1.0f);	// ����ƽ �޽� ������Ʈ ����� ũ��
};


UCLASS()
class UNREAL5_PORTFOLIO_API ATestFPVCharacter : public AParentsCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestFPVCharacter();

	// LowerState (��ȯ)
	UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerLowerState LowerStateValue = EPlayerLowerState::Idle;

	// Dir
	UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerMoveDir DirValue = EPlayerMoveDir::Forward;

	// 7/26 �߰� ��������
	UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsFaint = false;

	// LowerStateChange �Լ� (��ȯ)
	UFUNCTION(Reliable, Server)
	void ChangeLowerState(EPlayerLowerState _LowerState);
	void ChangeLowerState_Implementation(EPlayerLowerState _LowerState);

	// DirChange �Լ� (��ȯ)
	UFUNCTION(Reliable, Server)
	void ChangePlayerDir(EPlayerMoveDir _Dir);
	void ChangePlayerDir_Implementation(EPlayerMoveDir _Dir);

	// 7/26 �������·� �����ϴ� �Լ� (��ȯ)
	UFUNCTION(Reliable, Server)
	void ChangeIsFaint();
	void ChangeIsFaint_Implementation();

	// POV
	const FVector CameraRelLoc = FVector(0.0f, 60.0f, 110.0f);			// => ����ĳ���� ���� �ʿ� (24.07.29 �߰���) => ���� ����.
	const FVector FPVCameraRelLoc = FVector(10.0f, 0.0f, 72.0f);		// => ���� ���� �ʿ� (24.08.02 ������)
	const FVector FPVCameraRelLoc_Crouch = FVector(10.0f, 0.0f, 10.0f);	// => ����ĳ���� ���� �ʿ� (24.07.29 �߰���) => ���� ����.
	//bool IsFPV = true;
	EPlayerFPSTPSState PointOfView = EPlayerFPSTPSState::FPS;
	UFUNCTION()
	void ChangePOV();
	UFUNCTION()
	void CharacterReload();

	// Inventory => ����ĳ���ͷ� �����ؾ� �� (PickUpItem �Լ��� �ʿ�)
	UPROPERTY(/*Replicated, */VisibleAnywhere, BlueprintReadOnly)
	TArray<FFPVItemInformation> ItemSlot;							// => ���� ���� �ʿ� (24.08.06 ������)
	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)		// => ���ο��� ���� �ʿ� (24.08.06 ������)
	//TArray<bool> IsItemIn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurItemIndex = -1;
	UFUNCTION()
	bool IsItemInItemSlot(int _Index);

	// Item
	UPROPERTY(Category = "Contents", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* GetMapItemData = nullptr;	// �ʿ� �ִ� ���� Data
	//UPROPERTY(Category = "Contents", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//FString RayCastToItemName = "";
	UFUNCTION(BlueprintCallable)
	void MapItemOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// => ���� ���� �ʿ� (24.08.01 ������)
	UFUNCTION(BlueprintCallable)
	void MapItemOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	// => ���� ���� �ʿ� (24.08.01 ������)
	UFUNCTION(BlueprintCallable)
	void CheckItem();										// => ����ĳ���ͷ� �����ؾ� �� (24.07.29 �߰���)
	UFUNCTION(Reliable, Server)
	void InteractObject(AMapObjectBase* _MapObject);		// => ����ĳ���ͷ� �����ؾ� �� (24.07.29 �߰���)
	void InteractObject_Implementation(AMapObjectBase* _MapObject);
	UFUNCTION(/*Reliable, Server, */BlueprintCallable)
	void PickUpItem(AItemBase* _Item);						// => ���� ���� �ʿ� (24.08.06 ������)
	//void PickUpItem_Implementation(AItemBase* _Item);
	//UFUNCTION(BlueprintCallable)
	//void ItemSetting(FName _TagName, EPlayerUpperState _SlotIndex);	// => ���� ���� �ʿ� (24.08.06 ������)
	UFUNCTION(/*Reliable, Server, */BlueprintCallable)
	void DropItem(int _SlotIndex);							// => ���� ���� �ʿ� (24.08.06 ������)
	//void DropItem_Implementation(int _SlotIndex);
	UFUNCTION(Reliable, Server)
	void DestroyItem(AItemBase* _Item);						// => ���� ���� �ʿ� (24.08.06 �߰���)
	void DestroyItem_Implementation(AItemBase* _Item);
	UFUNCTION(Reliable, Server)
	void SpawnItem(FName _ItemName, FTransform _SpawnTrans);			// => ���� ���� �ʿ� (24.08.06 �߰���)
	void SpawnItem_Implementation(FName _ItemName, FTransform _SpawnTrans);
	UFUNCTION(BlueprintCallable)
	void DeleteItemInfo(int _Index);						// => ���� ���� �ʿ� (24.08.06 �Լ� �̸� ������)

	// Collision
	//UFUNCTION(BlueprintCallable)
	//void Collision(AActor* _OtherActor, UPrimitiveComponent* _Collision);

	// NotifyState���� ��� �� (��ȯ)
	UFUNCTION(BlueprintCallable)
	void HandAttackCollision(AActor* _OtherActor, UPrimitiveComponent* _Collision);

	UFUNCTION()
	void SendTokenToHpBarWidget();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHp(float _DeltaTime);

	UFUNCTION(Reliable, Server)
	void SettingPlayerState();
	void SettingPlayerState_Implementation();

	UFUNCTION(BlueprintCallable)
	void ShowMuzzle();

protected:
	// ���� �÷��̾� �߰� �ʿ� �ڵ� (��ȯ) 07/24
	void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AnimationEnd() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	// ��Ÿ�� ���濡 ��� �� (��ȯ)
	UPROPERTY()
	class UPlayerAnimInstance* PlayerAnimInst = nullptr;
	UPROPERTY()
	class UPlayerAnimInstance* FPVPlayerAnimInst = nullptr;

public:
	// Fire
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void FireRayCast();
	void FireRayCast_Implementation();

	// Drink
	UFUNCTION(BlueprintCallable)
	void Drink();
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void DrinkComplete();
	void DrinkComplete_Implementation();

	void ChangeHandAttackCollisionProfile(FName _Name);

	// Bomb Setting
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool IsBombSetting = false;	// => ���ο� ���� �ʿ� (24.07.29 �߰���)
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void BombSetStart();		// => ���ο� ���� �ʿ� (24.07.31 ������)
	void BombSetStart_Implementation();
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void BombSetTick();			// => ���ο� ���� �ʿ� (24.07.31 ������)
	void BombSetTick_Implementation();
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void BombSetCancel();		// => ���ο� ���� �ʿ� (24.07.31 ������)
	void BombSetCancel_Implementation();
	UFUNCTION(Reliable, Server, BlueprintCallable)
	void BombSetComplete();		// => ���ο� ���� �ʿ� (24.07.31 ������)
	void BombSetComplete_Implementation();

	// Animation, Montage
	UFUNCTION(Reliable, Server)				// ���� �� ���� ĳ���� ��Ÿ�� ���� �Լ� (��ȯ)
	void ChangeMontage(EPlayerUpperState _UpperState, bool IsSet = false);
	void ChangeMontage_Implementation(EPlayerUpperState _UpperState, bool IsSet = false);
	UFUNCTION(Reliable, NetMulticast)		// ���� �� ���� ĳ���� ��Ÿ�� ���� �Լ� (��ȯ)
	void ClientChangeMontage(EPlayerUpperState _UpperState);
	void ClientChangeMontage_Implementation(EPlayerUpperState _UpperState);

	UFUNCTION(/*Reliable, Server*/)
	void SettingItemSocket(int _InputKey);	// => ���ο� ���� �ʿ� (24.08.06 �߰���)
	//void SettingItemSocket_Implementation(int _InputKey);
	UFUNCTION(Reliable, Server)
	void SetItemSocketMesh(UStaticMesh* _ItemMeshRes, FVector _ItemRelLoc, FRotator _ItemRelRot, FVector _ItemRelScale);
	void SetItemSocketMesh_Implementation(UStaticMesh* _ItemMeshRes, FVector _ItemRelLoc, FRotator _ItemRelRot, FVector _ItemRelScale);
	UFUNCTION(Reliable, Server)
	void SetItemSocketVisibility(bool _Visibility);
	void SetItemSocketVisibility_Implementation(bool _Visibility);

	UFUNCTION()
	void ItemToCheckAnimation();

	// Crouch ī�޶� �̵�
	UFUNCTION()
	void CrouchCameraMove();

	UFUNCTION()
	void AttackCheck();

	UFUNCTION()
	void AttackEndCheck();

	UFUNCTION()
	void NetCheck();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsServer = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsClient = false;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsCanControlled = false;

	UPROPERTY(Category = "TPSNet", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Token = -1;

	UPROPERTY(Category = "TPSNet", Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerUpperState IdleDefault = EPlayerUpperState::UArm_Idle;
};
