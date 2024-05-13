// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwingComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes",meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated, Category = "Attributes")
		float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
		float HealthMax;

	//HealthMax,Stamina,Strength


	UFUNCTION(NetMulticast,Reliable)
	void MulticastHealthChanged(AActor* Instigator, float NewHealth, float Delta);


public:	

	UFUNCTION(BlueprintCallable)
		bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
		bool isAlive() const;

	UFUNCTION(BlueprintCallable)
		float GetHealth() const;

	UFUNCTION(BlueprintCallable)
		float GetHealthMax() const;

	UPROPERTY(BlueprintAssignable)
		FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool IsFullHealth() const;

		
};
