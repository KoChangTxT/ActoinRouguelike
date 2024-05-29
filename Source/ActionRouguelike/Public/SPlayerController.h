// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

/**
 * 
 */
UCLASS()
class ACTIONROUGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
		UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
		void TogglePauseMenu();

	void SetupInputComponent() override;

	// ����������PlayerState (���ڿͻ�����˵���ڸռ�����Ϸ��ʱ��PlayerState�����ǿ�ָ��, 
	// ���֮��PlayerState�����ٱ仯��ΪPlayerController��ͬһ���ؿ��б���PlayerState��״̬)
	UPROPERTY(BlueprintAssignable)
		FOnPlayerStateChanged OnPlayerStateReceived;

	virtual void SetPawn(APawn* InPawn) override;

	/* ��PlayerController׼���ÿ�ʼ��Ϸ��ʱ�����, ��ʼ��UI֮������Ϸ�����йصĶ���ĺ�ʱ��
		(�ر����ڶ�����Ϸ�пͻ��˿���û���㹻�����Ϣ����ΪPlayerState��û���յ�) */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
		void BlueprintBeginPlayingState();

	void OnRep_PlayerState() override;

	UPROPERTY(BlueprintAssignable)
		FOnPawnChanged OnPawnCHanged;

	//virtual void SetPawn(APawn* InPawn) override;
	
};
