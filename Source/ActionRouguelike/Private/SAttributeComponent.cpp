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


float USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) //CanBeDamaged为虚幻内置方法，通过作弊码God开启，开启后则返回false
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor Success8"));
		return false;
	}

	/*if (!GetOwner()->HasAuthority())
	{
		return false;
	}*///这段代码会返回false然后无法触发粒子碰撞之后爆炸效果

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;

	//是否是服务器
	if (GetOwner()->HasAuthority())
	{
		Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
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
	}

	
	/*if (Health < 0.0f) Health = 0.0f;
	else if (Health > HealthMax) Health = HealthMax;*/
	
	//UE_LOG(LogTemp, Warning, TEXT("ActualDelta value is: %f"), ActualDelta);


	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	

	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	UE_LOG(LogTemp, Warning, TEXT("ActualDelta = %f"), ActualDelta);
	if (ActualDelta != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActualDelta"));
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
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

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}


void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);

	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax,COND_OwnerOnly);
}

