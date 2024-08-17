// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EndMontage.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API UAnimNotify_EndMontage : public UAnimNotify
{
	GENERATED_BODY()
	
	UAnimNotify_EndMontage();

	// bShouldFireInEditor

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
