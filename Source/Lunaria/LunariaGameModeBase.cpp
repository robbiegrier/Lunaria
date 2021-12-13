// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LunariaGameModeBase.h"
#include "User.h"
#include "Spaceship.h"
#include "MapManager.h"
#include "Components/StaticMeshComponent.h"
#include "Helpers.h"
#include "Printer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Door.h"
#include "GameplayEventManager.h"

ALunariaGameModeBase::ALunariaGameModeBase()
{
	PlayerControllerClass = AUser::StaticClass();
	DefaultPawnClass = ASpaceship::StaticClass();
}

void ALunariaGameModeBase::StartPlay()
{
	Super::StartPlay();

	StartNewArea();
}

void ALunariaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MapManager = Cast<AMapManager>(GetWorld()->SpawnActor(MapManagerClass));
	EventManager = GetWorld()->SpawnActor<AGameplayEventManager>();
	OnEventManagerSpawned(EventManager);
}

FLinearColor ALunariaGameModeBase::GetArchetypeColor(EArchetype Archetype)
{
	if (auto Find = ArchetypeColorMap.Find(Archetype))
	{
		return *Find;
	}

	return FLinearColor::Gray;
}

void ALunariaGameModeBase::StartNewArea()
{
	static auto TestScale = 1500.f;
	static auto TestEntDir = FVector(1.f, 1.f, 0.f);
	MapManager->LoadNewMap(TestScale, TestEntDir, 1);

	auto Nearest = Helpers::NearestPointInsideSphere(MapManager->GetEntrance()->GetActorLocation(), MapManager->GetCenter(), MapManager->GetRadius() - 100.f);
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(Nearest);
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorRotation(UKismetMathLibrary::MakeRotFromX(TestEntDir));

	StartTasks();
}

void ALunariaGameModeBase::StartNewAreaFromDoor(ADoor* Door)
{
	auto ExitDirection = (Door->GetActorLocation() - MapManager->GetCenter()).GetSafeNormal();
	MapManager->LoadNewMap(FMath::RandRange(1400.f, 1800.f), ExitDirection, FMath::RandRange(1, 8));

	if (auto Spaceship = Cast<ASpaceship>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Spaceship->RespawnOnMap(MapManager);
	}

	StartTasks();
}

void ALunariaGameModeBase::StartTasks()
{
	MapManager->CloseCurrentDoors();

	LevelTaskList.Empty();

	LevelTaskList.Add(Helpers::GetRandomArrayElement(CombatTaskClasses));
	//LevelTaskList.Add(Helpers::GetRandomArrayElement(CombatTaskClasses));

	CurrentTaskIndex = 0;
	StartTask(GetCurrentLevelTaskClass());
}

TSubclassOf<ALevelTask> ALunariaGameModeBase::GetCurrentLevelTaskClass() const
{
	if (Helpers::IsValidIndex(LevelTaskList, CurrentTaskIndex))
	{
		return LevelTaskList[CurrentTaskIndex];
	}

	return nullptr;
}

void ALunariaGameModeBase::StartTask(TSubclassOf<ALevelTask> TaskClass)
{
	if (TaskClass)
	{
		CurrentTask = GetWorld()->SpawnActor<ALevelTask>(TaskClass);
		CurrentTask->GetTaskCompleteEvent().AddUObject(this, &ALunariaGameModeBase::OnCurrentTaskComplete);
		CurrentTask->NativeStartTask();
	}
}

void ALunariaGameModeBase::StartNextTask()
{
	++CurrentTaskIndex;
	StartTask(GetCurrentLevelTaskClass());
}

void ALunariaGameModeBase::OnCurrentTaskComplete()
{
	Print("Completed a Task", FColor::Blue);
	CurrentTask->Destroy();

	if (CurrentTaskIndex < LevelTaskList.Num() - 1)
	{
		StartNextTask();
	}
	else
	{
		Print("All Tasks Complete", FColor::Green);
		MapManager->OpenCurrentDoors();
	}
}