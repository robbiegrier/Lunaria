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
		Color = NewObject<ULunariaColor>();
		Color->Set(FLinearColor::White);
		Attributes->RegisterAttribute(SlotName + ".Color", Color);
	}
}

void UAbilitySlot::SetAbility(AAbility* InAbility)
{
	Ability = InAbility;
	InAbility->SetAgent(Parent->GetOwner());
	InAbility->Attach(this);
}