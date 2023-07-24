// Robbie Grier

#include "CombatComponent.h"
#include "Printer.h"
#include "GameFramework/Actor.h"
#include "SpaceProjectile.h"
#include "Spaceship.h"
#include "GameFramework/Controller.h"
#include "Action.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::AddSummon(AActor* InSummon)
{
	Summons.Add(InSummon);

	if (auto Combat = InSummon->FindComponentByClass<UCombatComponent>())
	{
		auto SelfSummoner = Summoner ? Summoner : GetOwner();
		Combat->SetSummoner(SelfSummoner);
	}
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	while (Actions.Num() > 0)
	{
		auto Action = Actions.Pop();
		Action->Execute();
	}
}