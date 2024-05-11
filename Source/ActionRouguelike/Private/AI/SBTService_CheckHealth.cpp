// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "SAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	HP_ratio = 0.3f;

}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (MyController)
		{
			APawn* AIPawn = MyController->GetPawn();

			if (AIPawn)
			{
				USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);
				
				bool bLowHealth = AttributeComp->GetHealth() / AttributeComp->GetHealthMax() < HP_ratio;
				BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bLowHealth);
			}
		}
	}


}


