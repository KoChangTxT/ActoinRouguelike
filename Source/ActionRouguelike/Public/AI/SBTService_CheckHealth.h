// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API USBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
		float HP_ratio;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	USBTService_CheckHealth();
	
};
