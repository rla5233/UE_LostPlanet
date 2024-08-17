// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "TestPlayMode.generated.h"

/**
 * 
 */
class ATestPlayerState;
UCLASS()
class UNREAL5_PORTFOLIO_API ATestPlayMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	void PostLogin(APlayerController* NewPlayer) override; // �÷��̾� �������� ����(Login) �� ȣ��Ǵ� �Լ� 

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void GPlayerToDropItem(FName _ItemName, FTransform _Transform);
	void GPlayerToDropItem_Implementation(FName _ItemName, FTransform _Transform);

protected :
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
};
	