// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"


static TAutoConsoleVariable<bool>CVarSpawnBots(TEXT("zk.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}
 

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimeHandle_SpawnBots,this, &ASGameModeBase::SpawnTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->isAlive())
		{
			AttributeComp->Kill(this); //pass in player? for credit

		}
	}
}

void ASGameModeBase::SpawnTimerElapsed()
{

	if (CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CvarspawnBots'.,in GameMode cpp"));
		return;
	}
	

	int32 NrofAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->isAlive())
		{
			NrofAliveBots++;

		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrofAliveBots);

	float MaxBotCount = 10.0f;
	if (NrofAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity.Skipping bot spawn."));
		return;

	}

	if (DifficultyCurve)
	{

		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());

	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);//这个GetOnQueryFinishedEvent返回一个代理再通过这个代理绑定事件
	}

	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Red,false,30.0f);
	}
}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	Controller->UnPossess();

	RestartPlayer(Controller);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delaegate;

		Delaegate.BindUFunction(this, "RespawnPlayerElapsed",Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delaegate, 2.0f, false);

	}
	UE_LOG(LogTemp, Log, TEXT("OnActorkilled:victim:%s,Killer:%s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

}

