// Robbie Grier

#include "BobbleMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Helpers.h"
#include "Components/SceneComponent.h"
#include "Printer.h"

UBobbleMovementComponent::UBobbleMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBobbleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (auto Mesh = Helpers::GetMeshForFlairMovement(GetOwner()))
	{
		InitialHeight = Mesh->GetRelativeLocation().Z;
	}
}

void UBobbleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateBobbling(DeltaTime);
}

void UBobbleMovementComponent::UpdateBobbling(float DeltaTime)
{
	if (auto Mesh = Helpers::GetMeshForFlairMovement(GetOwner()))
	{
		if (FMath::Abs(Mesh->GetRelativeLocation().Z - InitialHeight) > BobbleDistance)
		{
			CurrentlyGoingUp = !CurrentlyGoingUp;
		}

		Mesh->AddRelativeLocation(FVector(0.f, 0.f, Helpers::Dilate(BobbleSpeed, GetWorld()) * (CurrentlyGoingUp ? 1.f : -1.f)));
	}
}