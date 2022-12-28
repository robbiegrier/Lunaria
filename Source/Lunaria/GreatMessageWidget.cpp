// Robbie Grier

#include "GreatMessageWidget.h"
#include "Components/TextBlock.h"

UGreatMessageWidget::UGreatMessageWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UGreatMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGreatMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetRenderOpacity(GetRenderOpacity() + (InDeltaTime * 0.2f));
	}
}

void UGreatMessageWidget::SetGreatMessage(const FString& InText)
{
	GreatMessage->SetText(FText::FromString(InText));
}

void UGreatMessageWidget::StartDisplay(const FString& InText)
{
	SetGreatMessage(InText);
	SetVisibility(ESlateVisibility::Visible);
	SetRenderOpacity(0.f);
}

void UGreatMessageWidget::EndDisplay()
{
	SetVisibility(ESlateVisibility::Hidden);
}