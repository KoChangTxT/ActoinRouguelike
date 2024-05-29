// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include <SAttributeComponent.h>
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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
	
	//检查是否满血
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{

		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreaditCost) && AttributeComp->ApplyHealthChange(this, RecoverHP))
			{
				//只在治疗成功时触发
				HideAndCooldownPowerup();
			}
		}
		
		//AttributeComp->ApplyHealthChange(this,RecoverHP);
	}


}

FText ASPowerup_HealthPotion::GetInteractText_Implemetation(APawn * InstigatorPawn)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsFullHealth())
		{
			return LOCTEXT("HealthPotion_FullHealthWarning","Already at full health");
		}
	
		return FText::Format(LOCTEXT("HealthPotion_InterMessage", "Cost {0} Credits. Restores health to maxinum."), CreaditCost);
}


ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

}

#undef LOCTEXT_NAMESPACE
