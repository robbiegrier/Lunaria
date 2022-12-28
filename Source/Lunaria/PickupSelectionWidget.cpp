// Robbie Grier

#include "PickupSelectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "Pickup.h"
#include "Printer.h"
#include "LunariaGameModeBase.h"
#include "UpgradeManager.h"
#include "Choosable.h"
#include "AttributesComponent.h"
#include "User.h"
#include "TimerManager.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

UPickupSelectionWidget::UPickupSelectionWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	CurrentPickup = nullptr;
}

void UPickupSelectionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
}

void UPickupSelectionWidget::MakeSelectionFromPickup(APickup* Pickup)
{
	if (SelectionInProgress) return;
	SelectionInProgress = true;
	CurrentPickup = Pickup;

	auto User = Cast<AUser>(GetOwningPlayer());
	User->ToggleUIControl(true);

	SetVisibility(ESlateVisibility::Visible);
	ChoicesVerticalBox->ClearChildren();

	auto Color = ALunariaGameModeBase::Get(GetWorld())->GetArchetypeColor(Pickup->GetArchetype());
	SetColor(Color);

	if (auto UpgradeManager = ALunariaGameModeBase::Get(GetWorld())->GetUpgradeManager())
	{
		auto Choices = UpgradeManager->GetChoicesFromPickup(Pickup);

		for (auto ChoiceActor : Choices)
		{
			//auto ChoiceActor = GetWorld()->SpawnActor(Choice);
			TransientChoiceActors.Add(ChoiceActor);

			if (auto Choosable = Cast<IChoosable>(ChoiceActor))
			{
				auto ChoiceWidget = NewObject<UPickupChoiceWidget>(GetWorld(), ALunariaGameModeBase::Get(GetWorld())->GetPickupChoiceWidgetClass());
				ChoicesVerticalBox->AddChild(ChoiceWidget);

				ChoiceWidget->SetName(Choosable->GetChoiceName());
				ChoiceWidget->SetDescription(Choosable->GetChoiceDescription());
				ChoiceWidget->SetPadding(FMargin(20.f));
				ChoiceWidget->SetColor(Color);
				ChoiceWidget->SetChoiceActor(ChoiceActor);
				ChoiceWidget->SetSelectionWidget(this);
				ChoiceWidget->SetOwningPlayer(GetOwningPlayer());
			}
		}
	}

	if (auto First = ChoicesVerticalBox->GetChildAt(0))
	{
		auto TimerHandle = FTimerHandle();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [First, this]() {
			Cast<UPickupChoiceWidget>(First)->GetChoiceButton()->SetKeyboardFocus();
		}, 0.1f, false);
	}
}

void UPickupSelectionWidget::SignalChoiceMade(UPickupChoiceWidget* ChoiceWidget)
{
	//Print(ChoiceWidget->GetChoiceActor()->GetName());

	ChoicesVerticalBox->ClearChildren();

	for (auto TransientChoiceActor : TransientChoiceActors)
	{
		if (TransientChoiceActor != ChoiceWidget->GetChoiceActor())
		{
			TransientChoiceActor->Destroy();
		}
	}
	TransientChoiceActors.Empty();

	if (auto Choosable = Cast<IChoosable>(ChoiceWidget->GetChoiceActor()))
	{
		Choosable->Choose(GetOwningPlayerPawn());
	}

	//if (auto Boon = Cast<ABoon>(ChoiceWidget->GetChoiceActor()))
	//{
	//	if (auto Attributes = Player->FindComponentByClass<UAttributesComponent>())
	//	{
	//		Attributes->AddBoon(Boon);
	//	}
	//}

	SetVisibility(ESlateVisibility::Hidden);

	SelectionInProgress = false;

	if (auto User = Cast<AUser>(GetOwningPlayer()))
	{
		User->ToggleUIControl(false);
	}

	CurrentPickup->SignalInteractionComplete();
	CurrentPickup = nullptr;
}

void UPickupSelectionWidget::SetTitle(const FString& InTitle)
{
	TitleText->SetText(FText::FromString(InTitle));
}

void UPickupSelectionWidget::SetColor(const FLinearColor& Color)
{
	ColoredBorder->SetBrushColor(Color);
}