// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public :
	AMainPlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void InitPlayerData();

	UFUNCTION()
	float GetPlayerHp() const;

	UFUNCTION(Reliable, Server)
	void AddDamage(float _Damage);
	void AddDamage_Implementation(float _Damage);

private:
	// PlayerHp ���� ����Ǹ� UpdatePlayerHp �Լ� ����.
	UPROPERTY(Replicated)
	float PlayerHp = 0.0f;
};
