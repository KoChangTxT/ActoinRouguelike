// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

		UFUNCTION(BlueprintCallable, Category = "GameplayFunc")
		static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

		UFUNCTION(BlueprintCallable, Category = "GameplayFunc")
		static bool ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,const FHitResult& HitResult);

};
