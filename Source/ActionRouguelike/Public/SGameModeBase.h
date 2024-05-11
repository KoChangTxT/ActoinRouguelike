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
	
};
