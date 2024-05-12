// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

void ASPlayerState::AddCredits(int32 Delta)
{
	// 防止用户在操作时输入了负值或0
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}
bool ASPlayerState::RemoveCredits(int32 Delta)
{
	// 防止用户在操作时输入了负值或0
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// 分数不足
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}
int32 ASPlayerState::GetCredits() const
{
	return Credits;
}