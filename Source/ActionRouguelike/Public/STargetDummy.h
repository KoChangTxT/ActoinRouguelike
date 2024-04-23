// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAttributeComponent.h"
#include "STargetDummy.generated.h"

UCLASS()
class ACTIONROUGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASTargetDummy();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		USAttributeComponent* AttributComp;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta);
};
