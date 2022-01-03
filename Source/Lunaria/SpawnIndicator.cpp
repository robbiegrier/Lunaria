// Robbie Grier

#include "SpawnIndicator.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ASpawnIndicator::ASpawnIndicator()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = SceneComponent;
	Mesh->SetupAttachment(RootComponent);
}

void ASpawnIndicator::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Duration);
	SpawnTime = GetWorld()->GetTimeSeconds();
}

void ASpawnIndicator::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	Completion.Broadcast();
}

void ASpawnIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto TimePassed = GetWorld()->GetTimeSeconds() - SpawnTime;
	Mesh->SetScalarParameterValueOnMaterials(TEXT("Age"), TimePassed);
}