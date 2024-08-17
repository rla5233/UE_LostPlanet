// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefaultHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API UDefaultHpBarUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime) override;


	// ���� ������� 
	UPROPERTY(meta = (BindWidget))
	class UImage* HpCaseImage;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HpProgress;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NickNameBlock;

	void SetHp(float _percent);
	void SetNickName(FText _nickname);
};
