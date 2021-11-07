// Robbie Grier

#include "SpaceshipMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributesComponent.h"
#include "Helpers.h"
#include "Printer.h"
#include "Components/SphereComponent.h"
#include "Spaceship.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "LunariaGameModeBase.h"
#include "MapManager.h"

float USpaceshipMovementComponent::CpuTurnCompensation = 2.5f;

USpaceshipMovementComponent::USpaceshipMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpaceshipMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Attributes = Helpers::GetSister<UAttributesComponent>(this);
}

void USpaceshipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateBanking();

	if (TargetingIsActive)
	{
		UpdateTargeting();
	}

	if (TargetingVisualization)
	{
		TargetingVisualization->SetVisibility(TargetingIsActive && TargetIsAquired);
	}

	if (BlinkVisualization)
	{
		BlinkVisualization->SetVisibility(BlinkIsQueued);
	}

	if (BlinkIsQueued)
	{
		UpdateBlinkQueue();
	}

	auto Location = GetOwner()->GetActorLocation();
	Location.Z = 100.f;
	GetOwner()->SetActorLocation(Location);
}

void USpaceshipMovementComponent::ActivateTargeting()
{
	TargetingIsActive = true;
}

void USpaceshipMovementComponent::DeactivateTargeting()
{
	TargetingIsActive = false;
}

void USpaceshipMovementComponent::QueueBlink()
{
	BlinkIsQueued = true;
}

void USpaceshipMovementComponent::ExecuteBlink()
{
	BlinkIsQueued = false;

	auto BlinkLocation = GetBlinkLocation();

	auto Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	auto CameraPrevious = FVector();

	if (Camera)
	{
		CameraPrevious = Camera->GetComponentLocation();
	}

	GetOwner()->SetActorLocation(GetBlinkLocation(), false, nullptr, ETeleportType::TeleportPhysics);

	if (Camera)
	{
		Camera->SetWorldLocation(CameraPrevious);
	}
}

void USpaceshipMovementComponent::Accelerate(float Scale)
{
	if (!FMath::IsNearlyZero(Scale))
	{
		if (auto Pawn = Cast<APawn>(GetOwner()))
		{
			auto FrameThrust = Helpers::Dilate(Scale * Attributes->GetMoveSpeed(), GetWorld());
			Pawn->AddActorWorldOffset(Pawn->GetTransform().GetRotation().GetForwardVector() * FrameThrust);
		}
	}
}

void USpaceshipMovementComponent::Turn(float Scale)
{
	if (!TargetIsAquired)
	{
		ExecuteTurning(Scale, Attributes->GetTurnSpeed());
	}

	TargetIsAquired = false;
}

void USpaceshipMovementComponent::ExecuteTurning(float Scale, float Speed)
{
	if (!FMath::IsNearlyZero(Scale))
	{
		if (auto Pawn = Cast<APawn>(GetOwner()))
		{
			auto FrameTorc = Helpers::Dilate(Scale * Speed, GetWorld());

			if (Pawn->IsPlayerControlled())
			{
				Pawn->SetActorRotation(Pawn->GetActorRotation().Add(0.f, FrameTorc, 0.f));
			}
			else
			{
				Pawn->SetActorRotation(Pawn->GetActorRotation().Add(0.f, FrameTorc * CpuTurnCompensation, 0.f));
			}

			CurrentBankValue += Helpers::Dilate(Scale * BankAmount, GetWorld());
		}
	}
}

void USpaceshipMovementComponent::UpdateBanking()
{
	if (!FMath::IsNearlyZero(CurrentBankValue, 0.0001f))
	{
		if (auto Character = Cast<ACharacter>(GetOwner()))
		{
			auto FrameBankFalloff = Helpers::Dilate(BankFalloff, GetWorld());
			CurrentBankValue -= (CurrentBankValue > 0.0f ? FrameBankFalloff : -FrameBankFalloff);
			CurrentBankValue = FMath::Clamp(CurrentBankValue, -MaxBankLeft, MaxBankRight);
			Character->GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(CurrentBankValue, 0.f, 0.f)));
		}
	}
}

void USpaceshipMovementComponent::UpdateTargeting()
{
	if (!TargetingVolume) return;

	TArray<AActor*> Actors;
	TargetingVolume->GetOverlappingActors(Actors, UHittable::StaticClass());

	auto Closest = Helpers::GetClosestToLocation(GetOwner()->GetActorLocation(), Actors);

	if (Closest)
	{
		TargetIsAquired = true;

		auto ToClosest = (Closest->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
		auto DotFromRight = FVector::DotProduct(GetOwner()->GetActorRightVector(), ToClosest);

		ExecuteTurning(DotFromRight > 0.0f ? 1.0f : DotFromRight < -0.0f ? -1.0f : 0.f, Attributes->GetTargetingTurnSpeed());

		if (TargetingVisualization)
		{
			auto Direction = ToClosest.GetSafeNormal();
			auto Midpoint = (GetOwner()->GetActorLocation() + Closest->GetActorLocation()) * .5f;
			TargetingVisualization->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(Direction));
			TargetingVisualization->SetWorldLocation(Midpoint);

			TargetingVisualization->SetRelativeScale3D(
				FVector((Closest->GetActorLocation() - GetOwner()->GetActorLocation()).Size() / 125.f,
					TargetingVisualization->GetRelativeScale3D().Y,
					TargetingVisualization->GetRelativeScale3D().Z)
			);
		}
	}
}

void USpaceshipMovementComponent::UpdateBlinkQueue()
{
	if (!BlinkVisualization) return;

	auto PotentialBlinkLocation = GetBlinkLocation();
	auto MidPoint = (GetOwner()->GetActorLocation() + PotentialBlinkLocation) * 0.5f;
	auto Direction = (PotentialBlinkLocation - GetOwner()->GetActorLocation()).GetSafeNormal();
	BlinkVisualization->SetWorldLocation(MidPoint);
	BlinkVisualization->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(Direction));

	BlinkVisualization->SetRelativeScale3D(
		FVector((PotentialBlinkLocation - GetOwner()->GetActorLocation()).Size() / 125.f,
			BlinkVisualization->GetRelativeScale3D().Y,
			BlinkVisualization->GetRelativeScale3D().Z)
	);
}

FVector USpaceshipMovementComponent::GetBlinkLocation()
{
	auto BlinkLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector().GetSafeNormal() * 300.f);

	auto Map = ALunariaGameModeBase::Get(GetWorld())->GetMap();
	auto LocalMapCenter = Map->GetCenter();
	LocalMapCenter.Z = GetOwner()->GetActorLocation().Z;
	auto FeatheredMapRadius = Map->GetRadius() - (GetOwner()->GetSimpleCollisionRadius());

	BlinkLocation = Helpers::NearestPointInsideSphere(BlinkLocation, LocalMapCenter, FeatheredMapRadius);
	return BlinkLocation;
}