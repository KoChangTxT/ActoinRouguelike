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
#include "Kismet/GameplayStatics.h"
#include "SSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "SGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SMonsterData.h"
#include <../ActionRouguelike.h>
#include "SActionComponent.h"
#include "Engine/AssetManager.h"


static TAutoConsoleVariable<bool>CVarSpawnBots(TEXT("zk.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
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

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController*NewPlayer)
{
	
	ASPlayerState *PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
		
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}

	

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{

		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			//����������
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];


			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				TArray<FName> Bundles;

				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this,&ASGameModeBase::OnMonsterLoaded,SelectedRow->MonsterId,Locations[0]);
				
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}

		}

		
		//DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Red, false, 30.0f);
	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLoaction)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLoaction, FRotator::ZeroRotator);
			if (NewBot)
			{

				//LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				USActionComponent* ActionCompp = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
				if (ActionCompp)
				{
					for (TSubclassOf<USAction> ActionClass : MonsterData->Actions)
					{
						ActionCompp->AddAction(NewBot, ActionClass);
					}
				}

			}
		}
	}

	
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn PowerUp EQS Query Failed!"));
		return;
	}

	

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	

	// ��֮ǰʹ�ù���λ�ô������������Ը���ؼ��������
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// ��������Ѿ��������㹻���ʰȡ�����û�и���Ŀ���ʹ�õ����ɵ�ʱ����
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Locations.num: %d"), Locations.Num());
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

		FVector SpawnLocation = FVector(PickedLocation.X, PickedLocation.Y, PickedLocation.Z + 10.0f);

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, SpawnLocation, FRotator::ZeroRotator);

		// ��ʹ�ù��ĵ㴢���������ھ������
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void ASGameModeBase::SpawnTimerElapsed()
{

	if (CVarSpawnBots.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CvarspawnBots'.,in GameMode cpp"));
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

	//UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrofAliveBots);

	float MaxBotCount = 10.0f;
	if (NrofAliveBots >= MaxBotCount)
	{
		//UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity.Skipping bot spawn."));
		return;

	}

	if (DifficultyCurve)
	{

		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());

	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);//���GetOnQueryFinishedEvent����һ��������ͨ�����������¼�
	}

	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	
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

void ASGameModeBase::WriteSaveGame()
{
	
	//����������ҵ�״̬��Ŀǰ��ʱ��û�����ID��ƥ�䣨��ҪSteam����EOS��
	for (int32 i = 0;i < GameState->PlayerArray.Num();i++)
	{
		ASPlayerState *PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;//ע�����break��Ŀǰֻ����һ��������ԼӸ�break�����ڻ�����Һ�ID��Ӧ����ȡ����
		}
	}

	CurrentSaveGame->SavedActors.Empty(); //������һ��

	// ��������������Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// ֻ����Ӱ����Ϸ�Ե� 'gameplay actors'
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// ��Actor����Ϣ����������
		FMemoryWriter MemWriter(ActorData.ByteData);
		// ֻ�洢����� UPROPERTY(SaveGame)�ı���
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		Ar.ArIsSaveGame = true;
		// �����б���˵� UPROPERTIY �Ž����л��������
		Actor->Serialize(Ar);


		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		 CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)) ;
		 if (CurrentSaveGame == nullptr)
		 {
			 UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data"));
			 return;
		 }

		 UE_LOG(LogTemp, Warning, TEXT("load SaveGame Data Success"));
			
		 for (FActorIterator It(GetWorld()); It; ++It)
		 {
			 AActor* Actor = *It;

			 // ֻ����Ӱ����Ϸ�Ե� 'gameplay actors'
			 if (!Actor->Implements<USGameplayInterface>())
			 {
				 continue;
			 }

			 for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			 {
				 if (ActorData.ActorName == Actor->GetName())
				 {
					 Actor->SetActorTransform(ActorData.Transform);

					 FMemoryWriter MemReader(ActorData.ByteData);
					 FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					 Ar.ArIsSaveGame = true;
					 // ������������ת����Actor������
					 Actor->Serialize(Ar);

					 ISGameplayInterface::Execute_OnActorLoaded(Actor);

					 break;
				 }
			 }
		 }
	
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass())) ;
		UE_LOG(LogTemp, Warning, TEXT("Create New SaveGame Data"));
	}

	
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) //���κ���������֮ǰ����
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}


	LoadSaveGame();
}



