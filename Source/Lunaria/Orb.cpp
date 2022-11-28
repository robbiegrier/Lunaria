// Robbie Grier

#include "Orb.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Helpers.h"
#include "Printer.h"

AOrb::AOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	Shell = CreateDefaultSubobject<USphereComponent>(TEXT("Shell Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	RootComponent = Shell;
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionProfileName("NoCollision");
	Shell->SetCollisionProfileName("NoCollision");
}

void AOrb::BeginPlay()
{
	Super::BeginPlay();
}

void AOrb::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor == Target)
	{
		OnReachedTarget();
	}
}

void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		AddActorWorldOffset((Target->GetActorLocation() - GetActorLocation()).GetSafeNormal() * Helpers::Dilate(Speed * 10.f, GetWorld()));
	}
}

void AOrb::Launch(AActor* InTarget)
{
	Target = InTarget;
}