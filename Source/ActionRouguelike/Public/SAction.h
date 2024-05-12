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

		bool bIsRunning; //��ֹ����ص���û��ִ�еļ��ܵ�StopAction����

public:

		//Ҫͨ��������ִ��BuffЧ���Ļ������Ǿ���Ҫ����������ܡ��������ʱ���Զ�ִ�У������ڽ�����ʱ���Զ��Ƴ������Զ�ִ�к��Ƴ��Ĺ������ǻ�û����ɣ�
		// �����ǽ����������ӵ�ActionComponent��ʱ�Զ�ִ�� 

		UPROPERTY(EditDefaultsOnly, Category = "Action")
		bool bAutoStart;

		UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning() const;

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* Instigator); //���ǲ�ϣ������һ�ι����������ڲ��Ų���û�д���Ͷ���﷢���ʱ������ִ�еڶ��ι���

		UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* Instigator);

		UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* Instigator);

		//Action nickname to start/stop without a reference to the object
		UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

		//UWorld* GetWorld() const override;

		UWorld* GetWorld() const override; //�˺�����Ϊ���ܴ�����������GetWorld�������Worldָ�룬�����޷���������ͼ�л��Worldָ�룬P65
	
};
