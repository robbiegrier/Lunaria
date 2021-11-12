// Robbie Grier

#include "SpaceObjectMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Printer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Helpers.h"
#include "AttributesComponent.h"

USpaceObjectMovementComponent::USpaceObjectMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpaceObjectMovementComponent::RandomizeOrbitalDirection()
{
	if (FMath::RandBool())
	{
		FlipOrbitalDirection();
	}
}

void USpaceObjectMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Location = GetOwner()->GetActorLocation();
	Location.Z = Height;
	GetOwner()->SetActorLocation(Location);
	LocationLastFrame = GetOwner()->GetActorLocation();
	//QueryAndUpdateOrbitalParent();
	Attributes = GetOwner()->FindComponentByClass<UAttributesComponent>();
}

void USpaceObjectMovementComponent::QueryAndUpdateOrbitalParent()
{
	if (!OrbitStuff)
	{
		OrbitalParent = nullptr;
		return;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> HitArray;

	auto AnythingHit = UKismetSystemLibrary::SphereTraceMulti(
		GetOwner(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(),
		300.f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 60.f
	);

	if (AnythingHit)
	{
		AActor* MostMassive = nullptr;
		auto BiggestMass = -INFINITY;

		for (auto HitResult : HitArray)
		{
			if (auto Actor = HitResult.GetActor())
			{
				if (auto OtherComponent = Actor->FindComponentByClass<USpaceObjectMovementComponent>())
				{
					if (OtherComponent->GetMass() > BiggestMass)
					{
						BiggestMass = OtherComponent->GetMass();
						MostMassive = Actor;
					}
				}
			}
		}

		if (MostMassive)
		{
			OrbitalParent = MostMassive;
			OrbitalDistance = FVector::Distance(OrbitalParent->GetActorLocation(), GetOwner()->GetActorLocation());
			auto ToParent = GetOwner()->GetActorLocation() - OrbitalParent->GetActorLocation();
			auto Cos = ToParent.CosineAngle2D(FVector::ForwardVector);
			CurrentOrbitalRotation = FMath::RadiansToDegrees(FMath::Acos(Cos));
			if (ToParent.Y < 0.f) CurrentOrbitalRotation = 360.f - CurrentOrbitalRotation;
			UpdateOrbit(0.01f);
		}
	}
}

void USpaceObjectMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateOrbit(DeltaTime);
	UpdateFlairRotation(DeltaTime);
}

void USpaceObjectMovementComponent::InitializeOrbitalParent(AActor* Actor)
{
	SetOrbitalParent(Actor);
	GetOwner()->SetActorLocation(GetTargetLocation());
}

void USpaceObjectMovementComponent::SetOrbitalParent(AActor* Actor)
{
	OrbitalParent = Actor;
	OrbitalDistance = FVector::Distance(OrbitalParent->GetActorLocation(), GetOwner()->GetActorLocation());
	auto ToParent = GetOwner()->GetActorLocation() - OrbitalParent->GetActorLocation();
	auto Cos = ToParent.CosineAngle2D(FVector::ForwardVector);
	CurrentOrbitalRotation = FMath::RadiansToDegrees(FMath::Acos(Cos));
	if (ToParent.Y < 0.f) CurrentOrbitalRotation = 360.f - CurrentOrbitalRotation;
	UpdateOrbit(0.01f);
}

void USpaceObjectMovementComponent::UpdateOrbit(float DeltaTime)
{
	if (OrbitalParent && OrbitalParent->IsActorBeingDestroyed())
	{
		OrbitalParent = nullptr;
	}

	if (OrbitalParent)
	{
		GetOwner()->SetActorLocation(FMath::Lerp(GetOwner()->GetActorLocation(), GetTargetLocation(), 0.2f));
		auto MoveSpeed = Attributes ? Attributes->Get("MoveSpeed", OrbitalSpeed) : OrbitalSpeed;
		CurrentOrbitalRotation = FMath::Fmod(CurrentOrbitalRotation - (MoveSpeed * DeltaTime), 360.f);
		Velocity = GetOwner()->GetActorLocation() - LocationLastFrame;
		LocationLastFrame = GetOwner()->GetActorLocation();
	}
	else
	{
		auto Scale = Velocity.Size();
		auto Direction = Velocity.GetSafeNormal();
		Scale = Attributes ? Attributes->Get("MoveSpeed", Scale) : Scale;
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + (Direction * Scale));
	}
}

void USpaceObjectMovementComponent::UpdateFlairRotation(float DeltaTime)
{
	if (auto Mesh = Helpers::GetMeshForFlairMovement(GetOwner()))
	{
		auto ScaleFlair = [DeltaTime, this](auto Flair) { return Helpers::Dilate(Flair * PI / 4.f, GetWorld()); };
		Mesh->AddLocalRotation(FQuat(Mesh->GetUpVector(), ScaleFlair(Spin)));
		Mesh->AddLocalRotation(FQuat(FVector::RightVector, ScaleFlair(Roll)));
		Mesh->AddLocalRotation(FQuat(Mesh->GetForwardVector(), ScaleFlair(Tilt)));
	}
}

FVector USpaceObjectMovementComponent::GetTargetLocation()
{
	if (OrbitalParent)
	{
		auto CurrentValueInRadians = FMath::DegreesToRadians<float>(CurrentOrbitalRotation);

		return FVector(
			OrbitalParent->GetActorLocation().X + OrbitalDistance * FMath::Cos(CurrentValueInRadians),
			OrbitalParent->GetActorLocation().Y + OrbitalDistance * FMath::Sin(CurrentValueInRadians),
			GetOwner()->GetActorLocation().Z
		);
	}
	else
	{
		return GetOwner()->GetActorLocation();
	}
}