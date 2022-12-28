// Robbie Grier

#include "UserHud.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Spaceship.h"
#include "HealthComponent.h"
#include "AttributesComponent.h"
#include "Printer.h"
#include "InventoryComponent.h"

UUserHud::UUserHud(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UUserHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (auto Spaceship = Cast<ASpaceship>(GetOwningPlayerPawn()))
	{
		auto HealthComponent = Spaceship->GetHealthComponent();
		auto MaxHealth = HealthComponent->GetMaxHealth();
		SetHealthValues(HealthComponent->GetCurrentHealthFast(MaxHealth), MaxHealth);

		auto AttributesComponent = Spaceship->GetAttributesComponent();
		const auto& ActiveBoons = AttributesComponent->GetCurrentBoons();
		SetBoonList(ActiveBoons);
		SetInventoryDisplay(Spaceship->GetInventoryComponent());
	}
}

void UUserHud::SetHealthValues(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Current / Max);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), static_cast<int32>(Current), static_cast<int32>(Max))));
	}
}

void UUserHud::SetBoonList(const TArray<ABoon*>& ActiveBoons)
{
	auto Text = FString();

	for (auto Boon : ActiveBoons)
	{
		Text += FString::Printf(TEXT("%s (Lvl %d)\n"), *Boon->GetBoonName(), Boon->GetBoonLevel());
	}

	BoonListText->SetText(FText::FromString(Text));
}

void UUserHud::SetInventoryDisplay(UInventoryComponent* Inventory)
{
	auto Text = FString();

	Text += FString::Printf(TEXT("Nodes: %d\n"), Inventory->GetNodesHeld());
	Text += FString::Printf(TEXT("Spice: %d\n"), Inventory->GetSpiceHeld());

	InventoryText->SetText(FText::FromString(Text));
}