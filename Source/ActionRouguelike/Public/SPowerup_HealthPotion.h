// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "HealthPotion")
		int32 CreaditCost;
	UPROPERTY(EditAnywhere, Category = "HealthPotion")
		float RecoverHP;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implemetation(APawn* InstigatorPawn) override;
	
	//virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_HealthPotion();
};
