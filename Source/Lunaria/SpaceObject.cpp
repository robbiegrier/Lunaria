// Robbie Grier

#include "SpaceObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "SpaceObjectMovementComponent.h"
#include "BobbleMovementComponent.h"
#include "DebugWidget.h"
#include "DrawDebugHelpers.h"
#include "Printer.h"
#include "LunariaGameModeBase.h"

ASpaceObject::ASpaceObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Shell"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SpaceObjectMovementComponent = CreateDefaultSubobject<USpaceObjectMovementComponent>(TEXT("SpaceObject Movement Component"));
	BobbleMovementComponent = CreateDefaultSubobject<UBobbleMovementComponent>(TEXT("Bobble Movement Component"));

	RootComponent = SphereComponent;
	Mesh->SetupAttachment(SphereComponent);

	DebugWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Debug Widget Component"));
	DebugWidgetComponent->SetupAttachment(SphereComponent);

	DebugWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DebugWidgetComponent->SetDrawAtDesiredSize(true);

	SphereComponent->SetCollisionProfileName("SpaceObject");
}

void ASpaceObject::ToggleDetailOn()
{
	DebugWidgetComponent->SetVisibility(true);
}

void ASpaceObject::ToggleDetailOff()
{
	DebugWidgetComponent->SetVisibility(false);
}

void ASpaceObject::BeginPlay()
{
	Super::BeginPlay();
	SetInitialDetailToggleState();

	if (auto Gamemode = Cast<ALunariaGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		DebugWidgetComponent->SetWidgetClass(Gamemode->GetDebugWidgetClass());
	}
}

void ASpaceObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DebugWidgetComponent->GetVisibleFlag())
	{
		if (auto DebugWidget = Cast<UDebugWidget>(DebugWidgetComponent->GetUserWidgetObject()))
		{
			auto OrbitalParent = SpaceObjectMovementComponent->GetOrbitalParent();

			auto Str = FString();
			Str += OrbitalParent != nullptr ? "Orbiting " + OrbitalParent->GetName() : "No Orbital Parent";

			DebugWidget->SetText(GetName());
			DebugWidget->SetSubText(Str);

			DrawDebugLine(GetWorld(), GetActorLocation(), DebugWidgetComponent->GetComponentLocation(), FColor::Blue, false, -1.f, 0, 10);
			DebugWidgetComponent->SetWorldRotation(FRotator());
		}
	}
}