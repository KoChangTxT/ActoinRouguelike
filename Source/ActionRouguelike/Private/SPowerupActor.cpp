// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
	SetReplicates(true);

}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	//在跟组件上设置可视性以及将子类跟随跟组件设置为true
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimeHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bIsNewActive)
{
	SetActorEnableCollision(bIsNewActive);

	//set visibility on root and all children
	RootComponent->SetVisibility(bIsNewActive,true);

}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//logic in derived classes;
}

FText ASPowerupActor::GetInteractText_Implemetation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}


