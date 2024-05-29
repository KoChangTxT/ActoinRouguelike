// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROUGUELIKE_API ASPowerupActor : public AActor,public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	
	ASPowerupActor();

protected:

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
		bool bIsActive;

	UFUNCTION()
		void OnRep_IsActive();
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
		float RespawnTime;
	
	FTimerHandle TimeHandle_RespawnTimer;

	UFUNCTION()
		void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bIsNewActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;


public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implemetation(APawn* InstigatorPawn) ;

};
