// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Global/ContentsEnum.h"
#include "ItemDataRow.generated.h"


USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/// <summary>
	/// ������ Ÿ�� ��ȯ
	/// </summary>
	/// <returns></returns>
	EItemType GetItemType() const
	{
		return ItemType;
	}

	/// <summary>
	/// ������ Ÿ�� ��ȯ
	/// </summary>
	/// <returns></returns>
	EPlayerUpperState GetType() const
	{
		return Type;
	}

	/// <summary>
	/// ������ �̸� ��ȯ
	/// </summary>
	/// <returns></returns>
	FString GetName() const
	{
		return Name;
	}

	/// <summary>
	/// ������ ������ ��ȯ
	/// </summary>
	/// <returns></returns>
	int GetDamage() const
	{
		return Damage;
	}

	/// <summary>
	/// ���� ���� ��ȯ
	/// </summary>
	/// <returns></returns>
	int GetReloadNum() const
	{
		return ReloadNum;
	}

	/// <summary>
	/// ������ StaticMesh ������ ��ȯ
	/// </summary>
	/// <returns></returns>
	class UStaticMesh* GetResMesh() const
	{
		return ResMesh;
	}

	/// <summary>
	/// �������� UClass ��ȯ.
	/// </summary>
	/// <returns></returns>
	TSubclassOf<AActor> GetItemUClass() const
	{
		return ItemUClass;
	}

	/// <summary>
	/// ������ static mesh component�� relative location �� ��ȯ
	/// </summary>
	/// <returns></returns>
	FORCEINLINE FVector GetRelLoc_E() const
	{
		return RelLoc_E;
	}

	/// <summary>
	/// ������ static mesh component�� relative rotation �� ��ȯ
	/// </summary>
	/// <returns></returns>
	FORCEINLINE FRotator GetRelRot_E() const
	{
		return RelRot_E;
	}

	/// <summary>
	/// ������ static mesh component�� relative location �� ��ȯ
	/// </summary>
	/// <returns></returns>
	FORCEINLINE FVector GetRelLoc_A() const
	{
		return RelLoc_A;
	}

	/// <summary>
	/// ������ static mesh component�� relative rotation �� ��ȯ
	/// </summary>
	/// <returns></returns>
	FORCEINLINE FRotator GetRelRot_A() const
	{
		return RelRot_A;
	}

	/// <summary>
	/// ������ static mesh component�� relative scale �� ��ȯ
	/// </summary>
	/// <returns></returns>
	FORCEINLINE FVector GetRelScale() const
	{
		return RelScale;
	}

protected:

private:
	/// <summary>
	/// ������ ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	EItemType ItemType = EItemType::None;

	/// <summary>
	/// ������ ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	EPlayerUpperState Type = EPlayerUpperState::UArm_Idle;

	/// <summary>
	/// ������ �̸�
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FString Name = "";

	/// <summary>
	/// ������ ��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	int Damage = 0;

	/// <summary>
	/// ���� ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	int ReloadNum = -1;

	/// <summary>
	/// ������ StaticMesh ���ҽ� ���� ��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	class UStaticMesh* ResMesh = nullptr;

	/// <summary>
	/// �������� UClass
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	TSubclassOf<AActor> ItemUClass = TSubclassOf<AActor>();

	/// <summary>
	/// ������ static mesh component�� relative location
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FVector RelLoc_E = FVector(0.0f, 0.0f, 0.0f);

	/// <summary>
	/// ������ static mesh component�� relative rotation
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FRotator RelRot_E = FRotator(0.0f, 0.0f, 0.0f);

	/// <summary>
	/// ������ static mesh component�� relative location
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FVector RelLoc_A = FVector(0.0f, 0.0f, 0.0f);

	/// <summary>
	/// ������ static mesh component�� relative rotation
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FRotator RelRot_A = FRotator(0.0f, 0.0f, 0.0f);

	/// <summary>
	/// ������ static mesh component�� relative scale
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FVector RelScale = FVector(1.0f, 1.0f, 1.0f);
};
