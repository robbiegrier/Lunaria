// Robbie Grier

#include "PickupChoiceWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Printer.h"
#include "PickupSelectionWidget.h"

UPickupChoiceWidget::UPickupChoiceWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UPickupChoiceWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
}

void UPickupChoiceWidget::SetName(const FString& InName)
{
	ChoiceName->SetText(FText::FromString(InName));
}

void UPickupChoiceWidget::SetDescription(const FString& InDesc)
{
	ChoiceDescription->SetText(FText::FromString(InDesc));
}

void UPickupChoiceWidget::SetColor(const FLinearColor& Color)
{
	ColoredBorder->SetBrushColor(Color);
}

void UPickupChoiceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto Delegate = FScriptDelegate();
	Delegate.BindUFunction(this, TEXT("HandleButtonPressed"));
	ChoiceButton->OnClicked.Add(Delegate);
}

void UPickupChoiceWidget::HandleButtonPressed()
{
	SelectionWidget->SignalChoiceMade(this);
}