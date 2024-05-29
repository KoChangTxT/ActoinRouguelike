// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwingComp, float, NewHealth, float, Delta);

//这里可以用两个委托一个是原先的FOnHealthChanged，一个是新怒气值的的...，但是这两个的代码可以复用，所以可以直接声明成一个多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);


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

	//用怒气值来驱动特定的技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated, Category = "Attributes")
		float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
		float RageMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated, Category = "Attributes")
		float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
		float HealthMax;

	//HealthMax,Stamina,Strength


	UFUNCTION(NetMulticast,Reliable)
	void MulticastHealthChanged(AActor* Instigator, float NewHealth, float Delta);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);


public:	

	UFUNCTION(BlueprintCallable)
		bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
		bool isAlive() const;

	UFUNCTION(BlueprintCallable)
		float GetHealth() const;

	UFUNCTION(BlueprintCallable)
		float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
		float GetRage() const;

	UPROPERTY(BlueprintAssignable,Category = "Attributes")
		FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
		FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable,Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool ApplyRage(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool IsFullHealth() const;

		
};
