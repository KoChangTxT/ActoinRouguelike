// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurve;

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
		TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly,Category = "AI")
	UEnvQuery* SpawnBotQuery;

	FTimerHandle TimeHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UCurveFloat* DifficultyCurve; //难度曲线，根据时间有不同的敌人上限

	UFUNCTION()
	void SpawnTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
		void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);

	ASGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
		void KillAll();

protected:
	// 蓝图拥有完整的读写权限，这样我们就可以通过难度曲线蓝图资产来修改这个值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		UEnvQuery* PowerupSpawnQuery;

	//使用EQS在关卡开始时生成的所有拾取物类 
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		TArray<TSubclassOf<AActor>> PowerupClasses;

	//拾取物之间的最小间隔距离 
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float RequiredPowerupDistance;

	//* 游戏开始时生成的拾取物的数量
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 DesiredPowerupCount;

	UFUNCTION()
		void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
		void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
};
