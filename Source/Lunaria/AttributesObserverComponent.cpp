// Robbie Grier

#include "AttributesObserverComponent.h"
#include "AttributesComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

UAttributesObserverComponent::UAttributesObserverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesObserverComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributesObserverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}