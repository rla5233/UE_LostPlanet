// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataTable/NetDataRow.h"
#include "DataTable/WidgetDataRow.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void Init() override;

	/// <summary>
	/// PlayerDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ PlayerDataTable ���̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	const struct FPlayerDataRow* GetPlayerData(FName _Name);

	/// <summary>
	/// NetDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ NetDataTable ���̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	const struct FNetDataRow* GetNetData(FName _Name);

	/// <summary>
	/// MonsterDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ MonsterDataTable ���̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	const struct FMonsterDataRow* GetMonsterData(FName _Name);

	/// <summary>
	/// BossMonsterDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ BossMonsterDataTable ���̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	const struct FBossMonsterDataRow* GetBossMonsterData(FName _Name);

	/// <summary>
	/// ItemDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ ItemDataTable �� �̸�
	/// </summary>
	const struct FItemDataRow* GetItemData(FName _Name);

	/// <summary>
	/// InGameUserWidgetDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ InGameUserWidgetDataTable �� �̸�
	/// </summary>
	const struct FWidgetDataRow* GetInGameUserWidgetDataTable(FName _Name);

	/// <summary>
	/// TitleUserWidgetDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ TitleUserWidgetDataTable �� �̸�
	/// </summary>
	const struct FWidgetDataRow* GetTitleUserWidgetDataTable(FName _Name);

	/// <summary>
	/// LobbyUserWidgetDataTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ LobbyUserWidgetDataTable �� �̸�
	/// </summary>
	const struct FWidgetDataRow* GetLobbyUserWidgetDataTable(FName _Name);

	/// <summary>
	/// GlobalObjectTable �����͸� �޾ƿ��� �Լ�
	/// FName�� ������ GlobalObjectTable �� �̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	const TSubclassOf<UObject> GetGlobalObjectClass(FName _Name);

	/// <summary>
	/// InGameWidgets �� �޾ƿ��� �Լ�
	/// </summary>
	TMap<FString, FWidgetDataRow>& GetInGameWidgets()
	{
		return InGameWidgets;
	}

	/// <summary>
	/// TitleWidgets �� �޾ƿ��� �Լ�
	/// </summary>
	TMap<FString, FWidgetDataRow>& GetTitleWidgets()
	{
		return TitleWidgets;
	}

	/// <summary>
	/// LobbyWidgets �� �޾ƿ��� �Լ�
	/// </summary>
	TMap<FString, FWidgetDataRow>& GetLobbyWidgets()
	{
		return LobbyWidgets;
	}

	/// <summary>
	/// MapObjDataTable �����͸� ��ȯ
	/// FName�� ������ MapObjDataTable �� �̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	struct FMapObjDataRow* GetMapObjDataTable(FName _Name);

	/// <summary>
	/// QuestDataTable �����͸� ��ȯ
	/// FName�� ������ QuestDataTable �� �̸�
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	const struct FQuestDataRow* GetQuestDataTable(FName _Name);

	FNetDataRow CurNetInfo;
	FRandomStream Random;

	UFUNCTION()
	FORCEINLINE int GetNetToken()
	{
		return ++NetToken;
	}

	UFUNCTION()
	FORCEINLINE UDataTable* GetNetDataTable()
	{
		return NetDataTable;
	}

	UFUNCTION()
	FORCEINLINE FName GetUIToSelectCharacter() const
	{
		return UIToSelectCharacter;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetMainNickName() const
	{
		return MainNickname;
	}

	UFUNCTION()
	FORCEINLINE void SetUIToSelectCharacter(FName _Name)
	{
		UIToSelectCharacter = _Name;
	}

	UFUNCTION()
	FORCEINLINE void SetMainNickName(FString _Nickname)
	{
		MainNickname = _Nickname;
	}

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* PlayerDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* NetDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* MonsterDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* BossMonsterDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* ItemDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* InGameUserWidgetDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* TitleUserWidgetDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* LobbyUserWidgetDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* GlobalObjectTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* MapObjDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	UDataTable* QuestDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FWidgetDataRow> InGameWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FWidgetDataRow> TitleWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FWidgetDataRow> LobbyWidgets;
private:
	UMainGameInstance();

	UPROPERTY()
	int NetToken = -1;

	UPROPERTY()
	FName UIToSelectCharacter = "";

	UPROPERTY()
	FString MainNickname = "MyNickName";
};

