// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROUGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComponent* GetOwnningComponent() const;

	// Tags added to owning actor when activated, removed when action stops *
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;

	//*Action can only start if owningActor has none of these Tags applied *
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BlockedTags;

		bool bIsRunning;

public:

		UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning() const;

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator);

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

		//Action nickname to start/stop without a reference to the object
		UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

		//UWorld* GetWorld() const override;

		UWorld* GetWorld() const override; //此函数是为了能此类的派生类的GetWorld方法获得World指针，否则无法在子类蓝图中获得World指针，P65
	
};
