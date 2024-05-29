// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData //�����洢�����е�actor��������������һ������ͱ�����һ��
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName; // ���ִ˶����������ĸ�actor

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
