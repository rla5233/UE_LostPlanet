// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "LobbyCharacter.generated.h"

UCLASS()
class UNREAL5_PORTFOLIO_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALobbyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Reliable, Server)
	void ClientReady();
	void ClientReady_Implementation();

	UFUNCTION(Reliable, Server)
	void ClientChangedMesh(FName _NewType); // Ŭ�󿡼� �ڽ��� Ÿ���� �����ֱ� ���� �Ķ����
	void ClientChangedMesh_Implementation(FName _NewType);

	UFUNCTION(Reliable, Server)
	void SendNicknames(const FText& _Nickname);
	void SendNicknames_Implementation(const FText& _Nickname);

	UPROPERTY()
	bool ReadyClicked = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int MyOrder = -1;		// ���� ���� ���� �� ��ȣ (0 ~ 3)

	UPROPERTY(Replicated, BlueprintReadOnly)
	FName MyChracterType;	// ĳ���� �޽� Ÿ��

	UFUNCTION()
	FText GetMyNickName() const
	{
		return MyNickName;
	}
private:
	UPROPERTY()
	AGameModeBase* IsServerPtr = nullptr; // nullptr�̸� client

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ALobbyCapCharacter* MyMannequin = nullptr;

	UPROPERTY(Replicated, meta = (AllowPrivateAccess = "true"))
	FText MyNickName;		// �г���
};
