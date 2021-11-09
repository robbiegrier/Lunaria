// Robbie Grier

#include "AbilitiesComponent.h"
#include "Engine/World.h"
#include "Ability.h"
#include "Helpers.h"
#include "Printer.h"

UAbilitiesComponent::UAbilitiesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto Pair : AbilityDefaults)
	{
		SetAbility(Pair.Key, GetOwner()->GetWorld()->SpawnActor<AAbility>(Pair.Value));
	}
}

void UAbilitiesComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	for (auto Pair : Abilities)
	{
		if (auto Ability = Pair.Value)
		{
			Ability->Destroy();
		}
	}
}

void UAbilitiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto Pair : Abilities)
	{
		if (auto Ability = Pair.Value)
		{
			Ability->SetActorTransform(GetOwner()->GetActorTransform());
		}
	}
}

AAbility* UAbilitiesComponent::GetAbility(EAbilityKey Key) const
{
	if (auto Find = Abilities.Find(Key))
	{
		return *Find;
	}

	return nullptr;
}

void UAbilitiesComponent::PressAbility(EAbilityKey Key)
{
	if (auto Ability = GetAbility(Key))
	{
		Ability->Press();
	}
}

void UAbilitiesComponent::ReleaseAbility(EAbilityKey Key)
{
	if (auto Ability = GetAbility(Key))
	{
		Ability->Release();
	}
}

void UAbilitiesComponent::SetAbility(EAbilityKey Key, AAbility* Ability)
{
	if (Ability)
	{
		Abilities.Add(Key, Ability);
		Ability->Attach(GetOwner());
	}
}