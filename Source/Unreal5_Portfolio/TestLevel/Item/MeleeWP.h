// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestLevel/Item/WeaponBase.h"
#include "MeleeWP.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API AMeleeWP : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMeleeWP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ���Ÿ� ���� ȹ��
	virtual void HandlePickUp(class ATestCharacter* PickUpCharacter) override;

	// ���Ÿ� ���� ������
	virtual void HandlePutDown(class ATestCharacter* PutDownCharacter) override;
};
