// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SActionComponent.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USAction;


UCLASS()
class ACTIONROUGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_Dash;

	

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();

	void PrimaryAttack();   //攻击函数
	void BlackHoleAttack();
	void Dash();

	void PrimaryInteract(); //交互函数

	void PrimaryAttack_TimeElapsed();  //计时器触发攻击函数  //原因：避免播放攻击动画子弹还是从动画播放前的手生成	   
	void BlackHoleAttack_TimeElapsed();
	void Dash_TimeElapsed();

	void StartAttackEffects();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn); //通道检测公用函数

	void Jumping();
	void StopJump();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	UFUNCTION(Exec)
		void HealSelf(float Amount = 100);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	FRotator GetPawnViewRotator() const;

};
