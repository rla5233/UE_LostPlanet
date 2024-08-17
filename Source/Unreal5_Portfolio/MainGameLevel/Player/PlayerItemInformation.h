// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerItemInformation.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UNREAL5_PORTFOLIO_API FPlayerItemInformation
{
	GENERATED_USTRUCT_BODY()
	
public :
	FPlayerItemInformation();
	~FPlayerItemInformation();

public:
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))	// => ���� ���� �ʿ� (24.08.06 �߰���)
	bool IsItemIn = false;	// �κ��丮 ĭ�� �������� ��� �ִ��� ����

	// Item Name
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName Name = "";

	// ��ź MAX ��ġ.
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ReloadMaxNum = -1;
	// ���� ��ź ��.
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ReloadLeftNum = -1;
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Damage = 0;

	// ����ƽ �޽�
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* MeshRes = nullptr;

	// ����ƽ �޽� ������Ʈ ����� ��ġ
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector RelLoc = FVector(0.0f, 0.0f, 0.0f);
	// ����ƽ �޽� ������Ʈ ����� ȸ��
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator RelRot = FRotator(0.0f, 0.0f, 0.0f);
	// ����ƽ �޽� ������Ʈ ����� ũ��
	UPROPERTY(Category = "Contents", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector RelScale = FVector(1.0f, 1.0f, 1.0f);
};
