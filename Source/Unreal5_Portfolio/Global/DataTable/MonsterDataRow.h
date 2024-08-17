// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Global/ContentsEnum.h"
#include "Global/Animation/MainMultiAnimInstance.h"
#include "MonsterDataRow.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FMonsterDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	// ���� �ִ� ü��
	UPROPERTY(EditAnywhere)
	float MaxHp = 0.0f;

	// �ȱ� �ӵ�
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 300.0f;

	// �޸��� �ӵ�
	UPROPERTY(EditAnywhere)
	float RunSpeed = 600.0f;

	// ���ݷ�
	UPROPERTY(EditAnywhere)
	float AttackDamage = 0.0f;

	// ���� ����
	UPROPERTY(EditAnywhere)
	float AttackRange = 200.0f;

	// ��Ÿ��
	UPROPERTY(EditAnywhere)
	TMap<EBasicMonsterAnim, FAnimMontageGroup> AllAnimMontages;
};