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
#include "SPlayerState.h"


static TAutoConsoleVariable<bool>CVarSpawnBots(TEXT("zk.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ASPlayerState::StaticClass();
}
 

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimeHandle_SpawnBots,this, &ASGameModeBase::SpawnTimerElapsed, SpawnTimerInterval, true);

	// ֻ��������������˿���ʱ���ɵ�ʰȡ����ʱ�Ż�ִ����һ���߼�
	if (ensure(PowerupClasses.Num() > 0))
	{
		// ʹ��EQS�ҵ�����ʰȡ���λ��
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->isAlive())
		{
			AttributeComp->Kill(this); 

		}
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{

}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// ��֮ǰʹ�ù���λ�ô����������������ǿ��Ը���ؼ��������
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// ��������Ѿ��������㹻���ʰȡ�����û�и���Ŀ���ʹ�õ����ɵ�ʱ����
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// ��ʣ�µĵ������ѡ��һ��
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// ��ѡ���ĵ�ɾ�������ظ�ʹ��
		Locations.RemoveAt(RandomLocationIndex);

		// �����С����
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// ����ѡ��ʹ��Debug��Ϣ��ʾ����Ϊ�������ɸѡ���ĵ�
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// ��֮�����̫������ѡ������
				bValidLocation = false;
				break;
			}
		}

		// û��ͨ���������
		if (!bValidLocation)
		{
			continue;
		}

		// �����ѡһ��ʰȡ����
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// ��ʹ�ù��ĵ㴢���������ھ������
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);//���GetOnQueryFinishedEvent����һ��������ͨ�����������¼�
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

	//��û�ɱ����
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}

}

