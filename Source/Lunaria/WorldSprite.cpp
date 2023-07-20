// Robbie Grier

#include "WorldSprite.h"
#include "Components/StaticMeshComponent.h"

AWorldSprite::AWorldSprite()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(SceneRoot);
	Mesh->SetupAttachment(SceneRoot);

	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetGenerateOverlapEvents(false);
}

void AWorldSprite::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldSprite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}