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
	CharacterMovement = Helpers::GetSister<UCharacterMovementComponent>(this);
}

void USpaceshipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateBanking();

	auto Location = GetOwner()->GetActorLocation();
	Location.Z = 100.f;
	GetOwner()->SetActorLocation(Location);

	CharacterMovement->MaxWalkSpeed = FMath::Max(Attributes->Get("Speed.Move", MoveSpeedSeed), 0.f);
}

void USpaceshipMovementComponent::Accelerate(float Scale)
{
	if (!FMath::IsNearlyZero(Scale))
	{
		if (auto Pawn = Cast<APawn>(GetOwner()))
		{
			auto FrameThrust = Helpers::Dilate(Scale * FMath::Max(Attributes->Get("Speed.Move", MoveSpeedSeed), 0.f), GetWorld());
			Pawn->AddActorWorldOffset(Pawn->GetTransform().GetRotation().GetForwardVector() * FrameThrust);
		}
	}
}

void USpaceshipMovementComponent::Turn(float Scale)
{
	ExecuteTurning(Scale, FMath::Max(Attributes->Get("Speed.Turn", TurnSpeedSeed), 0.f));
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
				Pawn->SetActorRotation(Pawn->GetActorRotation().Add(0.f, FrameTorc, 0.f));
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