// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Object/TriggerBox/TriggerBoxBase.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MainGameLevel/UI/InGame/MainGameHUD.h"
#include "Global/MainGameState.h"
#include "Global/MainGameBlueprintFunctionLibrary.h"


#include "Global/ContentsLog.h"

ATriggerBoxBase::ATriggerBoxBase()
{
	// Ʈ���� �ڽ� ������Ʈ �ʱ�ȭ
	OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxBase::OnOverlapBegin);

	// ��Ʈ��ũ ����
	//bReplicates = true;
	//bAlwaysRelevant = true;
}

void ATriggerBoxBase::BeginPlay()
{
	Super::BeginPlay();

	CenterLocation = GetActorLocation();
}

void ATriggerBoxBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATriggerBoxBase, CenterLocation);
	DOREPLIFETIME(ATriggerBoxBase, LevelSequenceAsset);
	DOREPLIFETIME(ATriggerBoxBase, IsPlayerTP);
}

void ATriggerBoxBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (false == HasAuthority())
	{
		LOG(ObjectLog, Error, "������ �ƴմϴ�.");
		return;
	}

	if (true == IsPlayerTP)
	{
		return;
	}

	IsPlayerTP = true;
	SetAllPlayersLocation(CenterLocation);

	for (FConstPlayerControllerIterator PlayerIt = GetWorld()->GetPlayerControllerIterator(); PlayerIt; ++PlayerIt)
	{
		if (APlayerController* PlayerController = PlayerIt->Get())
		{
			DisablePlayerInput(PlayerController);
			DisablePlayerHUD(PlayerController);
		}
	}

	// �ó׸�ƽ ������ ���
	PlayCinematicSequence();

	// ��׶��� ���� ��� ����
	BackgroundSoundOnOff(UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld()), false);
}

void ATriggerBoxBase::SetAllPlayersLocation_Implementation(const FVector& NewLocation)
{
	const float DistanceFromCenter = 300.0f;

	TArray<FVector> PlayerLocations;
	PlayerLocations.Add(FVector(CenterLocation.X, CenterLocation.Y - (DistanceFromCenter * 2), CenterLocation.Z));
	PlayerLocations.Add(FVector(CenterLocation.X, CenterLocation.Y - DistanceFromCenter, CenterLocation.Z));
	PlayerLocations.Add(FVector(CenterLocation.X, CenterLocation.Y + DistanceFromCenter, CenterLocation.Z));
	PlayerLocations.Add(FVector(CenterLocation.X, CenterLocation.Y + (DistanceFromCenter * 2), CenterLocation.Z));

	// �÷��̾� ��Ʈ�ѷ� ��ȸ �� ��ġ ����
	int32 PlayerIndex = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = It->Get())
		{
			if (APawn* PlayerPawn = PlayerController->GetPawn())
			{
				// �÷��̾ �ڽ� �ֺ��� ��ġ
				if (PlayerIndex < PlayerLocations.Num())
				{
					// �÷��̾� ��Ʈ�ѷ� ȸ��
					PlayerPawn->SetActorLocation(PlayerLocations[PlayerIndex]);
					PlayerController->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));

					PlayerIndex++;
				}
			}
		}
	}
}

void ATriggerBoxBase::PlayCinematicSequence_Implementation()
{
	if (!LevelSequenceAsset)
	{
		LOG(ObjectLog, Error, "�ó׸�ƽ ������ �Ǿ����� �ʽ��ϴ�.");
		return;
	}

	// �ó׸�ƽ �������� ���
	FMovieSceneSequencePlaybackSettings Settings;
	ALevelSequenceActor* OutActor;

	// LevelSequencePlayer ����
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), LevelSequenceAsset, Settings, OutActor);

	if (SequencePlayer)
	{
		SequencePlayer->Play();

		// �ó׸�ƽ ��� ���� ����	
		AMainGameState* MainGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld());
		if (nullptr != MainGameState)
		{
			MainGameState->SetIsPlayCinematic(true);
		}

		// �ó׸�ƽ �������� ������ �� ȣ��Ǵ� ��������Ʈ ���ε�
		SequencePlayer->OnFinished.AddDynamic(this, &ATriggerBoxBase::OnSequenceFinished);
	}
}

void ATriggerBoxBase::OnSequenceFinished()
{
	// ��� �÷��̾��� �Է��� �ٽ� Ȱ��ȭ �� HUD ON
	for (FConstPlayerControllerIterator PlayerIt = GetWorld()->GetPlayerControllerIterator(); PlayerIt; ++PlayerIt)
	{
		if (APlayerController* PlayerController = PlayerIt->Get())
		{
			EnablePlayerInput(PlayerController);
			EnablePlayerHUD(PlayerController);
		}
	}

	AMainGameState* MainGameState = UMainGameBlueprintFunctionLibrary::GetMainGameState(GetWorld());
	if (nullptr != MainGameState)
	{
		// �ó׸�ƽ ��� ���� ����	
		MainGameState->SetIsPlayCinematic(false);

		// ��׶��� ���� ��� ����
		BackgroundSoundOnOff(MainGameState, true);
	}

	Destroy();
}

void ATriggerBoxBase::DisablePlayerInput_Implementation(APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->GetPawn())
	{
		PlayerController->GetPawn()->DisableInput(PlayerController);
		PlayerController->DisableInput(PlayerController);
	}
}

void ATriggerBoxBase::EnablePlayerInput_Implementation(APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->GetPawn())
	{
		PlayerController->GetPawn()->EnableInput(PlayerController);
		PlayerController->EnableInput(PlayerController);
	}
}

void ATriggerBoxBase::EnablePlayerHUD_Implementation(APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->GetPawn())
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Cast<AMainGameHUD>(PlayerController->GetHUD())->AllUIOn();
	}
}

void ATriggerBoxBase::DisablePlayerHUD_Implementation(APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->GetPawn())
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Cast<AMainGameHUD>(PlayerController->GetHUD())->AllUIOff();
	}
}

void ATriggerBoxBase::BackgroundSoundOnOff_Implementation(AGameState* _CurGameState, bool _Value)
{
	AMainGameState* CurGameState = Cast<AMainGameState>(_CurGameState);

	if (nullptr == CurGameState)
	{
		return;
	}

	if (true == _Value)
	{
		CurGameState->PlayBackgroundSound();
	}
	else
	{
		CurGameState->StopBackgroundSound();
	}
}