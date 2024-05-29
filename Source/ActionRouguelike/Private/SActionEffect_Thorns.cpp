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
//	// 只处理受伤
//	if (Delta < 0.0f && OwningActor != InstigatorActor)
//	{
//		// 四舍五入防止奇怪的小数出现
//		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
//		if (ReflectedAmount == 0)
//		{
//			return;
//		}
//
//		// 反伤值设置为正数, 使用ApplyDamage造成伤害
//		ReflectedAmount = FMath::Abs(ReflectedAmount);
//
//		// 反伤给来源
//		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
//	}
//}
//
