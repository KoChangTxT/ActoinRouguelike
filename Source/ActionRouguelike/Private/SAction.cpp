// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Logging/LogMacros.h"
#include "SActionComponent.h"
#include "../ActionRouguelike.h"
#include "Net/UnrealNetwork.h"



void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//UE_LOG(LogTemp, Warning, TEXT("Character is in111"));
	//LogOnScreen(this, FString::Printf(TEXT("Started:%s"), *ActionName.ToString()), FColor :: Green);

	USActionComponent* Comp = GetOwnningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwnningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	

	GetOwnningComponent()->OnActionStarted.Broadcast(GetOwnningComponent(),this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{ 
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped:%s"), *ActionName.ToString()),FColor::White);

	//ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwnningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwnningComponent()->OnActionStopped.Broadcast(GetOwnningComponent(), this);
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) //这部分是为了防止同一个技能还没释放完就进行下一次释放比如普通攻击
	{
		return false;
	}
	USActionComponent* Comp = GetOwnningComponent();

	//UE_LOG(LogTemp, Warning, TEXT("HasAny before"));
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags)) //这部分是为了防止在进行其他相互之间不能同时释放的技能时释放当前的技能，比如Sprint时不能primary Attack
	{
		//UE_LOG(LogTemp, Warning, TEXT("HasAny Success"));
		return false;
	}

	return true;
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwnningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(USActionComponent::StaticClass());
	
	//return Cast<USActionComponent>(GetOuter());

	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
	DOREPLIFETIME(USAction, TimeStarted);
}





