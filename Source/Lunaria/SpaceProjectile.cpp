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

void ASpaceProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(Handle, [this]() {
		if (this && this->IsValidLowLevel()) Die();
	}, SecondsUntilDespawn, false);

	StartingPoint = GetActorLocation();
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
			if (OtherActor->FindComponentByClass<UCombatComponent>())
			{
				auto Event = FGameplayEvent(GetOwner(), ENativeEventType::Hit, OtherActor);
				Event.Values.Add("Damage", DamagePayload);
				Event.Values.Add("Bounces", Bounces - 1);
				Event.Classes.Add("ProjectileClass", GetClass());
				Event.EventTags.AddTag(FGameplayTag::RequestGameplayTag("HitStrategy.Projectile"));
				Event.EventTags.AppendTags(GameplayTags);
				AGameplayEventManager::Get(GetWorld())->SubmitEvent(Event);
			}

			if (OtherActor->Implements<UHittable>())
			{
				IHittable::Execute_OnHitByProjectile(OtherActor, this);
			}

			OnHitBeforeDestroy(OtherActor);
			Die();
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