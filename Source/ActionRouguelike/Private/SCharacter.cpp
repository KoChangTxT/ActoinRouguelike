// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAction.h"
#include "SActionComponent.h"



// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AtributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	HandSocketName = "Muzzle_01";

	AttackAnimDelay = 0.2f;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
}

#pragma region Move
void ASCharacter::MoveForward(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move ForWard"));
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Right"));
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::Jumping()
{
	Jump();
}

void ASCharacter::StopJump()
{
	StopJumping();
}
#pragma endregion

#pragma region Attack;
void ASCharacter::PrimaryAttack_TimeElapsed() //计时器所要触发的函数
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleClass);
}


void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		FRotator CameraRotation = CameraComp->GetComponentRotation();
		FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraRotation);
		FVector ImpactLocation = CameraComp->GetComponentLocation() + CameraForwardVector * 5000;

		TArray<AActor*> IgnoreArray;
		IgnoreArray.Add(this);
		FHitResult HitResult;

		bool HitSuccess = UKismetSystemLibrary::LineTraceSingle(GetWorld(), CameraComp->GetComponentLocation(), ImpactLocation, TraceTypeQuery1, false, IgnoreArray,
			EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 4.0f);

		FRotator Rotator;
		if (HitSuccess)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("hit actor: %s"), *HitResult.Actor->GetName()));
			Rotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitResult.Location);
		}
		else
		{
			Rotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, ImpactLocation);
		}


		FTransform SpawnTM = FTransform(Rotator, HandLocation);

		FActorSpawnParameters SpawnParms;
		SpawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParms.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParms);
		//Muzzle_Flash
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

	}
}

#pragma endregion

#pragma region PreAttack
void ASCharacter::PrimaryAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("PrimaryAttack"));
	ActionComp->StartActionByName(this, "PrimaryAttack");
	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::BlackHoleAttack()
{
	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, AttackAnimDelay);
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ASCharacter::Dash()
{
	//StartAttackEffects();
	//GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);
	
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();//按下e键后触发接口组件
	}
}

#pragma endregion

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta)
{
	
		//MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		DisableInput(PC);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

FRotator ASCharacter::GetPawnViewRotator() const
{
	return CameraComp->GetComponentRotation();
}

void ASCharacter::HealSelf(float Amount /*= 100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#pragma region Input
// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUP", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this, &ASCharacter::Jumping);
	PlayerInputComponent->BindAction("StopJumping", IE_Released, this, &ASCharacter::StopJump); //jump为第一次作业自己写的
	PlayerInputComponent->BindAction("BlackHole", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

}



#pragma endregion
