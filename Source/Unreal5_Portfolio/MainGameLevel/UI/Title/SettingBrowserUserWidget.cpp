// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/UI/Title/SettingBrowserUserWidget.h"
#include "MainGameLevel/UI/Title/MainTitleHUD.h"
#include "MainGameLevel/UI/Title/ServerBtnUserWidget.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/ContentsLog.h"

#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void USettingBrowserUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư Ŭ�� �� ���� �Լ� ������
	OkButton->DefaultButton->OnClicked.AddUniqueDynamic(this, &USettingBrowserUserWidget::OnOkButtonClicked);

	// ��ư �ؽ�Ʈ ����
	OkButton->SetButtonText(FText::FromString(FString("Ok")));
}

void USettingBrowserUserWidget::OnOkButtonClicked()
{
	UMainGameInstance* Inst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
	APlayerController* MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (nullptr == MyController)
	{
		LOG(UILog, Fatal, "Controller is Null");
	}

	AMainTitleHUD* TitleHUD = Cast<AMainTitleHUD>(MyController->GetHUD());
	if (nullptr == TitleHUD)
	{
		LOG(UILog, Fatal, "HUD is Null");
	}

	// ���ð����� MainGameInstance�� ���� 
	FText text = NameTextBox->GetText();
	Inst->SetMainNickName(text.ToString()); // �г��� 

	// â �ݱ� 
	TitleHUD->UIOff(EUserWidgetType::SettingBrowser);
	TitleHUD->UIOn(EUserWidgetType::ServerBtn);

	// Ÿ��Ʋ�� �����ֱ� 
	Cast<UServerBtnUserWidget>(TitleHUD->GetWidget(EUserWidgetType::ServerBtn))->SetMainNicknameUI(text);
}
