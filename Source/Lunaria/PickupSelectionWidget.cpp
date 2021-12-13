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
	if (auto User = Cast<AUser>(GetOwningPlayer()))
	{
		User->ToggleUIControl(true);
	}

	SetVisibility(ESlateVisibility::Visible);
	ChoicesScrollBox->ClearChildren();

	auto Color = ALunariaGameModeBase::Get(GetWorld())->GetArchetypeColor(Pickup->GetArchetype());
	SetColor(Color);

	if (auto UpgradeManager = ALunariaGameModeBase::Get(GetWorld())->GetUpgradeManager())
	{
		auto Choices = UpgradeManager->GetChoicesFromPickup(Pickup);

		PrintFloat((float)Choices.Num());
		for (auto Choice : Choices)
		{
			auto ChoiceActor = GetWorld()->SpawnActor(Choice);
			TransientChoiceActors.Add(ChoiceActor);

			if (auto Choosable = Cast<IChoosable>(ChoiceActor))
			{
				auto ChoiceWidget = NewObject<UPickupChoiceWidget>(GetWorld(), ALunariaGameModeBase::Get(GetWorld())->GetPickupChoiceWidgetClass());
				ChoicesScrollBox->AddChild(ChoiceWidget);

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

	if (auto First = ChoicesScrollBox->GetAllChildren()[0])
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [First]() { First->SetFocus(); }, 0.1f, false);
	}
}

void UPickupSelectionWidget::SignalChoiceMade(UPickupChoiceWidget* ChoiceWidget)
{
	Print(ChoiceWidget->GetChoiceActor()->GetName());

	ChoicesScrollBox->ClearChildren();

	for (auto TransientChoiceActor : TransientChoiceActors)
	{
		if (TransientChoiceActor != ChoiceWidget->GetChoiceActor())
		{
			TransientChoiceActor->Destroy();
		}
	}

	auto Player = GetOwningPlayerPawn();

	if (auto Boon = Cast<ABoon>(ChoiceWidget->GetChoiceActor()))
	{
		if (auto Attributes = Player->FindComponentByClass<UAttributesComponent>())
		{
			Attributes->AddBoon(Boon);
		}
	}

	SetVisibility(ESlateVisibility::Hidden);

	if (auto User = Cast<AUser>(GetOwningPlayer()))
	{
		User->ToggleUIControl(false);
	}
}

void UPickupSelectionWidget::SetTitle(const FString& InTitle)
{
	TitleText->SetText(FText::FromString(InTitle));
}

void UPickupSelectionWidget::SetColor(const FLinearColor& Color)
{
	ColoredBorder->SetBrushColor(Color);
}