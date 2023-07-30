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
#include "Ability.h"
#include "AbilitySlot.h"
#include "AbilitiesComponent.h"
#include "Action.h"

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

	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("HitStrategy.Projectile"));

	Mesh->SetCollisionProfileName("NoCollision");
	ProjectileShell->SetCollisionProfileName("NoCollision");
}

void ASpaceProjectile::SetColor(const FLinearColor& InColor)
{
	Color = InColor;
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Tint"), Helpers::GetVectorFromLinearColor(Color));
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), Helpers::GetVectorFromLinearColor(Color));
}

void ASpaceProjectile::SetAbilityCreatedFrom(AAbility* Ability)
{
	AbilityParent = Ability;
	auto ActorParent = Ability->GetMyOwner();

	if (ActorParent)
	{
		if (auto Attributes = ActorParent->FindComponentByClass<UAttributesComponent>())
		{
			auto TagContainer = FGameplayTagContainer();
			TagContainer.AddTag(Ability->GetAbilityTag());
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Attribute.Speed.Projectile"));

			auto SpeedModifier = Attributes->GetFromTagContainer(TagContainer, ProjectileMovementComponent->InitialSpeed);
			ProjectileMovementComponent->InitialSpeed = SpeedModifier;
			ProjectileMovementComponent->MaxSpeed = SpeedModifier;
		}
	}
}

void ASpaceProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(Handle, [this]() {
		if (this && this->IsValidLowLevel()) Die();
		}, SecondsUntilDespawn, false);

	StartingPoint = GetActorLocation();
}

void ASpaceProjectile::EndPlay(EEndPlayReason::Type Reason)
{
	if (AbilityParent)
	{
		AbilityParent->OnProjectileEnd(this);
	}
}

void ASpaceProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Don't execute collision behavior on the first frame
	if (SecondsUntilDespawn - GetLifeSpan() < GetWorld()->GetDeltaSeconds() * 2.f)
	{
		return;
	}

	if (!(GetOwner() == OtherActor) && !Cast<ASpaceProjectile>(OtherActor) && !IgnoreActors.Contains(OtherActor))
	{
		if (Helpers::AreDifferentTeams(GetOwner(), OtherActor))
		{
			if (AbilityParent)
			{
				auto Agent = AbilityParent->GetSlot()->GetParent()->GetOwner();
				UActionHit::PerformHit(Agent, OtherActor, AbilityParent, this);
			}
		}
		else
		{
			// When hit a friendly, do not die
		}
	}
}

void ASpaceProjectile::Die()
{
	Handle.Invalidate();
	Destroy();
}

void ASpaceProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((GetActorLocation() - StartingPoint).Size() > TravelDistance)
	{
		Die();
	}
}

void ASpaceProjectile::SetPayloadProperties(const FGameplayTagContainer& TagContainer, float InDamage, float InDistance, const FLinearColor& InColor)
{
	GameplayTags.AppendTags(TagContainer);
	DamagePayload = InDamage;
	TravelDistance = InDistance;
	ProjectileShell->SetCollisionProfileName("OverlapAllDynamic");
	SetColor(InColor);
}