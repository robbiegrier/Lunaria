// Robbie Grier

#include "GameplayEventObserver.h"

void IGameplayEventObserver::ExecuteAgentOf(const FGameplayEvent& Event)
{
	NativeWhenAgentOf(Event);
	Execute_WhenAgentOf(Cast<UObject>(this), Event);

	switch (Event.Action)
	{
	case ENativeEventType::Hit:
		Execute_WhenHitSomething(Cast<UObject>(this), Event);
		break;
	case ENativeEventType::Kill:
		Execute_WhenKilledSomething(Cast<UObject>(this), Event);
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