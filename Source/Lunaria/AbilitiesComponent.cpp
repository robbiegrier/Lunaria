// Robbie Grier

#include "AbilitiesComponent.h"
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
}

void UAbilitiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto Ability : Abilities)
	{
		if (Ability)
		{
			Ability->SetActorTransform(GetOwner()->GetActorTransform());
		}
	}
}

void UAbilitiesComponent::PressAbility(int32 AbilitySlot)
{
	if (Helpers::IsValidIndex(Abilities, AbilitySlot) && Abilities[AbilitySlot])
	{
		Abilities[AbilitySlot]->Press();
	}
}

void UAbilitiesComponent::ReleaseAbility(int32 AbilitySlot)
{
	if (Helpers::IsValidIndex(Abilities, AbilitySlot) && Abilities[AbilitySlot])
	{
		Abilities[AbilitySlot]->Release();
	}
}

void UAbilitiesComponent::CancelAbility(int32 AbilitySlot)
{
	if (Helpers::IsValidIndex(Abilities, AbilitySlot) && Abilities[AbilitySlot])
	{
		Abilities[AbilitySlot]->Cancel();
	}
}

void UAbilitiesComponent::SetAbility(int32 AbilitySlot, AAbility* Ability)
{
	if (!Ability) return;
	if (AbilitySlot < 0) return;

	Ability->Attach(GetOwner());

	if (!Helpers::IsValidIndex(Abilities, AbilitySlot))
	{
		auto LastIndex = Abilities.Num() - 1;
		while (LastIndex < AbilitySlot)
		{
			++LastIndex;
			Abilities.Add(nullptr);
		}
	}

	Abilities[AbilitySlot] = Ability;
}