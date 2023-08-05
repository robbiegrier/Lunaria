// Robbie Grier

#include "SpaceshipWeaponComponent.h"
#include "SpaceProjectile.h"
#include "Printer.h"
#include "Engine/World.h"
#include "Spaceship.h"
#include "AttributesComponent.h"
#include "Components/SphereComponent.h"
#include "Helpers.h"
#include "GameFramework/ProjectileMovementComponent.h"

USpaceshipWeaponComponent::USpaceshipWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpaceshipWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	Attributes = Helpers::GetSister<UAttributesComponent>(this);
}

void USpaceshipWeaponComponent::FireWeapon()
{
	if (!GetWorld() || !ProjectileClass || !GetOwner() || !TargetingVolume) return;

	auto Projectile = GetWorld()->SpawnActorDeferred<ASpaceProjectile>(ProjectileClass,
		GetComponentTransform(), GetOwner(), GetOwner()->GetInstigator());

	InitializeProjectileBeforeSpawn(Projectile);
	Projectile->FinishSpawning(GetComponentTransform());

	OnWeaponFired();
}

void USpaceshipWeaponComponent::InitializeProjectileBeforeSpawn(ASpaceProjectile* Projectile)
{
	//Projectile->SetDamage(Attributes->GetAttackDamage());

	TArray<AActor*> Actors;
	TargetingVolume->GetOverlappingActors(Actors, UHittable::StaticClass());
	auto Closest = Helpers::GetClosestToLocation(GetOwner()->GetActorLocation(), Actors, GetOwner());

	if (Closest)
	{
		Projectile->GetProjectileMovement()->HomingTargetComponent = Closest->GetRootComponent();
	}
}

void USpaceshipWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}