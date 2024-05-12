// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction>ActionClass:DefaultActions)
	{
		AddAction(GetOwner(),ActionClass);
	}

	/*for (USAction* Action : Actions)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action name: %s"),*(Action->ActionName).ToString());


		return;
	}*/
	// ...
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// ...
}

void USActionComponent::AddAction(AActor* Instigator,TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if (ensure(NewAction))
	{
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
		//UE_LOG(LogTemp, Warning, TEXT("1111111Name: %s"), *ActionName.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("333333"));
		if (Action && Action->ActionName == ActionName)
		{
			UE_LOG(LogTemp, Warning, TEXT("222222222"));
			//UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *ActionName.ToString());
			
			if (!Action->CanStart(Instigator)) 
			{
				FString FailedMsg = FString :: Printf(TEXT("Failed to run:%s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;//这个continue是不同的技能但是有相同的名字，其中一个可以释放的情况
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
				Action->StopAction(Instigator);
				return true;
			}
		}
		
		
	}

	return false;
}

