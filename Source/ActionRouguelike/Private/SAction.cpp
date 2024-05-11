// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Logging/LogMacros.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//UE_LOG(LogTemp, Warning, TEXT("Character is in111"));

	USActionComponent* Comp = GetOwnningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwnningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	USActionComponent* Comp = GetOwnningComponent();

	UE_LOG(LogTemp, Warning, TEXT("HasAny before"));
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		UE_LOG(LogTemp, Warning, TEXT("HasAny Success"));
		return false;
	}

	return true;
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwnningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

