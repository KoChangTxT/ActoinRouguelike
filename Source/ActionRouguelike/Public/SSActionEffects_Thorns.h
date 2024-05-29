// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SAttributeComponent.h"
#include "SSActionEffects_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API USSActionEffects_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	USSActionEffects_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
		float ReflectFraction;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
};
