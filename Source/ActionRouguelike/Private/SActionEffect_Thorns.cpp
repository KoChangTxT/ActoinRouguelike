//
//#include "SActionEffect_Thorns.h"
//#include "SAttributeComponent.h"
//#include "SActionComponent.h"
//#include "SGameplayFunctionLibrary.h"
//
//
//USActionEffect_Thorns::USActionEffect_Thorns()
//{
//
//}
//
//
//void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
//{
//	AActor* OwningActor = GetOwningComponent()->GetOwner();
//
//	// ֻ��������
//	if (Delta < 0.0f && OwningActor != InstigatorActor)
//	{
//		// ���������ֹ��ֵ�С������
//		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
//		if (ReflectedAmount == 0)
//		{
//			return;
//		}
//
//		// ����ֵ����Ϊ����, ʹ��ApplyDamage����˺�
//		ReflectedAmount = FMath::Abs(ReflectedAmount);
//
//		// ���˸���Դ
//		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
//	}
//}
//
