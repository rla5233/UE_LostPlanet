// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestDataRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuestDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
	/// <summary>
	/// ����Ʈ ��ġ ��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FVector QuestLocation = FVector(0.0f, 0.0f, 0.0f);

	/// <summary>
	/// ����Ʈ ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	FString QuestText = "";
};
