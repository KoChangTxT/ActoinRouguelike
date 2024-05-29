// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
		float GetTimeRemaining() const;

protected:
		//Buff持续时长
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Duration;

		// 我们的持续性效果所执行的次数，例如DOT的一“跳”伤害  Buff效果执行一次的时间
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Period;

		FTimerHandle PeriodHandle;
		FTimerHandle DurationHandle;

		UFUNCTION(BlueprintNativeEvent, Category = "Effect")
		void ExecutePeriodicEffect(AActor* Instigator);

public:
	USActionEffect();
};
