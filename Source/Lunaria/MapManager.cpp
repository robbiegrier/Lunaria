// Robbie Grier

#include "MapManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Door.h"
#include "Printer.h"
#include "Helpers.h"
#include "DrawDebugHelpers.h"
#include "SpaceObjectMovementComponent.h"

AMapManager::AMapManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	Center = FVector(0.f, 0.f, MapElementHeight);
	Radius = 500.f;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Component"));
	CenterVisualization = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center Visualization Component"));
	CircleVisualization = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Circle Visualization Component"));
	CosmicBackgroundVisualization = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cosmic Background Visualization Component"));
	ArenaWalls = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arena Walls Visualization Component"));

	RootComponent = SceneRoot;
	CenterVisualization->SetupAttachment(SceneRoot);
	CircleVisualization->SetupAttachment(SceneRoot);
	CosmicBackgroundVisualization->SetupAttachment(SceneRoot);
	ArenaWalls->SetupAttachment(SceneRoot);

	CenterVisualization->SetCollisionProfileName("NoCollision");
	CircleVisualization->SetCollisionProfileName("NoCollision");
	CosmicBackgroundVisualization->SetCollisionProfileName("NoCollision");
	CenterVisualization->SetWorldScale3D(FVector(.2f, .2f, .2f));

	InitializeCircleVisualization();
}

void AMapManager::BeginPlay()
{
	Super::BeginPlay();
	Entrance = GetWorld()->SpawnActor<AActor>(EntranceClass);
	checkf(Entrance, TEXT("Entrance was not spawned. Specify EntranceClass"));
}

void AMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapManager::ToggleDetailOn()
{
	SetWallsDetailFlag(1.f);
}

void AMapManager::ToggleDetailOff()
{
	SetWallsDetailFlag(0.f);
}

void AMapManager::SetWallsDetailFlag(float Value)
{
	ArenaWalls->SetScalarParameterValueOnMaterials(FName("Global Detail Flag"), Value);
}

void AMapManager::LoadNewMap(float Scale, const FVector& ExitDirection, int32 NumberOfDoors)
{
	CleanupPreviousMap();
	SetRadius(Scale);
	SetupNewEntry(ExitDirection);
	SpawnRandomDoors(NumberOfDoors);
	InitializeCircleVisualization();
	SetRandomCosmicSoup();
	SpawnWalls();

	for (auto i = 0; i < 1; i++)
	{
		auto ScaleMax = 2.5f;
		auto ScaleMin = 1.5f;

		auto SpawnLocation = Center; //GetRandomPointInsideMap();
		auto Spawn = SpawnRootEnvironmentActor(SpawnLocation);
		auto Scale1 = FMath::RandRange(1.1f, 2.1f);
		Spawn->SetActorScale3D(FVector(Scale1));

		auto ClockwiseSecondary = FMath::RandBool();
		auto NumSecondaryObjects = FMath::RandRange(1, 10);
		for (auto j = 0; j < NumSecondaryObjects; j++)
		{
			auto SpawnRadius2 = FMath::RandRange(400.f, FMath::Max(400.f, Radius - 50.f));
			auto SpawnLocation2 = SpawnLocation + Helpers::GetRandomDirection2D() * SpawnRadius2;
			auto Spawn2 = SpawnEnvironmentActor(SpawnLocation2);
			auto Scale2 = FMath::RandRange(Scale1 / ScaleMax, Scale1 / ScaleMin);
			Spawn2->SetActorScale3D(FVector(Scale2));

			if (auto MovementComponent = Spawn2->FindComponentByClass<USpaceObjectMovementComponent>())
			{
				MovementComponent->InitializeOrbitalParent(Spawn);
				MovementComponent->SetOrbitalDirection(ClockwiseSecondary);
				auto Distance = FVector::Dist(Spawn2->GetActorLocation(), Spawn->GetActorLocation());
				auto InverseDistance = Radius - Distance;
				MovementComponent->SetOrbitalSpeed((InverseDistance / 100.f) * GlobalOrbitSpeedMultiplier);
			}

			auto ClockwiseTertiary = FMath::RandBool();
			auto NumTertiaryObjects = FMath::RandRange(1, 3);
			for (auto k = 0; k < NumTertiaryObjects; k++)
			{
				auto SpawnRadius3 = FMath::RandRange(100.f, 200.f);
				auto SpawnLocation3 = SpawnLocation2 + Helpers::GetRandomDirection2D() * SpawnRadius3;
				auto Spawn3 = SpawnObstacleEnvironmentActor(SpawnLocation3);
				auto Scale3 = FMath::RandRange(Scale2 / ScaleMax, Scale2 / ScaleMin);
				Spawn3->SetActorScale3D(FVector(Scale3));

				if (auto MovementComponent = Spawn3->FindComponentByClass<USpaceObjectMovementComponent>())
				{
					MovementComponent->InitializeOrbitalParent(Spawn2);
					MovementComponent->SetOrbitalDirection(ClockwiseTertiary);
					MovementComponent->SetOrbitalSpeed((MovementComponent->GetOrbitalSpeed() * 8.f) * GlobalOrbitSpeedMultiplier);
				}
			}
		}
	}
}

