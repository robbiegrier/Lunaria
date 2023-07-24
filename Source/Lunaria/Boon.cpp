// Robbie Grier

#include "Boon.h"
#include "Printer.h"
#include "AttributesComponent.h"
#include "GameplayEventManager.h"
#include "Components/SceneComponent.h"
#include "SpaceProjectile.h"
#include "Engine/World.h"
#include "LunariaLib.h"
#include "LunariaGameModeBase.h"
#include "Modification.h"

FAttributeModifier ABoon::NullModifier = FAttributeModifier();

ABoon::ABoon()
{
	BoonName = "Spaceship Upgrade";
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	RootComponent = SceneComponent;
}

void ABoon::NativeOnAdded(UAttributesComponent* Attributes, AActor* InCreator)
{
	MyOwner = Attributes->GetOwner();
	MyOwnerAttributes = Attributes;
	Creator = InCreator ? InCreator : MyOwner;

	if (auto Attributes = Creator->FindComponentByClass<UAttributesComponent>())
	{
		DurationAsStatusEffect = Attributes->ClassGet(GetClass(), "Duration.StatusEffectsCreated", DurationAsStatusEffect);
	}

	if (auto Attributes = MyOwner->FindComponentByClass<UAttributesComponent>())
	{
		DurationAsStatusEffect = Attributes->ClassGet(GetClass(), "Duration.StatusEffectsOwned", DurationAsStatusEffect);
	}

	OnAdded();
}

void ABoon::NativeOnRemoved()
{
	Close();
	OnRemoved();
}

void ABoon::NativeLevelUp()
{
	BoonLevel++;

	for (auto& Modifier : AttributeModifierList)
	{
		Modifier.Additive += Modifier.AdditiveGainPerLevel;
		Modifier.Multiplier += Modifier.MultiplierGainPerLevel;
	}

	OnLeveledUp();
}

FString ABoon::GetChoiceName()
{
	return BoonName;
}

FString ABoon::GetChoiceDescription()
{
	return BoonDescription;
}

void ABoon::Choose(AActor* Chooser)
{
	if (auto Attributes = Chooser->FindComponentByClass<UAttributesComponent>())
	{
		Attributes->AddBoon(this);
	}
}

FAttributeModifier ABoon::GetModifierForTagContainer(UClass* InClass, const FGameplayTagContainer& Attribute) const
{
	return CalculateModifier(InClass, Attribute);
}

void ABoon::SetAttributeModifier(const FGameplayTagContainer& Attribute, const FAttributeModifier& Modifier)
{
	if (auto Find = FindModifier(Attribute))
	{
		*Find = Modifier;
	}
	else
	{
		AttributeModifierList.Add(Modifier);
	}
}

void ABoon::Remove()
{
	if (MyOwnerAttributes)
	{
		MyOwnerAttributes->RemoveAndDestroyBoon(this);
	}
}

float ABoon::GetDurationAsStatusEffect() const
{
	auto Output = DurationAsStatusEffect;

	if (Creator)
	{
		if (auto Attributes = Creator->FindComponentByClass<UAttributesComponent>())
		{
			Output = Attributes->ClassGet(GetClass(), "Duration.StatusEffectsCreated", Output);
		}
	}

	if (MyOwner)
	{
		if (auto Attributes = MyOwner->FindComponentByClass<UAttributesComponent>())
		{
			Output = Attributes->ClassGet(GetClass(), "Duration.StatusEffectsOwned", Output);
		}
	}

	return Output;
}

ASpaceProjectile* ABoon::CreateBoonProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float InDamage, float Distance, const FGameplayTagContainer& InTags, const TArray<AActor*>& ActorsToIgnore, int32 Bounces)
{
	auto Params = FActorSpawnParameters();
	Params.Owner = MyOwner;
	auto Projectile = GetWorld()->SpawnActor<ASpaceProjectile>(SpawnClass, Transform, Params);

	if (Projectile)
	{
		auto Color = ALunariaGameModeBase::Get(GetWorld())->GetArchetypeColor(GetArchetype());
		Projectile->SetIgnoreActors(ActorsToIgnore);
		Projectile->SetBounces(Bounces);
		Projectile->SetPayloadProperties(InTags, InDamage, Distance, Color);
	}

	return Projectile;
}

void ABoon::MakeStatModification(FGameplayTag StatTag, float Base, float Scalar)
{
	auto Modification = NewObject<UModificationStat>();
	auto FindAttribute = MyOwnerAttributes->StatMap.Find(StatTag);

	if (FindAttribute)
	{
		auto Attribute = *FindAttribute;

		if (auto Stat = Cast<UStat>(Attribute))
		{
			Modification->Set(Stat, Base, Scalar);
			Modifications.Add(Modification);
			Modification->Open();
		}
		else
		{
			Print("Cannot make stat Modification. Invalid Attribute type for: " + StatTag.ToString());
		}
	}
	else
	{
		Print("Cannot make stat Modification. Invalid Stat: " + StatTag.ToString());
	}
}

void ABoon::MakeColorModification(FGameplayTag Name, const FLinearColor& Color)
{
	auto Modification = NewObject<UModificationColor>();
	auto FindAttribute = MyOwnerAttributes->StatMap.Find(Name);

	if (FindAttribute)
	{
		auto Attribute = *FindAttribute;

		if (auto ColorAttr = Cast<UColorAttribute>(Attribute))
		{
			Modification->Set(ColorAttr, Color);
			Modifications.Add(Modification);
			Modification->Open();
		}
		else
		{
			Print("Cannot make color Modification. Invalid Attribute type for: " + Name.ToString());
		}
	}
	else
	{
		Print("Cannot make color Modification. Invalid Color: " + Name.ToString());
	}
}

void ABoon::Close()
{
	for (auto Mod : Modifications)
	{
		Mod->Close();
	}
}

FAttributeModifier* ABoon::FindModifier(const FGameplayTagContainer& Attribute) const
{
	return const_cast<FAttributeModifier*>(AttributeModifierList.FindByPredicate([&Attribute](const auto& Element) {
		return Attribute == Element.Attribute;
		}));
}

FAttributeModifier ABoon::CalculateModifier(UClass* InClass, const FGameplayTagContainer& Attribute) const
{
	auto Output = NullModifier;

	for (const auto& Element : AttributeModifierList)
	{
		auto IsMatchingClass = InClass == nullptr || Element.Class->IsChildOf(InClass);

		if (IsMatchingClass && Attribute.HasAll(Element.Attribute))
		{
			Output.Additive += Element.Additive;
			Output.Multiplier += Element.Multiplier;
			Output.Color = Element.Color;
			Output.Attribute.AppendTags(Element.Attribute);
		}
	}

	return Output;
}