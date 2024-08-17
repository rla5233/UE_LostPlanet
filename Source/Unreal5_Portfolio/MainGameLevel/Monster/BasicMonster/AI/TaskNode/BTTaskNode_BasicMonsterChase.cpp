// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_BasicMonsterChase.h"
#include "MainGameLevel/Monster/BasicMonster/AI/BasicMonsterAIController.h"
#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"
#include "MainGameLevel/Monster/Base/BasicMonsterData.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "Global/MainGameBlueprintFunctionLibrary.h"
#include "Global/ContentsLog.h"

EBTNodeResult::Type UBTTaskNode_BasicMonsterChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABasicMonsterBase* Monster = GetSelfActor<ABasicMonsterBase>(OwnerComp);
	if (false == Monster->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, TEXT("Monster Is Not Valid"));
		return EBTNodeResult::Type::Aborted;
	}

	UBasicMonsterData* MonsterData = Monster->GetSettingData();
	if (false == MonsterData->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, TEXT("MonsterData Is Not Valid"));
		return EBTNodeResult::Type::Aborted;
	}

	Monster->GetCharacterMovement()->MaxWalkSpeed = MonsterData->BaseData->RunSpeed;
	Monster->ChangeRandomAnimation(EBasicMonsterAnim::Run);

	return EBTNodeResult::Type::InProgress;
}

void UBTTaskNode_BasicMonsterChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* pNodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, pNodeMemory, DeltaSeconds);

	// ���� ��ȭ�� Failed
	if (EBasicMonsterState::Chase != GetCurState<EBasicMonsterState>(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ABasicMonsterBase* Monster = GetSelfActor<ABasicMonsterBase>(OwnerComp);
	FVector MonsterLocation = Monster->GetActorLocation();

	AActor* TargetActor = GetValueAsObject<AActor>(OwnerComp, TEXT("TargetActor"));
	FVector TargetLocation = TargetActor->GetActorLocation();

	// �ó׸�ƽ üũ
	AMainGameState* MainGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld());
	if (nullptr != MainGameState)
	{
		if (true == MainGameState->IsPlayCinematic())
		{
			Monster->GetAIController()->StopMovement();
			return;
		}
		else
		{
			// �̵�
			Monster->GetAIController()->MoveToLocation(TargetLocation);
		}
	}

	// ���� ���� �ȿ� ������ Attack
	UBasicMonsterData* MonsterData = Monster->GetSettingData();
	FVector LocationDiff = TargetLocation - MonsterLocation;
	float DiffLength = LocationDiff.Size();
	if (DiffLength <= MonsterData->BaseData->AttackRange)
	{
		StateChange(OwnerComp, EBasicMonsterState::Attack);
		return;
	}
}
