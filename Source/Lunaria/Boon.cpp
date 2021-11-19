// Robbie Grier

#include "Boon.h"
#include "Printer.h"
#include "AttributesComponent.h"
#include "GameplayEventManager.h"
#include "Components/SceneComponent.h"

FAttributeModifier ABoon::NullModifier = FAttributeModifier();

ABoon::ABoon()
{
	BoonName = "Spaceship Upgrade";
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	RootComponent = SceneComponent;
}

void ABoon::NativeOnAdded(UAttributesComponent* Attributes)
{
	MyOwner = Attributes->GetOwner();
	MyOwnerAttributes = Attributes;
	OnAdded();
}

const FAttributeModifier& ABoon::GetModifierForTagContainer(const FGameplayTagContainer& Attribute) const
{
	auto Find = FindModifier(Attribute);
	return Find ? *Find : NullModifier;
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

FAttributeModifier* ABoon::FindModifier(const FGameplayTagContainer& Attribute) const
{
	return const_cast<FAttributeModifier*>(AttributeModifierList.FindByPredicate([&Attribute](const auto& Element) {
		return Attribute.HasAll(Element.Attribute);
	}));
}