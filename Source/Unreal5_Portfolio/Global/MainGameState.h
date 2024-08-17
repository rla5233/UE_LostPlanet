// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Global/ContentsEnum.h"
#include "MainGameState.generated.h"

UENUM(BlueprintType)
enum class EQuestItem : uint8
{
	Report1		UMETA(DisplayName = "Ư�� ��ü1 ��� �� ��ӵǴ� ����"),
	Report2		UMETA(DisplayName = "â�� �ִ� ����"),
	Report3		UMETA(DisplayName = "Ư�� ��ü2 ��� ��, �����2���� ��� ����"),
	Max			UMETA(Hidden)
};


UCLASS()
class UNREAL5_PORTFOLIO_API UActorGroup : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Actors;
};

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API AMainGameState : public AGameState
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void PushActor(EnumType _Index, AActor* _Actor)
	{
		PushActor(static_cast<uint8>(_Index), _Actor);
	}

	void PushActor(uint8 _Index, AActor* _Actor);

	template<typename EnumType>
	UActorGroup* GetActorGroup(EnumType _Index)
	{
		return GetActorGroup(static_cast<uint8>(_Index));
	}

	UActorGroup* GetActorGroup(uint8 _Index);

	UFUNCTION(BlueprintCallable)
	EGameStage GetCurStage()
	{
		return CurStage;
	}

	UFUNCTION(Reliable, Server)
	void SetCurStage(EGameStage _Stage);
	void SetCurStage_Implementation(EGameStage _Stage);

	/// <summary>
	/// ���� ���� ���� ���� üũ �Լ�
	/// </summary>
	UFUNCTION(Reliable, Server)
	void GameStateCheck(AActor* _OtherActor);
	void GameStateCheck_Implementation(AActor* _OtherActor);

	void SetIsPlayCinematic(bool _Value);

	UFUNCTION()
	FORCEINLINE int GetPlayerCount()
	{
		return PlayerCount;
	}

	UFUNCTION()
	FORCEINLINE void AddPlayerCount()
	{
		++PlayerCount;
	}

	UFUNCTION()
	FORCEINLINE void SubPlayerCount()
	{
		if (0 >= PlayerCount)
		{
			PlayerCount = 0;
			return;
		}

		--PlayerCount;
	}

	UFUNCTION()
	FORCEINLINE bool GetIsStageChange() const
	{
		return IsStageChange;
	}

	UFUNCTION()
	FORCEINLINE void SetIsStageChange(bool _Value)
	{
		IsStageChange = _Value;
	}

	FORCEINLINE bool IsPlayCinematic() const
	{
		return bPlayCinematic;
	}

	void PlayBackgroundSound();

	void StopBackgroundSound();

	/// <summary>
	/// �ó׸�ƽ�� ����� �� �ִ� TriggerBox�� �����ϴ� �Լ�
	/// </summary>
	void SpawnTriggerBox(FVector _Pos, FRotator _Rot);

protected:
	AMainGameState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<uint8, UActorGroup*> AllActor;
	
	/// <summary>
	/// ���� ���� Stage ����(�ܰ�)
	/// </summary>
	UPROPERTY(Replicated)
	EGameStage CurStage = EGameStage::Init;

	/// <summary>
	/// ���� Stage�� �ٲ�������� ���� ���� ����
	/// </summary>
	UPROPERTY()
	bool IsStageChange = false;

	/// <summary>
	/// �ó׸�ƽ ��� ���ο� ���� ����
	/// </summary>
	UPROPERTY()
	bool bPlayCinematic = false;

	/// <summary>
	/// �÷��̾� ��
	/// </summary>
	UPROPERTY()
	int PlayerCount = 0;

	/// <summary>
	/// �÷��̾� �ִ� ��
	/// </summary>
	UPROPERTY()
	int MaxPlayerCount = 4;

	/// <summary>
	/// �ִ� �÷��̾� ���� �޾Ƽ� MaxPlayerCount ���� �����ߴ����� ���� ���� ����
	/// </summary>
	UPROPERTY()
	bool SetPlayerMaxNum = false;

	/// <summary>
	/// EndingTriggerBox�� ��ġ, ȸ���� ����
	/// </summary>
	FVector EndingTriggerBoxPos = FVector(-8500.0f, 87200.0f, -7930.0f);
	FRotator EndingTriggerBoxRot = FRotator(0.0f, 0.0f, 0.0f);
	FVector TriggerBoxColScale = FVector(10.0f, 10.0f, 18.0f);

	/// <summary>
	/// ������� ������Ʈ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* BackgroundSound = nullptr;
};
