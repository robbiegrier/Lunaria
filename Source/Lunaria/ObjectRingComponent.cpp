// Robbie Grier

#include "ObjectRingComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Printer.h"
#include "SpaceObjectMovementComponent.h"

// Sets default values for this component's properties
UObjectRingComponent::UObjectRingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectRingComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto i = 0; i < TotalObjects; i++)
	{
		auto OwnerLocation = GetOwner()->GetActorLocation();
		auto Rx = FMath::RandRange(-1.f, 1.f);
		auto Ry = FMath::RandRange(-1.f, 1.f);

		auto Randv = FVector(Rx, Ry, 0.f);
		Randv.Normalize();
		Randv *= BaseRadius + FMath::RandRange(-RadiusVariance, RadiusVariance);
		Randv.Z = 0.f;
		Randv += OwnerLocation;

		auto T = FTransform(FRotator(), Randv, FVector(BaseObjectScale + FMath::RandRange(-ObjectScaleVariance, ObjectScaleVariance)));
		FTransform const* Tp = &T;

		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		auto Spawn = GetWorld()->SpawnActor(ObjectClass, Tp, Params);
		RingActors.Add(Spawn);

		if (auto MovementComponent = Spawn->FindComponentByClass<USpaceObjectMovementComponent>())
		{
			MovementComponent->InitializeOrbitalParent(GetOwner());
		}
	}
}

void UObjectRingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Clear();
}

void UObjectRingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UObjectRingComponent::Clear()
{
	for (auto Actor : RingActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	RingActors.Empty();
}