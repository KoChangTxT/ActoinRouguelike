// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData //用来存储世界中的actor，避免人物生成一次物体就被重置一次
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName; // 区分此段数据属于哪个actor

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;

};

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
