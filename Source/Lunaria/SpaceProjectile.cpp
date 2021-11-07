// Robbie Grier

#include "SpaceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Printer.h"
#include "TimerManager.h"
#include "Spaceship.h"
#include "Hittable.h"

ASpaceProjectile::ASpaceProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileShell = CreateDefaultSubobject<USphereComponent>(TEXT("Shell"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	RootComponent = ProjectileShell;
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = 1500.f;
	ProjectileMovementComponent->MaxSpeed = 1500.f;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
}

void ASpaceProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(Handle, [this]() {
		if (this && this->IsValidLowLevel()) this->ConditionalBeginDestroy();
	}, SecondsUntilDespawn, false);

	StartingPoint = GetActorLocation();
}

void ASpaceProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto IsSameOwnersProjectile = OtherActor->GetOwner() == GetOwner() && Cast<ASpaceProjectile>(OtherActor);
	if (!(GetOwner() == OtherActor) && !IsSameOwnersProjectile)
	{
		if (auto Hittable = Cast<IHittable>(OtherActor))
		{
			Hittable->TakeProjectileHit(this);
		}

		Die();
	}
}

void ASpaceProjectile::Die()
{
	Handle.Invalidate();
	ConditionalBeginDestroy();
}

void ASpaceProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetActorLocation() - StartingPoint).Size() > TravelDistance)
	{
		Die();
	}
}