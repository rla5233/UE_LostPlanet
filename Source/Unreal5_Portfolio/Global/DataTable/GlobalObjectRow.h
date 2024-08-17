// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GlobalObjectRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGlobalObjectRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FORCEINLINE TSubclassOf<UObject> GetObject() const
	{
		return Object;
	}

private:
	/// <summary>
	/// UObject Ŭ������ ������ ������Ʈ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	TSubclassOf<UObject> Object = TSubclassOf<UObject>();
};
