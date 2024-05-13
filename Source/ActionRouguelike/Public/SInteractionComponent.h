// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SWorldUserWidget.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PrimaryInteract();

public:	
	
	USInteractionComponent();

protected:

	UFUNCTION(Server, Reliable)
		void ServerInteract(AActor* InFocus);

	UPROPERTY()
		AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
		USWorldUserWidget* DefaultWidgetInstance;

	void FindBestInteractable();
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
