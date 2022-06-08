// Robbie Grier

#include "LevelUpChoice.h"
#include "Boon.h"

ALevelUpChoice::ALevelUpChoice()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelUpChoice::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelUpChoice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString ALevelUpChoice::GetChoiceName()
{
	return FString::Printf(TEXT("%s (Lvl %d -> Lvl %d)"), *Boon->GetChoiceName(), Boon->GetBoonLevel(), Boon->GetBoonLevel() + 1);
}

FString ALevelUpChoice::GetChoiceDescription()
{
	return Boon->GetLevelUpDescription();
}

void ALevelUpChoice::Choose(AActor* Chooser)
{
	static_cast<void>(Chooser);
	Boon->NativeLevelUp();
}