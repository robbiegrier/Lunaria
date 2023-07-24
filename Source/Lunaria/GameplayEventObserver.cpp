// Robbie Grier

#include "GameplayEventObserver.h"
#include "Ability.h"
#include "AbilitySlot.h"

void IGameplayEventObserver::ExecuteAgentOf(const FGameplayEvent& Event)
{
	NativeWhenAgentOf(Event);
	Execute_WhenAgentOf(Cast<UObject>(this), Event);

	switch (Event.Action)
	{
	case ENativeEventType::Hit:
		if (Event.Tool)
		{
			if (auto Ability = Cast<AAbility>(Event.Tool))
			{
				if (Ability->GetSlot()->GetSlotName().Equals("Attack"))
				{
					Execute_WhenHitSomethingWithAttack(Cast<UObject>(this), Event);
				}
				else if (Ability->GetSlot()->GetSlotName().Equals("Special"))
				{
					Execute_WhenHitSomethingWithSpecial(Cast<UObject>(this), Event);
				}
				else if (Ability->GetSlot()->GetSlotName().Equals("Movement"))
				{
					Execute_WhenHitSomethingWithMovement(Cast<UObject>(this), Event);
				}
				else if (Ability->GetSlot()->GetSlotName().Equals("Defensive"))
				{
					Execute_WhenHitSomethingWithDefensive(Cast<UObject>(this), Event);
				}
			}
		}

		Execute_WhenHitSomething(Cast<UObject>(this), Event);
		break;
	case ENativeEventType::Kill:
		Execute_WhenKilledSomething(Cast<UObject>(this), Event);
		break;
	case ENativeEventType::AbilityUsed:
		if (Event.Tool)
		{
			if (auto Ability = Cast<AAbility>(Event.Tool))
			{
				if (Ability->GetSlot()->GetSlotName().Equals("Attack"))
				{
					Execute_WhenAttackUsed(Cast<UObject>(this), Event);
				}
				else if (Ability->GetSlot()->GetSlotName().Equals("Special"))
				{
					Execute_WhenSpecialUsed(Cast<UObject>(this), Event);
				}
				else if (Ability->GetSlot()->GetSlotName().Equals("Movement"))
				{
					Execute_WhenMovementUsed(Cast<UObject>(this), Event);
				}
				else if (Ability->GetSlot()->GetSlotName().Equals("Defensive"))
				{
					Execute_WhenDefensiveUsed(Cast<UObject>(this), Event);
				}
			}
		}

		break;
	default:
		break;
	}
}

void IGameplayEventObserver::ExecuteSubjectOf(const FGameplayEvent& Event)
{
	NativeWhenSubjectOf(Event);
	Execute_WhenSubjectOf(Cast<UObject>(this), Event);

	switch (Event.Action)
	{
	case ENativeEventType::Hit:
		Execute_WhenHitBySomething(Cast<UObject>(this), Event);
		break;
	case ENativeEventType::Kill:
		Execute_WhenKilledBySomething(Cast<UObject>(this), Event);
		break;
	default:
		break;
	}
}