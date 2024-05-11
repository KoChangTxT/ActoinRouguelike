// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "SAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
		if (ensure(AttributeComp))
		{
			AttributeComp->ApplyHealthChange(MyPawn, RecoverAmount);

			
		}

		
	}
	return EBTNodeResult::Succeeded;
}
