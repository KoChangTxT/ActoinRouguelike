// Fill out your copyright notice in the Description page of Project Settings.


#include "SSActionEffects_Thorns.h"
#include "SAttributeComponent.h"
#include "SAction.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"

USSActionEffects_Thorns::USSActionEffects_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void USSActionEffects_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// 绑定至玩家的OnHealthChange事件开始监听
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwnningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USSActionEffects_Thorns::OnHealthChanged);
	}
}

void USSActionEffects_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// 停止监听
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwnningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USSActionEffects_Thorns::OnHealthChanged);
	}
}

void USSActionEffects_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwnningComponent()->GetOwner();

	// 只处理受伤
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// 四舍五入防止奇怪的小数出现
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// 反伤值设置为正数, 使用ApplyDamage造成伤害
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// 反伤给来源
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

