// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurve;
class USSaveGame;
class UDataTable;
class USMonsterData;


//������Ϣ�ṹ��
USTRUCT(BlueprintType)
struct FMonsterInfoRow:public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, Category = "AI")
		FPrimaryAssetId MonsterId;

		//TSubclassOf<AActor> MonsterClass;
		

	//ѡ��ĳ�ֵ������ɵ����Ȩ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Weight;

	//���ɵ�����������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SpawnCost;

	//��ɱ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float KillReward;

};


/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;

	UPROPERTY()
		USSaveGame* CurrentSaveGame;

	//���п��õĵ��˽�ɫ
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UDataTable* MonsterTable;

	/*UPROPERTY(EditAnywhere, Category = "AI")
		TSubclassOf<AActor> MinionClass;*/

	UPROPERTY(EditDefaultsOnly,Category = "AI")
	UEnvQuery* SpawnBotQuery;

	FTimerHandle TimeHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UCurveFloat* DifficultyCurve; //�Ѷ����ߣ�����ʱ���в�ͬ�ĵ�������

	UFUNCTION()
	void SpawnTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
		void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);

	ASGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController*NewPlayer) override;

	UFUNCTION(Exec)
		void KillAll();

	UFUNCTION(BlueprintCallable,Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

protected:
	// ��ͼӵ�������Ķ�дȨ�ޣ��������ǾͿ���ͨ���Ѷ�������ͼ�ʲ����޸����ֵ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		UEnvQuery* PowerupSpawnQuery;

	//ʹ��EQS�ڹؿ���ʼʱ���ɵ�����ʰȡ���� 
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		TArray<TSubclassOf<AActor>> PowerupClasses;

	//ʰȡ��֮�����С������� 
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float RequiredPowerupDistance;

	//* ��Ϸ��ʼʱ���ɵ�ʰȡ�������
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 DesiredPowerupCount;

	UFUNCTION()
		void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLoaction);

	UFUNCTION()
		void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
};
