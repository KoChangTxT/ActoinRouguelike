// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../ActionRouguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;


	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	//仅服务器
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction>ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
	
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//停止所有的技能
	TArray<USAction*> ActionCopy = Actions;
	for (USAction* Action:ActionCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);


}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Draw All Actions
	//for (USAction*Action : Actions)
	//{
	//	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	//	FString ActionMsg = FString::Printf(TEXT("[%s]Action: %s IsRunning: %s"),
	//		*GetNameSafe(GetOwner()),
	//		*GetNameSafe(Action),
	//		Action->IsRunning() ? TEXT("true") : TEXT("false"));


	//	//LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	//	
	//}
		
}

void USActionComponent::AddAction(AActor* Instigator,TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	//跳过客户端
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction: [Class:%s]"),*GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}

}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action: Actions)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("333333"));
		if (Action && Action->ActionName == ActionName)
		{
			
			//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *ActionName.ToString());
			
			if (!Action->CanStart(Instigator)) 
			{
				FString FailedMsg = FString :: Printf(TEXT("Failed to run:%s"), *ActionName.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;//这个continue是不同的技能但是有相同的名字，其中一个可以释放的情况
			}

			//是否是客户端
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);

			return true;
		}
		
		
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{

				//是否是客户端
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				Action->StopAction(Instigator);
				return true;
			}
		}
		
		
	}

	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const //拾取技能函数
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
} 

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}





