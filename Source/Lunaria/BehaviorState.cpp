// Robbie Grier

#include "BehaviorState.h"
#include "BehaviorStateComponent.h"
#include "CombatCpu.h"

UBehaviorState::UBehaviorState()
{
}

void UBehaviorState::Initialize(UBehaviorStateComponent* InStateMachine)
{
	StateMachine = InStateMachine;
}

ACombatCpu* UBehaviorState::GetOwner()
{
	return Cast<ACombatCpu>(StateMachine->GetOwner());
}