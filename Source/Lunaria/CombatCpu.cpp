// Robbie Grier

#include "CombatCpu.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Spaceship.h"
#include "CombatComponent.h"
#include "HealthComponent.h"
#include "Printer.h"

ACombatCpu::ACombatCpu()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void ACombatCpu::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	if (auto Spaceship = Cast<ASpaceship>(GetPawn()))
	{
		//Spaceship->GetHealthComponent()->GetHealthDepletedEvent().AddUObject(this, &ACombatCpu::HandleShipDeath);
	}

	if (BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorComponent->StartTree(*BehaviorTree);
	}
}

void ACombatCpu::MoveShipTo(AActor* Target)
{
	if (!Target) return;

	if (auto Spaceship = Cast<ASpaceship>(GetPawn()))
	{
		auto PointToMoveTo = Target->GetActorLocation();
		auto MovementDirection = (Target->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();

		Spaceship->HandleThrottleInput(1.f);
		Spaceship->TurnToFaceDirection(MovementDirection);
	}
}

void ACombatCpu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatCpu::HandleShipDeath(UHealthComponent* HealthComponent, int32 KillingBlow)
{
	Print(GetName() + " died with Killing Blow: " + FString::FromInt(KillingBlow));
	GetPawn()->Destroy();
}