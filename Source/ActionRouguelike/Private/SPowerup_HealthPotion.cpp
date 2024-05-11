// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include <SAttributeComponent.h>

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	//UE_LOG(LogTemp, Log, TEXT("InteractSuccessfouth"));
	if (!ensure(InstigatorPawn))
	{
		//UE_LOG(LogTemp, Log, TEXT("Pawn false"));
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("nextsuccess"));
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	
	
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		//UE_LOG(LogTemp, Log, TEXT("HideAndCooldownPowerup"));
		HideAndCooldownPowerup();
		AttributeComp->ApplyHealthChange(this,RecoverHP);
	}


}

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

}
