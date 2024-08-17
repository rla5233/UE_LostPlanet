// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ContentsEnum.generated.h"


// Contents ���������� ����ϴ� Enum class
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	Player,
	Max
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Walk	UMETA(DisplayName = "Walk"),
	Run		UMETA(DisplayName = "Run"),
	Jump	UMETA(DisplayName = "Jump"),
	Crouch	UMETA(DisplayName = "Crouch"),
	Fire	UMETA(DisplayName = "Fire"),
};

/// <summary>
/// ���� Stage ���� Enum
/// </summary>
UENUM(BlueprintType)
enum class EGameStage : uint8
{
	Init,					// ���� �÷��� ���� �ʱ�
	VisitArmory,			// ����� �湮
	ObtainFirstSample,		// ����1 ������ Ȯ��
	ObtainSecondSample,		// ����2 ������ Ȯ��
	ObtainThirdSample,		// ����3 ������ Ȯ��
	PlantingBomb,			// ��ź ��ġ
	MoveToGatheringPoint,	// �������� �̵�
	Defensing,				// ���� ���
	MissionClear,			// �̼� ����
	Max
};


///// <summary>
///// 1, 2, 3, 4 ���� ���� �÷��̾� �ڼ�.
///// </summary>
//UENUM(BlueprintType)
//enum class EPlayerPosture : uint8
//{
//	Rifle1,			// �� ���� 1		// �ش� ������� �ڼ� ����
//	Rifle2,			// �� ���� 2		// �ش� ������� �ڼ� ����
//	Melee,			// �ٰŸ� ����	// �ش� ������� �ڼ� ����
//	Drink,			// ����(����)	// �κ��丮���� ����
//	Bomb,			// ��ġ�� ��ź	// �κ��丮���� ����
//	Barehand,		// �Ǽ�			// �ش� ������� �ڼ� ����
//	SlotMax
//};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Rifle,
	Melee,
	Drink,
	Bomb,
	None
};

UENUM(BlueprintType)
enum class ECameraShakeState : uint8
{
	Shoot,
	Turnback,
	Stop
};

UENUM(BlueprintType)
enum class ETestMonsterState : uint8
{
	Idle,
	Patrol,
	Chase,
	Fall,
	Jump,
	Climb,
	ClimbEnd,
	Attack,
	Dead,
	JumpAttack,
	Scream,
	Rotate,
	GroundAttack,
};

UENUM(BlueprintType)
enum class ETestMonsterType : uint8
{
	Noraml,
	Crawl,
	Boss
};

UENUM(BlueprintType)
enum class EBasicMonsterState : uint8
{
	Idle,
	Patrol,
	Scream,
	Chase,
	Fall,
	Jump,
	Climb,
	ClimbEnd,
	Attack,
	JumpAttack,
	Dead
};

UENUM(BlueprintType)
enum class ETestMonsterAnim : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walk		UMETA(DisplayName = "Walk"),
	Run			UMETA(DisplayName = "Run"),
	Attack		UMETA(DisplayName = "Attack"),
	Dead		UMETA(DisplayName = "Dead"),
	Climb		UMETA(DisplayName = "Climb"),
	ClimbEnd	UMETA(DisplayName = "ClimbEnd"),
	Scream		UMETA(DisplayName = "Scream"),
	JumpAttack	UMETA(DisplayName = "JumpAttack"),
	LRotate		UMETA(DisplayName = "LeftRotate"),
	RRotate		UMETA(DisplayName = "RightRotate"),
	LAttack		UMETA(DisplayName = "LeftAttack"),
	RAttack		UMETA(DisplayName = "RightAttack"),
};

UENUM(BlueprintType)
enum class EBasicMonsterAnim : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Scream		UMETA(DisplayName = "Scream"),
	Walk		UMETA(DisplayName = "Walk"),
	Run			UMETA(DisplayName = "Run"),
	Attack		UMETA(DisplayName = "Attack"),
	JumpAttack	UMETA(DisplayName = "JumpAttack"),
	Dead		UMETA(DisplayName = "Dead"),
	Climb		UMETA(DisplayName = "Climb"),
	ClimbEnd	UMETA(DisplayName = "ClimbEnd")
};

UENUM()
enum class EMonsterSpawnerType : uint8
{
	Once,
	Continuous,
	Max
};

UENUM(BlueprintType)
enum class EBossMonsterState : uint8
{
	Idle,
	Walk,
	Chase,
	MeleeAttack,
	RangedAttack,
	Dead
};

UENUM(BlueprintType)
enum class EBossMonsterAnim : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	WalK				UMETA(DisplayName = "Walk"),
	Run					UMETA(DisplayName = "Run"),
	MeleeAttack			UMETA(DisplayName = "MeleeAttack"),
	RangedAttack		UMETA(DisplayName = "RangedAttack"),
	Dead				UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EUserWidgetType : uint8
{
	// InGame
	HpBar,
	WeaponSlot,
	MiniMap,
	Leaderboard,
	Crosshair,
	WorldMap,
	ReloadComment,
	BossHpbar,
	E_Key,
	Num5_Key,
	Quest,
	Timer,
	SlotEmpty,
	NotAreaComment,
	AnyInteractionComment,
	AlreadyHaveComment,
	KeyHelp,

	// Title
	TitleBackground,
	TitleLogo,
	ServerBtn,
	ConnectBrowser,
	SettingBrowser,

	// Lobby
	LobbyButton,
	CantStart,
};


UENUM(BlueprintType)
enum class EPlayerLowerState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Crouch	UMETA(DisplayName = "CrouchIdle"),
};

UENUM(BlueprintType)
enum class EPlayerMoveDir : uint8
{
	Forward	UMETA(DisplayName = "Forward"),
	Back	UMETA(DisplayName = "Back"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EPlayerUpperState : uint8
{
	UArm_Idle,
	Rifle_Idle,
	Melee_Idle,
	
	UArm_Attack,
	Melee_Attack,
	Rifle_Attack,
	
	Drink,
	Bomb,

	MoveForward,
	MoveBack,
	MoveLeft,
	MoveRight,
};

UENUM(BlueprintType)
enum class EPlayerFPSTPSState : uint8
{
	FPS	UMETA(DisplayName = "1��Ī"),
	TPS	UMETA(DisplayName = "3��Ī"),
};

UCLASS()
class UNREAL5_PORTFOLIO_API UContentsEnum : public UObject
{
	GENERATED_BODY()
	
};
