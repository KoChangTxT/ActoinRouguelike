// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SWorldUserWidget.h"
//#include <SGameplayInterface.h>

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("zk.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);


USInteractionComponent::USInteractionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f; //检测球体半径
	TraceDistance = 500.0f; //检测距离
	CollisionChannel = ECC_WorldDynamic;
	// ...
}



void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	ASCharacter* MyCharacter = Cast<ASCharacter>(MyOwner);

	FVector EyeLocation;
	FRotator EyeRotation;

	if (ensure(MyCharacter))
	{
		EyeLocation = MyCharacter->GetPawnViewLocation();
		EyeRotation = MyCharacter->GetPawnViewRotator();
	}

	//MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance); 

	/*FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	TArray<FHitResult> Hits;

	//float Radius = 30.0f;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//清除之前的引用
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				
				FocusedActor = HitActor;
				break;  
				
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
			
		}
		
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
	
}  

#pragma region NetWorking
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);

}

#pragma endregion

