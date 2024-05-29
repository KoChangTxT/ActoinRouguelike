// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

USTRUCT()
struct FActionRepData //此结构体是用来防止这两个变量被传递过去有一个先后顺序导致的结果不同
						//所以选择使用一个结构体将这两个变量一次全部传递
{
	GENERATED_BODY();

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;

};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROUGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
		float TimeStarted;
	
	UPROPERTY(Replicated)
		USActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComponent* GetOwnningComponent() const;

		
		//当一个技能被释放时，此技能会拥有此tag，当此技能释放结束时，此技能拥有的tag会被移除
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;

		//Action can only start if owningActor has none of these Tags applied *
		//当人物有任何一个被标记为BlockedTags的技能时，当前正要释放的技能不能释放
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BlockedTags;

		UPROPERTY(ReplicatedUsing = "OnRep_RepData") 
			FActionRepData RepData;
		//bool bIsRunning; //防止意外地调用没有执行的技能的StopAction函数

		UFUNCTION()
			void OnRep_RepData();

public:

		

		void Initialize(USActionComponent* NewActionComp); //这个函数是为了防止客户端和服务端函数GetOuter得到的结果不同因为会被引擎内部修改

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
	
		bool IsSupportedForNetworking() const override //由于Action类是继承自UObject而非Actor，我们不能简单通过SetReplicates打开他的网络复制
		{
			return true;
		}
};
