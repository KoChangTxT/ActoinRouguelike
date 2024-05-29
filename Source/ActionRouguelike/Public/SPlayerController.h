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

	// 监听到来的PlayerState (对于客户端来说，在刚加入游戏的时候PlayerState可能是空指针, 
	// 获得之后PlayerState不会再变化因为PlayerController在同一个关卡中保持PlayerState的状态)
	UPROPERTY(BlueprintAssignable)
		FOnPlayerStateChanged OnPlayerStateReceived;

	virtual void SetPawn(APawn* InPawn) override;

	/* 当PlayerController准备好开始游戏的时候调用, 初始化UI之类与游戏数据有关的对象的好时机
		(特别是在多人游戏中客户端可能没有足够多的信息，因为PlayerState还没有收到) */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
		void BlueprintBeginPlayingState();

	void OnRep_PlayerState() override;

	UPROPERTY(BlueprintAssignable)
		FOnPawnChanged OnPawnCHanged;

	//virtual void SetPawn(APawn* InPawn) override;
	
};
