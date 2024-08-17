// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestTeamHpBarUserWidget.generated.h"

/**
 * ���� �� ���� Ŭ����
 */
class UProgressBar;
class UTextBlock;
UCLASS()
class UNREAL5_PORTFOLIO_API UTestTeamHpBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// ���� ü�� ���α׷�����
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_P2HpBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_P3HpBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_P4HpBar;

	// ���� �г���
	UPROPERTY(meta = (BindWidget))
	UTextBlock* P2_Name;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* P3_Name;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* P4_Name;

	TArray<UProgressBar*> HpArray;
	TArray<UTextBlock*> NameArray;
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void HPUpdate();

private:

	float CurHp = 0.f;
	float MaxHp = 100.f;
};
