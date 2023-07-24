// Robbie Grier

#include "AbilitySlot.h"
#include "AbilitiesComponent.h"
#include "AttributesComponent.h"
#include "Ability.h"
#include "Stat.h"

void UAbilitySlot::Begin()
{
	if (auto Attributes = GetParent()->GetOwner()->FindComponentByClass<UAttributesComponent>())
	{
		Damage = NewObject<UStat>();
		Damage->Set(0.f);
		Attributes->RegisterAttribute(SlotName + ".Damage", Damage);

		Color = NewObject<UColorAttribute>();
		Color->Set(FLinearColor::White);
		Attributes->RegisterAttribute(SlotName + ".Color", Color);
	}
}

void UAbilitySlot::SetAbility(AAbility* InAbility)
{
	Ability = InAbility;
	InAbility->Attach(this);
}