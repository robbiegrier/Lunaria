// Robbie Grier

#include "UpgradeManager.h"
#include "Pickup.h"
#include "Engine/World.h"
#include "User.h"
#include "AttributesComponent.h"
#include "LevelUpChoice.h"
#include "Printer.h"
#include "Boon.h"

AUpgradeManager::AUpgradeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentRoomArchetype = EArchetype::Alpha;
}

TArray<UClass*> AUpgradeManager::GetChoiceClassesFromPickup(APickup* Pickup, int32 Num, UAttributesComponent* Attributes)
{
	auto Output = TArray<UClass*>();

	auto ViableNodes = UpgradeNodes.FilterByPredicate([Pickup, Attributes](const auto& Node) {
		return Node.Archetype == Pickup->GetArchetype() && Attributes->IsNodeViable(Node);
		});

	for (auto i = 0; i < Num; i++)
	{
		if (ViableNodes.Num() > 0)
		{
			auto Rand = FMath::RandRange(0, ViableNodes.Num() - 1);
			Output.Add(ViableNodes[Rand].BoonClass);
			ViableNodes.RemoveAt(Rand);
		}
	}

	return Output;
}

void AUpgradeManager::BeginPlay()
{
	Super::BeginPlay();
}

void AUpgradeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<AActor*> AUpgradeManager::GetChoicesFromPickup(APickup* Pickup)
{
	auto Output = TArray<AActor*>();

	auto Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto Attributes = Player->FindComponentByClass<UAttributesComponent>();

	auto NumberOfChoices = 3;
	//NumberOfChoices = Attributes->Get("NumberOfChoices.PickupSelection", NumberOfChoices);

	auto ChoiceClasses = GetChoiceClassesFromPickup(Pickup, NumberOfChoices - 1, Attributes);

	for (auto Class : ChoiceClasses)
	{
		auto ChoiceActor = GetWorld()->SpawnActor(Class);
		Output.Add(ChoiceActor);
	}

	if (Attributes->GetCurrentBoons().Num() > 0)
	{
		auto PotentialLevelUps = Attributes->GetCurrentBoons().FilterByPredicate([](auto Element) {
			return !Element->GetLevelUpDescription().IsEmpty();
			});

		if (PotentialLevelUps.Num() > 0)
		{
			auto RandomIndex = FMath::RandRange(0, PotentialLevelUps.Num() - 1);
			//Print(FString::Printf(TEXT("%d, pick %d"), PotentialLevelUps.Num(), RandomIndex));
			auto Boon = PotentialLevelUps[RandomIndex];

			auto ChoiceActor = GetWorld()->SpawnActor<ALevelUpChoice>();
			ChoiceActor->SetBoon(Boon);

			Output.Add(ChoiceActor);
		}
	}

	return Output;
}