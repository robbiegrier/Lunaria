// Robbie Grier

#include "GameplayEventObserver.h"
#include "Ability.h"
#include "AbilitySlot.h"
#include "Action.h"
#include "GameFramework/Pawn.h"

//void IGameplayEventObserver::ExecuteAgentOf(const FGameplayEvent& Event)
//{
//	//NativeWhenAgentOf(Event);
//	Execute_WhenAgentOf(Cast<UObject>(this), Event);
//
//	switch (Event.Action)
//	{
//	case ENativeEventType::Hit:
//		if (Event.Tool)
//		{
//			if (auto Ability = Cast<AAbility>(Event.Tool))
//			{
//				if (Ability->GetSlot()->GetSlotName().Equals("Attack"))
//				{
//					//Execute_WhenHitSomethingWithAttack(Cast<UObject>(this), Event);
//				}
//				else if (Ability->GetSlot()->GetSlotName().Equals("Special"))
//				{
//					Execute_WhenHitSomethingWithSpecial(Cast<UObject>(this), Event);
//				}
//				else if (Ability->GetSlot()->GetSlotName().Equals("Movement"))
//				{
//					Execute_WhenHitSomethingWithMovement(Cast<UObject>(this), Event);
//				}
//				else if (Ability->GetSlot()->GetSlotName().Equals("Defensive"))
//				{
//					Execute_WhenHitSomethingWithDefensive(Cast<UObject>(this), Event);
//				}
//			}
//		}
//
//		//Execute_WhenHitSomething(Cast<UObject>(this), Event);
//		break;
//	case ENativeEventType::Kill:
//		Execute_WhenKilledSomething(Cast<UObject>(this), Event);
//		break;
//	case ENativeEventType::AbilityUsed:
//		if (Event.Tool)
//		{
//			if (auto Ability = Cast<AAbility>(Event.Tool))
//			{
//				if (Ability->GetSlot()->GetSlotName().Equals("Attack"))
//				{
//					Execute_WhenAttackUsed(Cast<UObject>(this), Event);
//				}
//				else if (Ability->GetSlot()->GetSlotName().Equals("Special"))
//				{
//					Execute_WhenSpecialUsed(Cast<UObject>(this), Event);
//				}
//				else if (Ability->GetSlot()->GetSlotName().Equals("Movement"))
//				{
//					Execute_WhenMovementUsed(Cast<UObject>(this), Event);
//				}
//				else if (Ability->GetSlot()->GetSlotName().Equals("Defensive"))
//				{
//					Execute_WhenDefensiveUsed(Cast<UObject>(this), Event);
//				}
//			}
//		}
//
//		break;
//	default:
//		break;
//	}
//}

void IGameplayEventObserver::ExecuteAgentOf(UAction* Action)
{
	NativeWhenAgentOf(Action);
	Execute_WhenAgentOf(Cast<UObject>(this), Action);
	Action->VisitAsAgent(this);

	if (auto ThisAsActor = Cast<AActor>(this))
	{
		for (auto Component : ThisAsActor->GetComponentsByInterface(UGameplayEventObserver::StaticClass()))
		{
			Cast<IGameplayEventObserver>(Component)->ExecuteAgentOf(Action);
		}

		if (auto Pawn = Cast<APawn>(ThisAsActor))
		{
			if (auto PawnObserver = Cast<IGameplayEventObserver>(Pawn->GetController()))
			{
				PawnObserver->ExecuteAgentOf(Action);
			}
		}
	}
}

void IGameplayEventObserver::ExecuteSubjectOf(UAction* Action)
{
	NativeWhenSubjectOf(Action);
	Execute_WhenSubjectOf(Cast<UObject>(this), Action);
	Action->VisitAsSubject(this);

	if (auto ThisAsActor = Cast<AActor>(this))
	{
		for (auto Component : ThisAsActor->GetComponentsByInterface(UGameplayEventObserver::StaticClass()))
		{
			Cast<IGameplayEventObserver>(Component)->ExecuteSubjectOf(Action);
		}

		if (auto Pawn = Cast<APawn>(ThisAsActor))
		{
			if (auto PawnObserver = Cast<IGameplayEventObserver>(Pawn->GetController()))
			{
				PawnObserver->ExecuteSubjectOf(Action);
			}
		}
	}
}