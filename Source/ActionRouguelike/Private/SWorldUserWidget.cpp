// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ScreenPosition;

	

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		return;
	}

	FVector RollOffsetLocation = FVector(AttachedActor->GetActorLocation().X, AttachedActor->GetActorLocation().Y, AttachedActor->GetActorLocation().Z + WorldOffset);

	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), RollOffsetLocation, ScreenPosition);

	if (bIsOnScreen)
	{

		float scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
	
	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

}
