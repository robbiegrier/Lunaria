// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTask.generated.h"

DECLARE_EVENT(ALevelTask, ASignalTaskCompleteSignature)
UCLASS()
class LUNARIA_API ALevelTask : public AActor
{
	GENERATED_BODY()

public:
	ALevelTask();
	virtual void Tick(float DeltaTime) override;

	virtual void NativeStartTask();

	ASignalTaskCompleteSignature& GetTaskCompleteEvent() { return TaskCompleteEvent; }

	UFUNCTION(BlueprintCallable)
		void SignalTaskComplete();

protected:
	virtual void BeginPlay() override;

private:
	ASignalTaskCompleteSignature TaskCompleteEvent;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnStartTask();
};
