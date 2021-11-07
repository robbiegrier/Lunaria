// Robbie Grier

#include "DebugWidget.h"
#include "Components/TextBlock.h"

UDebugWidget::UDebugWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDebugWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDebugWidget::SetText(const FString& InText)
{
	DebugText->SetText(FText::FromString(InText));
}

void UDebugWidget::SetSubText(const FString& InText)
{
	DebugSubText->SetText(FText::FromString(InText));
}