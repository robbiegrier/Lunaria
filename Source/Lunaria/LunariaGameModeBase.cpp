// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LunariaGameModeBase.h"
#include "User.h"
#include "Spaceship.h"
#include "MapManager.h"
#include "Components/StaticMeshComponent.h"
#include "Helpers.h"
#include "Printer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Door.h"
#include "GameplayEventManager.h"
#include "TimerManager.h"

ALunariaGameModeBase::ALunariaGameModeBase()
{
	PlayerControllerClass = AUser::StaticClass();
	DefaultPawnClass = ASpaceship::StaticClass();
	ValidArchetypes = TArray<EArchetype>({ EArchetype::Alpha, EArchetype::Omega });
}

void ALunariaGameModeBase::StartPlay()
{
	Super::StartPlay();

	UpgradeManager->SetCurrentRoomArchetype(GetRandomArchetype());
	StartSpecificArea(FString("Home"));
}

void ALunariaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MapManager = Cast<AMapManager>(GetWorld()->SpawnActor(MapManagerClass));
	EventManager = GetWorld()->SpawnActor<AGameplayEventManager>();
	OnEventManagerSpawned(EventManager);

	UpgradeManager = GetWorld()->SpawnActor<AUpgradeManager>(UpgradeManagerClass);
}

FLinearColor ALunariaGameModeBase::GetArchetypeColor(EArchetype Archetype)
{
	if (auto Find = ArchetypeColorMap.Find(Archetype))
	{
		return *Find;
	}

	return FLinearColor::Gray;
}

FLinearColor ALunariaGameModeBase::GetGameColor(const FString& String)
{
	if (auto Find = StringColorMap.Find(String))
	{
		return *Find;
	}

	return FLinearColor::Gray;
}

EArchetype ALunariaGameModeBase::GetRandomArchetype()
{
	auto Random = FMath::RandRange(0, ValidArchetypes.Num() - 1);
	return ValidArchetypes[Random];
}

void ALunariaGameModeBase::OnPlayerDeath()
{
	auto Ship = Cast<ASpaceship>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Ship->StartPlayerDeath();
}

void ALunariaGameModeBase::StartNewArea(const FVector& EntryDirection)
{
	MapManager->LoadNewMap(FMath::RandRange(MinMapRadius, MaxMapRadius), EntryDirection, FMath::RandRange(1, 8));
	RespawnPlayer();
	StartTasks();
}

void ALunariaGameModeBase::StartNewAreaFromDoor(ADoor* Door)
{
	if (Door->ContainsMapDescription())
	{
		StartSpecificAreaFromDescription(Door->GetMapDescription());
	}
	else
	{
		auto ExitDirection = (Door->GetActorLocation() - MapManager->GetCenter()).GetSafeNormal();
		UpgradeManager->SetCurrentRoomArchetype(GetRandomArchetype());
		StartNewArea(ExitDirection);
	}
}

void ALunariaGameModeBase::StartSpecificArea(const FString& AreaName)
{
	if (auto Find = MapDescriptions.Find(AreaName))
	{
		StartSpecificAreaFromDescription(*Find);
	}
}

void ALunariaGameModeBase::StartSpecificAreaFromDescription(const FMapDescription& Description)
{
	MapManager->LoadMapFromDescription(Description);
	RespawnPlayer();

	if (Description.LevelTasks.Num() > 0)
	{
		StartTasks();
	}
	else
	{
		OnAllTasksComplete();
	}
}

void ALunariaGameModeBase::StartTasks()
{
	CurrentLevel += 1;

	MapManager->CloseCurrentDoors();

	LevelTaskList.Empty();

	//LevelTaskList.Add(Helpers::GetRandomArrayElement(CombatTaskClasses));
	//LevelTaskList.Add(Helpers::GetRandomArrayElement(CombatTaskClasses));

	for (auto Task : TestTasks)
	{
		LevelTaskList.Add(Task);
	}

	LevelTaskList.Add(RoomFinishedTask);

	if (LevelTaskList.Num() < 1)
	{
		OnAllTasksComplete();
	}
	else
	{
		CurrentTaskIndex = 0;
		StartTask(GetCurrentLevelTaskClass());
	}
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
	//Print("Completed a Task", FColor::Blue);
	CurrentTask->Destroy();

	if (CurrentTaskIndex < LevelTaskList.Num() - 1)
	{
		StartNextTask();
	}
	else
	{
		OnAllTasksComplete();
	}
}

void ALunariaGameModeBase::OnAllTasksComplete()
{
	//Print("All Tasks Complete", FColor::Green);
	MapManager->OpenCurrentDoors();
}

void ALunariaGameModeBase::RespawnPlayer()
{
	if (auto Spaceship = Cast<ASpaceship>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Spaceship->RespawnOnMap(MapManager);
	}
}