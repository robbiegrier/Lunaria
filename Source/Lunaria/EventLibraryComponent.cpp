// Robbie Grier

#include "EventLibraryComponent.h"
#include "Printer.h"
#include "GameFramework/PlayerController.h"

UEventLibraryComponent::UEventLibraryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEventLibraryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEventLibraryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEventLibraryComponent::AnnounceEnemyHitWithAttack(AActor* HitActor)
{
	//Print("Enemy Hit With Attack. " + GetOwner()->GetName() + " hit " + HitActor->GetName());
}