// Robbie Grier

#include "PickupSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "Pickup.h"
#include "Printer.h"
#include "LunariaGameModeBase.h"

UPickupSelectionWidget::UPickupSelectionWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UPickupSelectionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
}

void UPickupSelectionWidget::MakeSelectionFromPickup(APickup* Pickup)
{
	SetVisibility(ESlateVisibility::Visible);
	ChoicesScrollBox->ClearChildren();

	SetTitle("Choose a ship upgrade...");
	SetColor(ALunariaGameModeBase::Get(GetWorld())->GetArchetypeColor(Pickup->GetArchetype()));
}

void UPickupSelectionWidget::SetTitle(const FString& InTitle)
{
	TitleText->SetText(FText::FromString(InTitle));
}

void UPickupSelectionWidget::SetColor(const FLinearColor& Color)
{
	ColoredBorder->SetBrushColor(Color);
}