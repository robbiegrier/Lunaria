// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CpuHealthBar.h"
#include "DebugWidget.h"
#include "MapManager.h"
#include "LevelTask.h"
#include "LunariaGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API ALunariaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALunariaGameModeBase();

	static ALunariaGameModeBase* Get(class UWorld* World) { return Cast<ALunariaGameModeBase>(World->GetAuthGameMode()); }

	UFUNCTION(BlueprintCallable)
		class AMapManager* GetMap() const { return MapManager; }

	UFUNCTION(BlueprintCallable)
		class AGameplayEventManager* GetEventManager() const { return EventManager; }

	void SetEventManager(AGameplayEventManager* InMananger) { EventManager = InMananger; }

	void StartNewArea();
	void StartNewAreaFromDoor(class ADoor* Door);

	UClass* GetDebugWidgetClass() const { return DebugWidgetClass; }
	UClass* GetHealthBarClass() const { return HealthBarClass; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnEventManagerSpawned(class AGameplayEventManager* Manager);

protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

private:
	void StartTasks();
	TSubclassOf<ALevelTask> GetCurrentLevelTaskClass() const;
	void StartTask(TSubclassOf<ALevelTask> TaskClass);
	void StartNextTask();
	void OnCurrentTaskComplete();
	TArray<TSubclassOf<ALevelTask>> LevelTaskList;
	int32 CurrentTaskIndex = 0;
	ALevelTask* CurrentTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tasks, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<ALevelTask>> CombatTaskClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AMapManager> MapManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UDebugWidget> DebugWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Map, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UCpuHealthBar> HealthBarClass;

	class AMapManager* MapManager;
	class AGameplayEventManager* EventManager;
};
