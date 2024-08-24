// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicMonsterAIController.h"
#include "MainGameLevel/Player/MainPlayerState.h"
#include "MainGameLevel/Player/MainCharacter.h"
#include "MainGameLevel/Monster/Base/BTTaskNodeBase_Monster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GenericTeamAgentInterface.h"

#include "PartDevLevel/Character/ParentsCharacter.h"
#include "TestLevel/Character/TestPlayerState.h"

ABasicMonsterAIController::ABasicMonsterAIController()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));

	UAIPerceptionComponent* AIPerception = CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicMonsterAIController::PlayerDetect);
	AIPerception->SetDominantSense(*SightConfig->GetSenseImplementation());
	AIPerception->ConfigureSense(*SightConfig);

	SetPerceptionComponent(*AIPerception);
}

void ABasicMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(BehaviorTree);
}

void ABasicMonsterAIController::PlayerDetect(AActor* Other, FAIStimulus const Stimulus)
{
	if (true == Stimulus.WasSuccessfullySensed())
	{
		if (AParentsCharacter* TargetPlayer = Cast<AParentsCharacter>(Other))
		{
			if (ATestPlayerState* TargetPlayerState = Cast<ATestPlayerState>(TargetPlayer->GetPlayerState()))
			{
				UObject* PrevTarget = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
				if (0.0f < TargetPlayerState->GetPlayerHp() && nullptr == PrevTarget)
				{
					GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), TargetPlayer);
				}
			}			
		}
	}
}

ETeamAttitude::Type ABasicMonsterAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	Super::GetTeamAttitudeTowards(Other);

	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (nullptr == OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (nullptr == OtherTeamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId MyTeamId = GetGenericTeamId();
	FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
	if (MyTeamId == OtherTeamId)
	{
		return ETeamAttitude::Neutral;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}