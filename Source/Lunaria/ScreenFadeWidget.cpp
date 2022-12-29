// Robbie Grier

#include "ScreenFadeWidget.h"
#include "TimerManager.h"

UScreenFadeWidget::UScreenFadeWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UScreenFadeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);
}

void UScreenFadeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetRenderOpacity(FMath::Clamp(GetRenderOpacity() + (InDeltaTime * OpacityDirection), 0.f, 1.f));
	}
}

void UScreenFadeWidget::Fade()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
	SetVisibility(ESlateVisibility::Visible);
	OpacityDirection = 1.f;
}

void UScreenFadeWidget::Unfade()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
	SetVisibility(ESlateVisibility::Visible);
	OpacityDirection = -1.f;
	ScheduleHide();
}

void UScreenFadeWidget::ScheduleHide()
{
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]() {
		SetVisibility(ESlateVisibility::Hidden);
	}, 1.f, false);
}