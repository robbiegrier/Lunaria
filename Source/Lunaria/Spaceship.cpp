// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpaceshipMovementComponent.h"
#include "SpaceshipWeaponComponent.h"
#include "EventLibraryComponent.h"
#include "Printer.h"
#include "Helpers.h"
#include "Obstacle.h"
#include "HealthComponent.h"
#include "AttributesComponent.h"
#include "LunariaGameModeBase.h"
#include "MapManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitiesComponent.h"
#include "CombatComponent.h"
#include "Components/WidgetComponent.h"

ASpaceship::ASpaceship()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 50.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.2f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetMesh()->SetCastShadow(false);
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Spaceship")));

	GenericAwarenessVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Generic Awareness Volume Component"));
	GenericAwarenessVolume->SetupAttachment(RootComponent);
	GenericAwarenessVolume->SetSphereRadius(2000.f);
	GenericAwarenessVolume->ShapeColor = FColor::Cyan;

	SpaceshipMovementComponent = CreateDefaultSubobject<USpaceshipMovementComponent>("Spaceship Movement Component");

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Volume Component"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetSphereRadius(100.f);
	InteractionVolume->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	InteractionVolume->ShapeColor = FColor::Emerald;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget Component"));
	HealthBarComponent->SetupAttachment(GetCapsuleComponent());

	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarComponent->SetDrawAtDesiredSize(true);

	DebugViz = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugViz Component"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes Component"));

	AbilitiesComponent = CreateDefaultSubobject<UAbilitiesComponent>(TEXT("Abilities Component"));

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();
	CombatComponent->SetTeam(IsPlayerControlled() ? UCombatComponent::PlayerTeam : UCombatComponent::EnemyTeam);
	Helpers::BindHealthBar(this, HealthBarComponent);
}

void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerControlled())
	{
		auto Map = ALunariaGameModeBase::Get(GetWorld())->GetMap();

		if (FVector::DistSquared(GetActorLocation(), Map->GetCenter()) > Map->GetRadius() * Map->GetRadius())
		{
			SetActorLocation(Map->GetNearestPointInsideMap(GetActorLocation(), GetCapsuleComponent()->GetScaledCapsuleRadius()));
		}
	}
}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASpaceship::RespawnOnMap(AMapManager* MapManager)
{
	SetActorLocation(MapManager->GetNearestPointInsideMap(MapManager->GetEntrance()->GetActorLocation(), 100.f));
	SetActorRotation(UKismetMathLibrary::MakeRotFromX(MapManager->GetEntryDirection()));
}

void ASpaceship::NativeWhenAgentOf(const FGameplayEvent& Event)
{
	//Print("Was agent of " + Event.EventType);
}

void ASpaceship::HandleThrottleInput(float Scale)
{
	if (SpaceshipMovementComponent)
	{
		SpaceshipMovementComponent->Accelerate(Scale);
	}
}

void ASpaceship::HandleRutterInput(float Scale)
{
	if (SpaceshipMovementComponent)
	{
		SpaceshipMovementComponent->Turn(Scale);
	}
}

void ASpaceship::TurnToFaceDirection(const FVector& Direction, float MinDeltaAngle)
{
	auto CosAngle = Direction.CosineAngle2D(GetActorForwardVector());

	if (FMath::Abs(CosAngle) < MinDeltaAngle || CosAngle < -MinDeltaAngle)
	{
		auto DotFromRight = FVector::DotProduct(GetActorRightVector(), Direction);
		HandleRutterInput(DotFromRight > 0.0f ? 1.f : DotFromRight < -0.0f ? -1.f : 0.0f);
	}
}