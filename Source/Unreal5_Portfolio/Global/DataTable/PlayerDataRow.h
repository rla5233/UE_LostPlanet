// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Global/ContentsEnum.h"
#include "PlayerDataRow.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FPlayerDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FORCEINLINE FString GetName() const
	{
		return Name;
	}

	FORCEINLINE float GetHp() const
	{
		return Hp;
	}

	FORCEINLINE float GetMoveSpeed() const
	{
		return MoveSpeed;
	}

	FORCEINLINE float GetRotSpeed() const
	{
		return RotSpeed;
	}

	FORCEINLINE float GetRunSpeed() const
	{
		return RunSpeed;
	}

	FORCEINLINE bool GetJumping() const
	{
		return Jumping;
	}

	FORCEINLINE class USkeletalMesh* GetPlayerSkeletalMesh() const
	{
		return PlayerSkeletalMesh;
	}

	FORCEINLINE class USkeletalMesh* GetPlayerFPVPlayerSkeletalMesh() const
	{
		return FPVPlayerSkeletalMesh;
	}

	FORCEINLINE TSubclassOf<class UAnimInstance> GetPlayerAnimInstance() const
	{
		return PlayerAnimInstance;
	}

	FORCEINLINE TMap<EPlayerUpperState, class UAnimMontage*> GetAnimMontages() const
	{
		return AnimMontages;
	}

protected:

private:
	/// <summary>
	/// Player �̸�
	/// </summary>
	UPROPERTY(Category = "Parameter", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FString Name = "None";

	/// <summary>
	/// ü��
	/// </summary>
	UPROPERTY(Category = "Parameter", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	float Hp = 100.0f;

	/// <summary>
	/// �̵� �ӵ�
	/// </summary>
	UPROPERTY(Category = "Parameter", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	float MoveSpeed = 100.0f;

	/// <summary>
	/// ȸ�� �ӵ�
	/// </summary>
	UPROPERTY(Category = "Parameter", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	float RotSpeed = 100.0f;

	/// <summary>
	/// �ٱ� �ӵ�
	/// </summary>
	UPROPERTY(Category = "Parameter", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	float RunSpeed = 2.0f;

	/// <summary>
	/// ���� ��
	/// </summary>
	UPROPERTY(Category = "Parameter", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	bool Jumping = false;

	/// <summary>
	/// ���õ� Player�� ����� Skeletal Mesh
	/// </summary>
	UPROPERTY(Category = "PlayerSkeletalMesh", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	class USkeletalMesh* PlayerSkeletalMesh = nullptr;

	/// <summary>
	/// ���õ� Player�� ����� 1��Ī Skeletal Mesh
	/// </summary>
	UPROPERTY(Category = "PlayerSkeletalMesh", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	class USkeletalMesh* FPVPlayerSkeletalMesh = nullptr;

	/// <summary>
	/// ���õ� Player�� ����� Anim Instance
	/// </summary>
	UPROPERTY(Category = "PlayerAnimInstance", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	TSubclassOf<class UAnimInstance> PlayerAnimInstance;

	/// <summary>
	/// Player ��Ÿ��
	/// </summary>
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	TMap<EPlayerUpperState, class UAnimMontage*> AnimMontages = TMap<EPlayerUpperState, class UAnimMontage*>();

};
