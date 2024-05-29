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
struct FActionRepData //�˽ṹ����������ֹ���������������ݹ�ȥ��һ���Ⱥ�˳���µĽ����ͬ
						//����ѡ��ʹ��һ���ṹ�彫����������һ��ȫ������
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

		
		//��һ�����ܱ��ͷ�ʱ���˼��ܻ�ӵ�д�tag�����˼����ͷŽ���ʱ���˼���ӵ�е�tag�ᱻ�Ƴ�
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;

		//Action can only start if owningActor has none of these Tags applied *
		//���������κ�һ�������ΪBlockedTags�ļ���ʱ����ǰ��Ҫ�ͷŵļ��ܲ����ͷ�
		UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BlockedTags;

		UPROPERTY(ReplicatedUsing = "OnRep_RepData") 
			FActionRepData RepData;
		//bool bIsRunning; //��ֹ����ص���û��ִ�еļ��ܵ�StopAction����

		UFUNCTION()
			void OnRep_RepData();

public:

		

		void Initialize(USActionComponent* NewActionComp); //���������Ϊ�˷�ֹ�ͻ��˺ͷ���˺���GetOuter�õ��Ľ����ͬ��Ϊ�ᱻ�����ڲ��޸�

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
	
		bool IsSupportedForNetworking() const override //����Action���Ǽ̳���UObject����Actor�����ǲ��ܼ�ͨ��SetReplicates���������縴��
		{
			return true;
		}
};
