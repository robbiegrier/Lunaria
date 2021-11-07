// Robbie Grier

#include "ButtonTooltip.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Printer.h"
#include "Interactable.h"

UButtonTooltip::UButtonTooltip(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UButtonTooltip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UButtonTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UButtonTooltip::SetText(const FString& InText)
{
	if (ButtonTooltipText)
	{
		ButtonTooltipText->SetText(FText::FromString(InText));
	}
}

void UButtonTooltip::LoadInteraction(IInteractable* Interactable)
{
	SetText(Interactable->GetInteractionText());

	ButtonTooltipImage->SetVisibility(Interactable->ShowButtonWidget() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}