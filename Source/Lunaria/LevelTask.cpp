// Robbie Grier

#include "LevelTask.h"

ALevelTask::ALevelTask()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelTask::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelTask::NativeStartTask()
{
	OnStartTask();
}

void ALevelTask::SignalTaskComplete()
{
	TaskCompleteEvent.Broadcast();
}