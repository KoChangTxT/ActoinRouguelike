// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AtributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParams = "TimeToHit";

}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());

	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", NewTarget);

		//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);//OnSeePawn.AddDynamic的OnSeePawn是一个代理，具体可查看PawnSensingComponent.h文件

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta)
{
	

	if (Delta < 0)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}//若弹丸碰撞再大一些则会与自己相碰撞，所以设置这段

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();

			}

		}
		

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParams, GetWorld()->TimeSeconds);

		//Died
		if (NewHealth <= 0)
		{

			//stop BT
			AAIController* AIC = Cast<AAIController>(GetController());

			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			//regdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//Collision
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			//set lifespawn  the AI's time to be destroyed
			SetLifeSpan(10.0f);

		}
	}
}

