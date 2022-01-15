// Robbie Grier

#include "CombatCpu.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Spaceship.h"
#include "CombatComponent.h"
#include "HealthComponent.h"
#include "Printer.h"
#include "SpawnIndicator.h"
#include "LunariaGameModeBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

ACombatCpu::ACombatCpu()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComponent"));
}

void ACombatCpu::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	Spaceship = Cast<ASpaceship>(MyPawn);
	checkf(Spaceship, TEXT("%s pawn is not a ASpaceship"), *GetFullName());

	//if (BehaviorTree)
	//{
	//	BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	//	BehaviorComponent->StartTree(*BehaviorTree);
	//}

	Spaceship->SetRutterScaleModifier(2.f);

	EnterSpawningState();
}

void ACombatCpu::MoveShipTo(AActor* Target)
{
	if (!Target) return;

	auto PointToMoveTo = Target->GetActorLocation();
	auto MovementDirection = (Target->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();

	Spaceship->HandleThrottleInput(1.f);
	Spaceship->TurnToFaceDirection(MovementDirection);
}

void ACombatCpu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatCpu::EnterSpawningState()
{
	if (BehaviorTree)
	{
		BehaviorComponent->StopTree();
	}

	Spaceship->EnterSpawningState();
	auto GameMode = Cast<ALunariaGameModeBase>(GetWorld()->GetAuthGameMode());

	auto SpawnIndicatorTransform = FTransform();
	SpawnIndicatorTransform.SetLocation(Spaceship->GetActorLocation() + FVector(0.f, 0.f, SpawnIndicatorOffset));
	SpawnIndicatorTransform.SetScale3D(FVector(Spaceship->GetCapsuleComponent()->GetScaledCapsuleRadius() / 30.f));
	auto SpawnIndicator = GetWorld()->SpawnActor<ASpawnIndicator>(GameMode->UnitSpawnIndicatorClass, SpawnIndicatorTransform);
	SpawnIndicator->Completion.AddUObject(this, &ACombatCpu::EnterCombatState);
}

void ACombatCpu::EnterCombatState()
{
	if (BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorComponent->StartTree(*BehaviorTree);
	}

	Spaceship->EnterCombatState();
}

FVector ACombatCpu::GetMoveFocus() const
{
	return GetPathFollowingComponent()->GetMoveFocus(false);
}

void ACombatCpu::HandleShipDeath(UHealthComponent* HealthComponent, int32 KillingBlow)
{
	Print(GetName() + " died with Killing Blow: " + FString::FromInt(KillingBlow));
	GetPawn()->Destroy();
}