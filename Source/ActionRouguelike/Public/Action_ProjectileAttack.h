// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SCharacter.h"
#include "Action_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;


/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API UAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* CastingEffect;

	UFUNCTION()
		void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UAction_ProjectileAttack();
};
