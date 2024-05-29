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

	// ������ҵ�OnHealthChange�¼���ʼ����
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwnningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USSActionEffects_Thorns::OnHealthChanged);
	}
}

void USSActionEffects_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// ֹͣ����
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwnningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USSActionEffects_Thorns::OnHealthChanged);
	}
}

void USSActionEffects_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwnningComponent()->GetOwner();

	// ֻ��������
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// ���������ֹ��ֵ�С������
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// ����ֵ����Ϊ����, ʹ��ApplyDamage����˺�
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// ���˸���Դ
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

