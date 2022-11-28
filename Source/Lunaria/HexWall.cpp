// Robbie Grier

#include "HexWall.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "SpaceObject.h"
#include "Helpers.h"
#include "Printer.h"

AHexWall::AHexWall()
{
	PrimaryActorTick.bCanEverTick = true;

	Shell = CreateDefaultSubobject<USphereComponent>(TEXT("Shell Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SpaceObjectDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Space Object Detector"));

	RootComponent = Shell;
	Mesh->SetupAttachment(RootComponent);
	SpaceObjectDetector->SetupAttachment(RootComponent);
	SpaceObjectDetector->SetCapsuleRadius(1000.f);
}

// Called when the game starts or when spawned
void AHexWall::BeginPlay()
{
	Super::BeginPlay();
	RestPosition = GetActorLocation();
}

// Called every frame
void AHexWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Sinking)
	{
		auto Actors = TArray<AActor*>();
		GetOverlappingActors(Actors, TSubclassOf<ASpaceObject>());

		auto Closest = Helpers::GetClosestToLocation(GetActorLocation(), Actors);
		auto Distance = FVector::Distance(GetActorLocation(), Closest->GetActorLocation());
		auto Closeness = FMath::Max(0.f, SinkRadius - Distance);

		auto TargetLocation = RestPosition + (FVector::DownVector * Closeness);
		auto NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, .5f);

		SetActorLocation(NewLocation);
		AddActorWorldOffset(FVector::DownVector * Helpers::Dilate(SinkSpeed * Closeness / 10.f, GetWorld()));
	}
	else if (GetActorLocation().Z < RestPosition.Z)
	{
		AddActorWorldOffset(FVector::UpVector * Helpers::Dilate(SinkSpeed, GetWorld()));
	}

	auto DistanceSunk = FVector::DistSquared(RestPosition, GetActorLocation());
	Mesh->SetScalarParameterValueOnMaterials(TEXT("Sink"), 1.f - (DistanceSunk / DarknessCreep));
}

FVector AHexWall::GetAdjecentSlot(int ClockSide)
{
	const static auto AnglePerSide = 360.f / 6.f;
	auto Direction = FVector::ForwardVector;
	Direction = Direction.RotateAngleAxis(AnglePerSide * ClockSide, FVector::UpVector);
	return RestPosition + (Direction * Seperation);
}

void AHexWall::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (auto SpaceObject = Cast<ASpaceObject>(OtherActor))
	{
		Sinking = true;
	}
}

void AHexWall::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto Actors = TArray<AActor*>();
	GetOverlappingActors(Actors, TSubclassOf<ASpaceObject>());

	if (Actors.Num() <= 0)
	{
		Sinking = false;
	}
}