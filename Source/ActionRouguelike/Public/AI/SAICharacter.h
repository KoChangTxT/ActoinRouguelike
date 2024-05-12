// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAttributeComponent.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class UUserWidget;
class USWorldUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROUGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName TimeToHitParams;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USActionComponent* ActionComp;

	UFUNCTION()
		void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta);

};
