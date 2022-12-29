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
#include "CombatCpu.h"
#include "CpuHealthBar.h"
#include "InventoryComponent.h"
#include "User.h"

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
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

float ASpaceship::GetCurrentTurnSpeed() const
{
	return SpaceshipMovementComponent->GetCurrentTurnSpeed();
}

void ASpaceship::InitializeEnemy()
{
	CombatComponent->SetTeam(UCombatComponent::EnemyTeam);

	for (const auto& BoonClass : Cast<ALunariaGameModeBase>(GetWorld()->GetAuthGameMode())->GetEnemySpawnBoons())
	{
		Attributes->AddBoonFromClass(BoonClass);
	}
}

void ASpaceship::InitializeAlly()
{
	CombatComponent->SetTeam(UCombatComponent::PlayerTeam);
}

void ASpaceship::SetSpawnTime(float Time)
{
	if (auto Cpu = Cast<ACombatCpu>(GetOwner()))
	{
		Cpu->SetSpawnTime(Time);
	}
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerControlled())
	{
		CombatComponent->SetTeam(UCombatComponent::PlayerTeam);
	}

	Helpers::BindHealthBar(this, HealthBarComponent);

	if (IsPlayerControlled())
	{
		//HealthBarComponent->SetVisibility(false);
		if (auto HealthBar = Cast<UCpuHealthBar>(HealthBarComponent->GetUserWidgetObject()))
		{
			HealthBar->HideHealthBar();
		}
	}
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
	OnRespawn();
}

void ASpaceship::NativeWhenAgentOf(const FGameplayEvent& Event)
{
	//Print("Was agent of " + Event.EventType);
}

void ASpaceship::EnterSpawningState()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto GameMode = Cast<ALunariaGameModeBase>(GetWorld()->GetAuthGameMode());
	auto SpawnColor = GameMode->GetGameColor("Spawning Tint");
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("Tint"), Helpers::GetVectorFromLinearColor(SpawnColor));
	HealthBarComponent->SetVisibility(false);
}

void ASpaceship::EnterCombatState()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetVectorParameterValueOnMaterials(TEXT("Tint"), Helpers::GetVectorFromLinearColor(FLinearColor::White));
	HealthBarComponent->SetVisibility(true);
}

void ASpaceship::StartPlayerDeath()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	Controller->DisableInput(Cast<APlayerController>(Controller));

	if (auto User = Cast<AUser>(Controller))
	{
		User->NativeOnDeath();
	}
}

void ASpaceship::EndPlayerDeath()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	Controller->EnableInput(Cast<APlayerController>(Controller));
	HealthComponent->Reset();
	Attributes->Reset();

	if (auto User = Cast<AUser>(Controller))
	{
		User->NativeOnRespawn();
	}
}

void ASpaceship::HandleThrottleInput(float Scale)
{
	if (SpaceshipMovementComponent)
	{
		SpaceshipMovementComponent->Accelerate(Scale * MovementScale);
	}
}

void ASpaceship::HandleRutterInput(float Scale)
{
	if (SpaceshipMovementComponent)
	{
		SpaceshipMovementComponent->Turn(Scale * RutterScaleModifier * MovementScale);
	}
}

void ASpaceship::TurnToFaceActor(AActor* InActor, float MinDeltaAngle)
{
	TurnToFaceLocation(InActor->GetActorLocation(), MinDeltaAngle);
}

void ASpaceship::TurnToFaceLocation(FVector InLocation, float MinDeltaAngle)
{
	TurnToFaceDirection((InLocation - GetActorLocation()).GetSafeNormal(), MinDeltaAngle);
}

void ASpaceship::TurnToFaceDirection(const FVector& Direction, float MinDeltaAngle)
{
	auto CosAngle = Direction.CosineAngle2D(GetActorForwardVector());

	if (FMath::Abs(CosAngle) < MinDeltaAngle || CosAngle < -MinDeltaAngle)
	{
		auto DotFromRight = FVector::DotProduct(GetActorRightVector(), Direction);
		HandleRutterInput(DotFromRight > 0.01f ? 1.f : DotFromRight < -0.001f ? -1.f : 0.0f);
	}
}