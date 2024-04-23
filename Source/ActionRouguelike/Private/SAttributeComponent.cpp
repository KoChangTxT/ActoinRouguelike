// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{

	Health = 100;
	// ...
}


bool USAttributeComponent::isAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax);
	/*if (Health < 0.0f) Health = 0.0f;
	else if (Health > HealthMax) Health = HealthMax;*/
	

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}



