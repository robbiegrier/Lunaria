// Robbie Grier

#include "AttributesComponent.h"
#include "GameFramework/Actor.h"
#include "Printer.h"
#include "CpuHealthBar.h"
#include "HealthComponent.h"
#include "AttributeObserver.h"
#include "Boon.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto Boon : Boons)
	{
		Boon->SetActorLocation(GetOwner()->GetActorLocation());
	}
}

void UAttributesComponent::NativeWhenAgentOf(const FGameplayEvent& Event)
{
	for (auto Boon : Boons)
	{
		Boon->ExecuteAgentOf(Event);
	}
}

void UAttributesComponent::NativeWhenSubjectOf(const FGameplayEvent& Event)
{
	for (auto Boon : Boons)
	{
		Boon->ExecuteSubjectOf(Event);
	}
}

void UAttributesComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	ClearBoons();
}

void UAttributesComponent::AddBoon(ABoon* NewBoon)
{
	if (NewBoon)
	{
		Boons.Add(NewBoon);
		NewBoon->NativeOnAdded(this);
	}
}

void UAttributesComponent::RemoveAndDestroyBoon(ABoon* TheBoon)
{
	Boons.Remove(TheBoon);
	TheBoon->Destroy();
}

void UAttributesComponent::ClearBoons()
{
	for (auto Boon : Boons)
	{
		if (Boon)
		{
			Boon->Destroy();
		}
	}

	Boons.Empty();
}

float UAttributesComponent::Get(const FString& Attribute, float Seed)
{
	auto Base = Seed;
	auto Multiplier = 1.f;

	for (auto Boon : Boons)
	{
		Boon->BeforeAttributeQueried(Attribute);
		auto Modifier = Boon->GetAttributeModifier(Attribute);

		Base += Modifier.Additive;
		Multiplier += Modifier.Multiplier / 100.f;
	}

	return Base * Multiplier;
}