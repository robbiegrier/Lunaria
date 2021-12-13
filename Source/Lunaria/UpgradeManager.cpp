// Robbie Grier

#include "UpgradeManager.h"
#include "Pickup.h"
#include "Engine/World.h"
#include "User.h"
#include "AttributesComponent.h"

AUpgradeManager::AUpgradeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUpgradeManager::BeginPlay()
{
	Super::BeginPlay();
}

void AUpgradeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<UClass*> AUpgradeManager::GetChoicesFromPickup(APickup* Pickup)
{
	auto Output = TArray<UClass*>();

	auto Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	auto NumberOfChoices = 3;
	if (auto Attributes = Player->FindComponentByClass<UAttributesComponent>())
	{
		NumberOfChoices = Attributes->Get("NumberOfChoices.PickupSelection", NumberOfChoices);
	}

	for (auto i = 0; i < NumberOfChoices; i++)
	{
		if (i < PotentialChoices.Num())
		{
			Output.Add(PotentialChoices[i]);
		}
	}

	return Output;
}