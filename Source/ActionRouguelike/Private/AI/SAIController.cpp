// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree,TEXT("BehaviorTree is nullptr! Please assign BehaviorTree in your AIController")))
	{
		RunBehaviorTree(BehaviorTree);
	}


	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (MyPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}*/
}
