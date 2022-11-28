// Robbie Grier

#include "AttributesComponent.h"
#include "GameFramework/Actor.h"
#include "Printer.h"
#include "CpuHealthBar.h"
#include "HealthComponent.h"
#include "AttributeObserver.h"
#include "StatusEffect.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		Boon->SetActorTransform(GetOwner()->GetActorTransform());
	}

	for (auto& StatusEffect : StatusEffects)
	{
		StatusEffect.Value->SetActorTransform(GetOwner()->GetActorTransform());
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
	ClearStatusEffects();
}

void UAttributesComponent::AddBoon(ABoon* NewBoon)
{
	if (NewBoon)
	{
		Boons.Add(NewBoon);
		NewBoon->NativeOnAdded(this);
	}
}

void UAttributesComponent::AddBoonFromClass(TSubclassOf<ABoon> NewBoonClass)
{
	if (NewBoonClass)
	{
		auto NewBoon = GetWorld()->SpawnActor<ABoon>(NewBoonClass);
		Boons.Add(NewBoon);
		NewBoon->NativeOnAdded(this);
	}
}

void UAttributesComponent::AddStatusEffectFromClass(TSubclassOf<class ABoon> NewEffectClass, AActor* Creator)
{
	if (NewEffectClass)
	{
		auto AddedEffect = static_cast<AStatusEffect*>(nullptr);
		auto Find = StatusEffects.Find(NewEffectClass);

		if (!Find)
		{
			AddedEffect = GetWorld()->SpawnActor<AStatusEffect>();
			StatusEffects.Add(NewEffectClass, AddedEffect);
			AddedEffect->SetBoonClass(NewEffectClass);
			AddedEffect->NativeOnAdded(this);
		}
		else
		{
			AddedEffect = *Find;
		}

		AddedEffect->AddStack(Creator);
	}
}

void UAttributesComponent::RemoveAndDestroyBoon(ABoon* TheBoon)
{
	if (TheBoon)
	{
		TheBoon->OnRemoved();
		Boons.Remove(TheBoon);
		TheBoon->Destroy();
	}
}

void UAttributesComponent::RemoveAndDestroyStatusEffect(AStatusEffect* TheStatusEffect)
{
	if (TheStatusEffect)
	{
		StatusEffects.Remove(TheStatusEffect->GetBoonClass());
		TheStatusEffect->Destroy();
	}
}

void UAttributesComponent::RemoveAndDestroyStatusEffectByName(const FString& StatusEffectName)
{
	auto TheEffect = static_cast<AStatusEffect*>(nullptr);

	for (auto& Pair : StatusEffects)
	{
		auto Boon = Pair.Value->GetStatuses()[0];
		if (Boon->GetBoonName() == StatusEffectName)
		{
			TheEffect = Pair.Value;
			break;
		}
	}

	RemoveAndDestroyStatusEffect(TheEffect);
}

void UAttributesComponent::ClearBoons()
{
	for (auto Boon : Boons)
	{
		if (Boon)
		{
			Boon->OnRemoved();
			Boon->Destroy();
		}
	}

	Boons.Empty();
}

void UAttributesComponent::ClearStatusEffects()
{
	for (auto& StatusEffect : StatusEffects)
	{
		if (StatusEffect.Value)
		{
			StatusEffect.Value->Destroy();
		}
	}

	StatusEffects.Empty();
}

float UAttributesComponent::ClassGet(UClass* Class, const FString& Attribute, float Seed)
{
	return ClassGetTagged(Class, FGameplayTag::RequestGameplayTag(*("Attribute." + Attribute)), Seed);
}

float UAttributesComponent::ClassGetTagged(UClass* Class, const FGameplayTag& Attribute, float Seed)
{
	return ClassGetFromTagContainer(Class, FGameplayTagContainer(Attribute), Seed);
}

float UAttributesComponent::ClassGetFromTagContainer(UClass* Class, const FGameplayTagContainer& Attribute, float Seed)
{
	auto Modifier = GetModifierFromTagContainer(Attribute, Seed, Class);
	return Modifier.Additive * Modifier.Multiplier;
}

float UAttributesComponent::Get(const FString& Attribute, float Seed)
{
	return GetTagged(FGameplayTag::RequestGameplayTag(*("Attribute." + Attribute)), Seed);
}

float UAttributesComponent::GetTagged(const FGameplayTag& Attribute, float Seed)
{
	return GetFromTagContainer(FGameplayTagContainer(Attribute), Seed);
}

float UAttributesComponent::GetFromTagContainer(const FGameplayTagContainer& Attribute, float Seed)
{
	auto Modifier = GetModifierFromTagContainer(Attribute, Seed);
	return Modifier.Additive * Modifier.Multiplier;
}

FAttributeModifier UAttributesComponent::GetModifierFromTagContainer(const FGameplayTagContainer& Attribute, float Seed, UClass* Class)
{
	auto Output = FAttributeModifier();
	Output.Additive = Seed;
	Output.Multiplier = 1.f;

	for (auto Boon : Boons)
	{
		Boon->BeforeAttributeQueried(Attribute);
		auto Modifier = Boon->GetModifierForTagContainer(Class, Attribute);

		Output.Additive += Modifier.Additive;
		Output.Multiplier += Modifier.Multiplier / 100.f;

		if (!Modifier.Color.Equals(FLinearColor::White))
		{
			Output.Color = Modifier.Color;
		}
	}

	for (auto& EffectRegister : StatusEffects)
	{
		for (auto Boon : EffectRegister.Value->GetStatuses())
		{
			Boon->BeforeAttributeQueried(Attribute);
			auto Modifier = Boon->GetModifierForTagContainer(Class, Attribute);

			Output.Additive += Modifier.Additive;
			Output.Multiplier += Modifier.Multiplier / 100.f;

			if (!Modifier.Color.Equals(FLinearColor::White))
			{
				Output.Color = Modifier.Color;
			}
		}
	}

	return Output;
}

FLinearColor UAttributesComponent::GetColor(const FGameplayTag& Attribute)
{
	return GetColorFromTagContainer(FGameplayTagContainer(Attribute));
}

FLinearColor UAttributesComponent::GetColorFromTagContainer(const FGameplayTagContainer& Attribute)
{
	return GetModifierFromTagContainer(Attribute, 0.f).Color;
}

float UAttributesComponent::GetForAbilityType(const FGameplayTag& Ability, const FGameplayTag& Attribute, float Seed)
{
	auto Container = FGameplayTagContainer(Ability);
	Container.AddTag(Attribute);
	return GetFromTagContainer(Container, Seed);
}

bool UAttributesComponent::IsNodeViable(const FUpgradeNode& Node)
{
	bool Viable = Node.Enablers.Num() == 0;

	for (auto Boon : GetCurrentBoons())
	{
		if (Boon->GetClass() == Node.BoonClass)
		{
			return false;
		}

		if (Node.Enablers.Contains(Boon->GetClass()))
		{
			Viable = true;
		}
	}

	return Viable;
}