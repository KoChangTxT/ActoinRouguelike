// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);

	if (AttributeComp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor Success7"));
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OtherActor Success4"));
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			//UE_LOG(LogTemp, Warning, TEXT("OtherActor Success5"));

			//Direction = Target - Orign;
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 3000.0f, HitResult.ImpactPoint, HitResult.BoneName);

		}
		return true;

	}
	//UE_LOG(LogTemp, Warning, TEXT("OtherActor Failed2222"));
	return false;
}
