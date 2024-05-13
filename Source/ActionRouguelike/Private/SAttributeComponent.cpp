// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Logging/LogMacros.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float>CVarDamageMultiplier(TEXT("zk.DamageMultiplier"), true, TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
	HealthMax = 100.0f;

	SetIsReplicatedByDefault(true);
	// ...
}


bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -HealthMax);
}

bool USAttributeComponent::isAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) //CanBeDamaged为虚幻内置方法，通过作弊码God开启，开启后则返回false
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor Success8"));
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);
	/*if (Health < 0.0f) Health = 0.0f;
	else if (Health > HealthMax) Health = HealthMax;*/
	float ActualDelta = Health - OldHealth;
	//UE_LOG(LogTemp, Warning, TEXT("ActualDelta value is: %f"), ActualDelta);


	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}
	

	//if died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->isAlive();
	}

	return false;

}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigator,this, NewHealth, Delta);
}



void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax,COND_OwnerOnly);
}