AActor* AMapManager::SpawnEnvironmentActor(const FVector& Location)
{
	auto SpawnClass = GetRandomEnvironmentActorClass();
	auto SpawnRotation = FRotator();
	auto Spawn = GetWorld()->SpawnActor(SpawnClass, &Location, &SpawnRotation);
	CurrentEnvironmentActors.Add(Spawn);
	return Spawn;
}

AActor* AMapManager::SpawnRootEnvironmentActor(const FVector& Location)
{
	auto SpawnClass = Helpers::GetRandomArrayElement(RootEnvironmentActorClasses);
	auto SpawnRotation = FRotator();
	auto Spawn = GetWorld()->SpawnActor(SpawnClass, &Location, &SpawnRotation);
	CurrentEnvironmentActors.Add(Spawn);
	return Spawn;
}

AActor* AMapManager::SpawnObstacleEnvironmentActor(const FVector& Location)
{
	auto SpawnClass = Helpers::GetRandomArrayElement(ObstacleEnvironmentActorClasses);
	auto SpawnRotation = FRotator();
	auto Spawn = GetWorld()->SpawnActor(SpawnClass, &Location, &SpawnRotation);
	CurrentEnvironmentActors.Add(Spawn);
	return Spawn;
}

void AMapManager::SetupNewEntry(const FVector& ExitDirection)
{
	EntryDirection = ExitDirection.GetSafeNormal();
	Entrance->SetActorLocation(Center + (-EntryDirection * (Radius - DoorFeathering)));
}

void AMapManager::ClearCurrentDoors()
{
	for (auto Door : CurrentDoors)
	{
		Door->Destroy();
	}
	CurrentDoors.Empty();
}

void AMapManager::SpawnRandomDoors(int32 NumberOfDoors)
{
	auto NoSpawnLocations = TArray<FVector>();
	NoSpawnLocations.Add(Entrance->GetActorLocation());

	for (auto i = 0; i < NumberOfDoors; i++)
	{
		auto Door = GetWorld()->SpawnActor<ADoor>(DoorClass);
		CurrentDoors.Add(Door);

		auto SpawnLocation = GetRandomPointOnEdge(DoorFeathering);

		auto IsBadSpawn = [&NoSpawnLocations](const auto& InLoc) {
			for (auto Location : NoSpawnLocations) {
				if (FVector::DistSquared(Location, InLoc) < 60000.f) {
					return true;
				}
			}
			return false;
		};

		static const auto Retries = 100;
		auto RetryCounter = 0;
		while (IsBadSpawn(SpawnLocation) && RetryCounter < Retries)
		{
			SpawnLocation = GetRandomPointOnEdge(DoorFeathering);
			RetryCounter++;
		}

		Door->SetActorLocation(SpawnLocation);
		NoSpawnLocations.Add(Door->GetActorLocation());
	}
}

