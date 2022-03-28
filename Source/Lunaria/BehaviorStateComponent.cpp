// Robbie Grier

#include "BehaviorStateComponent.h"

UBehaviorStateComponent::UBehaviorStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBehaviorStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBehaviorStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->Execute(DeltaTime);
	}
}

void UBehaviorStateComponent::SetState(TSubclassOf<UBehaviorState> StateClass)
{
	if (CurrentState)
	{
		CurrentState->NativeExit();
	}

	CurrentState = NewObject<UBehaviorState>((UObject*)GetTransientPackage(), StateClass);
	CurrentState->Initialize(this);
	CurrentState->NativeEnter();
}