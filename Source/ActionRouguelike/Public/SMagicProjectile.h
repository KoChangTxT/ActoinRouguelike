// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class SoundBase;
class UCameraShakeBase;
class UMatineeCameraShake;
class USActionEffect;

UCLASS()
class ACTIONROUGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void Explode();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> BurningActionClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, Category = "HitEffects")
	UParticleSystem* HitEffects;

	UPROPERTY(EditAnywhere,Category = "Audio")
	USoundBase* HitPawnSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		float ImpactShakeInnerRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
		float ImpactShakeOuterRadius;

	UPROPERTY(EditDefaultsOnly,Category = "Effects|Shake")
	TSubclassOf<UMatineeCameraShake> ImpactShake;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float DamageAmount;

	UPROPERTY(EditAnywhere, Category = "Damage")
		FGameplayTag ParryTag;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
