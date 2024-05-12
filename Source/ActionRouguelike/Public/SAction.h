// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROUGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	

	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComponent* GetOwnningComponent() const;

	// Tags added to owning actor when activated, removed when action stops *
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;

	//*Action can only start if owningActor has none of these Tags applied *
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BlockedTags;

		bool bIsRunning; //防止意外地调用没有执行的技能的StopAction函数

public:

		//要通过技能来执行Buff效果的话，我们就需要让这个“技能”在授予的时候自动执行，并且在结束的时候自动移除。而自动执行和移除的功能我们还没有完成：
		// 当我们将这个技能添加到ActionComponent中时自动执行 

		UPROPERTY(EditDefaultsOnly, Category = "Action")
		bool bAutoStart;

		UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning() const;

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator); //我们不希望在上一次攻击动画仍在播放并且没有触发投射物发射的时候马上执行第二次攻击

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

		UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

		//Action nickname to start/stop without a reference to the object
		UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

		//UWorld* GetWorld() const override;

		UWorld* GetWorld() const override; //此函数是为了能此类的派生类的GetWorld方法获得World指针，否则无法在子类蓝图中获得World指针，P65
	
};
