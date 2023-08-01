// Robbie Grier

#include "Medium.h"
#include "Engine/World.h"
#include "Ability.h"

AMedium* AMedium::Spawn(UClass* Class, AActor* Agent, AActor* Tool, FTransform Transform, float BaseDamage)
{
	auto Params = FActorSpawnParameters();
	Params.Owner = Agent;

	auto MediumSpawn = Agent->GetWorld()->SpawnActor<AMedium>(Class, Transform, Params);
	MediumSpawn->SetAgent(Agent);
	MediumSpawn->SetTool(Tool);
	MediumSpawn->SetBaseDamage(BaseDamage);

	if (auto Ability = Cast<AAbility>(Tool))
	{
		MediumSpawn->SetColor(Ability->GetAbilityColor());
	}

	return MediumSpawn;
}

AMedium::AMedium()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMedium::BeginPlay()
{
	Super::BeginPlay();
}

void AMedium::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}