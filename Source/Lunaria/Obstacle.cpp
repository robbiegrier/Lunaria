// Robbie Grier

#include "Obstacle.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "AttributesComponent.h"
#include "HealthComponent.h"
#include "SpaceObjectMovementComponent.h"
#include "BobbleMovementComponent.h"
#include "DebugWidget.h"
#include "DrawDebugHelpers.h"
#include "LunariaGameModeBase.h"
#include "Printer.h"
#include "CombatComponent.h"
#include "Helpers.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget Component"));
	HealthBarComponent->SetupAttachment(GetSphereComponent());

	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarComponent->SetDrawAtDesiredSize(true);

	GetMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetTeam(UCombatComponent::NeutralTeam);
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	Helpers::BindHealthBar(this, HealthBarComponent);
}

void AObstacle::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor->GetClass() == GetClass()) return;

	if (OtherActor)
	{
		if (auto Hittable = Cast<IHittable>(OtherActor))
		{
			Hittable->TakeHit(CollisionDamage, this);
		}
	}

	TakeHit(SelfDamageOnCollision, this);
}

void AObstacle::HandleObstacleDestroyed(UHealthComponent* HealthComp, int KillingBlow)
{
	Destroy();
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HealthBarComponent->GetUserWidgetObject()->SetRenderScale(FVector2D(GetActorScale3D().X / 2.f));

	HealthBarComponent->SetWorldLocation(GetActorLocation() + FVector(GetSphereComponent()->GetScaledSphereRadius(), 0, GetSphereComponent()->GetScaledSphereRadius()));
}

void AObstacle::HandleCollisionRecognized(AActor* OtherActor)
{
}