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
		if (auto Ability = Pair.Value->GetAbility())
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
		if (auto Ability = Pair.Value->GetAbility())
		{
			Ability->SetActorTransform(GetOwner()->GetActorTransform());
		}
	}
}

AAbility* UAbilitiesComponent::GetAbility(EAbilityKey Key) const
{
	if (auto Find = Abilities.Find(Key))
	{
		return (*Find)->GetAbility();
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
		EnsureSlotExists(Key);

		if (auto Find = Abilities.Find(Key))
		{
			auto& Slot = *Find;
			Slot->SetAbility(Ability);
		}
	}
}

void UAbilitiesComponent::EnsureSlotExists(EAbilityKey Key)
{
	if (!Abilities.Find(Key))
	{
		auto Slot = NewObject<UAbilitySlot>();
		Slot->SetParent(this);
		Slot->SetSlotName(AbilityKeyToSlotName(Key));
		Slot->SetAbilityKey(Key);
		Abilities.Add(Key, Slot);
		Slot->Begin();
	}
}

FGameplayTag UAbilitiesComponent::AbilityKeyToTag(EAbilityKey Key)
{
	auto AbilityTag = FGameplayTag();

	switch (Key)
	{
	case EAbilityKey::A:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Movement"));
		break;
	case EAbilityKey::B:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Defensive"));
		break;
	case EAbilityKey::X:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"));
		break;
	case EAbilityKey::Y:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Special"));
		break;
	case EAbilityKey::LT:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Other"));
		break;
	default:
		break;
	}

	return AbilityTag;
}

FString UAbilitiesComponent::AbilityKeyToSlotName(EAbilityKey Key)
{
	auto Output = FString();

	switch (Key)
	{
	case EAbilityKey::A:
		Output = "Movement";
		break;
	case EAbilityKey::B:
		Output = "Defensive";
		break;
	case EAbilityKey::X:
		Output = "Attack";
		break;
	case EAbilityKey::Y:
		Output = "Special";
		break;
	case EAbilityKey::LT:
		Output = "Other";
		break;
	default:
		break;
	}

	return Output;
}