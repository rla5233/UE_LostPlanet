// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainGameState.h"
#include "MainGameInstance.h"
#include "MainGameBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API UMainGameBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UMainGameInstance* GetMainGameInstance(const UWorld* WorldContextObject);
	static AMainGameState* GetMainGameState(const UObject* WorldContextObject);

	template<typename EnumType>
	static void PushActor(EnumType _GroupIndex, AActor* _Actor)
	{
		if (nullptr == _Actor->GetWorld())
		{
			return;
		}

		PushActor(static_cast<uint8>(_GroupIndex), _Actor);
	}

	// MainGameState �� AllActor �� �߰�
	UFUNCTION(BlueprintCallable, Category = "Game", meta = ( UnsafeDuringActorConstruction = "true"))
	static void PushActor(uint8 _GroupIndex, AActor* _Actor);

	/// <summary>
	/// ������ ��Ȳ���� ���ϴ� ����� �޽����� ����ϱ� ���� �Լ�
	/// </summary>
	/// <param name="_World"></param>
	/// <param name="_Text"></param>
	UFUNCTION(BlueprintCallable)
	static void DebugTextPrint(UWorld* _World, FString _Text);
};
