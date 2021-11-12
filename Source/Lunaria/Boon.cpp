// Robbie Grier

#include "Boon.h"
#include "Printer.h"
#include "AttributesComponent.h"
#include "GameplayEventManager.h"

ABoon::ABoon()
{
	BoonName = "Spaceship Upgrade";
}

void ABoon::NativeOnAdded(UAttributesComponent* Attributes)
{
	MyOwner = Attributes->GetOwner();
	MyOwnerAttributes = Attributes;
	OnAdded();
}

FAttributeModifier ABoon::GetAttributeModifier(const FString& Attribute) const
{
	if (auto Find = AttributeModifiers.Find(Attribute))
	{
		return *Find;
	}
	else
	{
		return FAttributeModifier();
	}
}

void ABoon::SetAttributeModifier(const FString& Attribute, const FAttributeModifier& Modifier)
{
	AttributeModifiers.Add(Attribute, Modifier);
}

void ABoon::SetAttributeModifierAdditive(const FString& Attribute, float Additive)
{
	auto Modifier = GetAttributeModifier(Attribute);
	Modifier.Additive = Additive;
	SetAttributeModifier(Attribute, Modifier);
}

void ABoon::SetAttributeModifierMultiplier(const FString& Attribute, float Multiplier)
{
	auto Modifier = GetAttributeModifier(Attribute);
	Modifier.Multiplier = Multiplier;
	SetAttributeModifier(Attribute, Modifier);
}

void ABoon::Remove()
{
	if (MyOwnerAttributes)
	{
		MyOwnerAttributes->RemoveAndDestroyBoon(this);
	}
}