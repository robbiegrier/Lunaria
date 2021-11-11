// Robbie Grier

#include "SpaceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Printer.h"
#include "TimerManager.h"
#include "Spaceship.h"
#include "Hittable.h"
#include "CombatComponent.h"
#include "Helpers.h"
#include "GameplayEventManager.h"
#include "AttributesComponent.h"

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
	if (!(GetOwner() == OtherActor) && !Cast<ASpaceProjectile>(OtherActor))
	{
		if (auto OtherAsHittable = Cast<IHittable>(OtherActor))
		{
			if (Helpers::AreDifferentTeams(GetOwner(), OtherActor))
			{
				auto Event = FGameplayEvent(GetOwner(), ENativeEventType::Hit, OtherActor);
				Event.Tags.Add("AbilityType", AbilityAssociation);
				Event.Values.Add("Damage", DamagePayload);
				Event.Vectors.Add("Location", GetActorLocation());
				AGameplayEventManager::Get(GetWorld())->SubmitEvent(Event);
			}
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