void AMapManager::CleanupPreviousMap()
{
	ClearCurrentDoors();
	ClearCurrentEnvironmentActors();
}

void AMapManager::SpawnWalls()
{
	auto NumIslands = FMath::RandRange(MinIslands, MaxIslands);
	for (auto j = 0; j < NumIslands; j++)
	{
		auto WallLocation = GetRandomPointInsideMap() + (FVector::DownVector * 100.f);
		auto Wall = static_cast<AHexWall*>(nullptr);

		auto IslandSize = FMath::RandRange(MinIslandSize, MaxIslandSize);
		for (auto i = 0; i < IslandSize; i++)
		{
			auto Last = Wall;

			if (Last)
			{
				WallLocation = Last->GetAdjecentSlot(FMath::RandRange(0, 6));
			}

			if (IsLocationInsideMap(WallLocation) && FVector::DistSquared(WallLocation, Center) > (MinWallRange * MinWallRange))
			{
				Wall = GetWorld()->SpawnActor<AHexWall>(HexWallClass, WallLocation, FRotator());
				WallMatrix.Add(Wall->GetActorLocation(), Wall);
			}
		}
	}
}

UClass* AMapManager::GetRandomEnvironmentActorClass()
{
	return Helpers::GetRandomArrayElement(EnvironmentActorClasses);
}

void AMapManager::InitializeCircleVisualization()
{
	CenterVisualization->SetWorldLocation(Center);
	CircleVisualization->SetWorldLocation(Center);
	CosmicBackgroundVisualization->SetWorldLocation(Center);
	ArenaWalls->SetWorldLocation(Center);

	CosmicBackgroundVisualization->SetWorldScale3D(FVector(Radius * 100.f, Radius * 100.f, 1.f));
	CircleVisualization->SetWorldScale3D(FVector(Radius / 50.f, Radius / 50.f, 1.f));
	auto WallWidth = 0.01f;
	ArenaWalls->SetWorldScale3D(FVector((Radius / 100.f) + WallWidth, (Radius / 100.f) + WallWidth, 1.f));

	CosmicBackgroundVisualization->SetScalarParameterValueOnMaterials(FName("Map Radius"), Radius);
	CosmicBackgroundVisualization->SetVectorParameterValueOnMaterials(FName("Map Center"), Center);
}

FVector AMapManager::GetRandomPointOnEdge(float Feathering)
{
	return Center + (FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f).GetSafeNormal() * (Radius - Feathering));
}

void AMapManager::SetCosmicSoup(const FVector& Color)
{
	CosmicBackgroundVisualization->SetVectorParameterValueOnMaterials(FName("Color"), Color);
}

FVector AMapManager::GetNearestPointInsideMap(const FVector& Point, float Feathering)
{
	return Helpers::NearestPointInsideSphere(Point, Center, Radius - Feathering);
}

void AMapManager::OpenCurrentDoors()
{
	for (auto Door : CurrentDoors)
	{
		Door->Open();
	}
}

void AMapManager::CloseCurrentDoors()
{
	for (auto Door : CurrentDoors)
	{
		Door->Close();
	}
}

FVector AMapManager::GetRandomPointInsideMap()
{
	auto LocationXY = Helpers::GetRandomDirection2D() * FMath::RandRange(0.f, Radius);
	return Center + FVector(LocationXY.X, LocationXY.Y, CharacterHeight);
}

bool AMapManager::IsLocationInsideMap(const FVector& Location)
{
	return FVector::DistSquared(Location, Center) < Radius * Radius;
}

void AMapManager::ClearCurrentEnvironmentActors()
{
	for (auto Actor : CurrentEnvironmentActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	CurrentEnvironmentActors.Empty();

	for (const auto& Entry : WallMatrix)
	{
		Entry.Value->Destroy();
	}

	WallMatrix.Empty();
}