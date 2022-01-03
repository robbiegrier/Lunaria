// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CpuHealthBar.h"
#include "DebugWidget.h"
#include "MapManager.h"
#include "LevelTask.h"
#include "AreaOfEffect.h"
#include "Archetype.h"
#include "PickupSelectionWidget.h"
#include "UpgradeManager.h"
#include "PickupChoiceWidget.h"
#include "Components/Button.h"
#include "SpawnIndicator.h"
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

	UFUNCTION(BlueprintCallable)
		class AUpgradeManager* GetUpgradeManager() const { return UpgradeManager; }

	UFUNCTION(BlueprintCallable)
		FLinearColor GetArchetypeColor(EArchetype Archetype);

	UFUNCTION(BlueprintCallable)
		FLinearColor GetGameColor(const FString& String);

	void StartNewArea();
	void StartNewAreaFromDoor(class ADoor* Door);

	UClass* GetDebugWidgetClass() const { return DebugWidgetClass; }
	UClass* GetHealthBarClass() const { return HealthBarClass; }
	UClass* GetAreaOfEffectClass() const { return AreaOfEffectClass; }
	UClass* GetPickupSelectionWidgetClass() const { return PickupSelectionWidgetClass; }
	UClass* GetPickupChoiceWidgetClass() const { return PickupChoiceWidgetClass; }
	UClass* GetPickupChoiceButtonWidgetClass() const { return PickupChoiceButtonWidgetClass; }

	void SetEventManager(AGameplayEventManager* InMananger) { EventManager = InMananger; }

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
	void OnAllTasksComplete();

	TArray<TSubclassOf<ALevelTask>> LevelTaskList;
	int32 CurrentTaskIndex = 0;
	ALevelTask* CurrentTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tasks, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<ALevelTask>> CombatTaskClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tasks, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<ALevelTask>> TestTasks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tasks, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ALevelTask> RoomFinishedTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Managers, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AMapManager> MapManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UDebugWidget> DebugWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UCpuHealthBar> HealthBarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Managers, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AUpgradeManager> UpgradeManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AAreaOfEffect> AreaOfEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UPickupSelectionWidget> PickupSelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UPickupChoiceWidget> PickupChoiceWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UButton> PickupChoiceButtonWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors, meta = (AllowPrivateAccess = "true"))
		TMap<EArchetype, FLinearColor> ArchetypeColorMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colors, meta = (AllowPrivateAccess = "true"))
		TMap<FString, FLinearColor> StringColorMap;

	class AMapManager* MapManager;
	class AGameplayEventManager* EventManager;
	class AUpgradeManager* UpgradeManager;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actors, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ASpawnIndicator> UnitSpawnIndicatorClass;
};
