// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGameplayEvent.h"
#include "GameplayEventManager.generated.h"

DECLARE_EVENT_OneParam(AGameplayEventManager, FGameplayEventSignature, const FGameplayEvent&)
UCLASS()
class LUNARIA_API AGameplayEventManager : public AActor
{
	GENERATED_BODY()

public:
	AGameplayEventManager();
	virtual void Tick(float DeltaTime) override;

	static AGameplayEventManager* Get(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable)
		void SubmitEvent(const FGameplayEvent& Event);

protected:
	virtual void BeginPlay() override;

private:
	void ProcessGameplayEvents();
	void TriggerSubjectObservation(const FGameplayEvent& Event);
	void TriggerAgentObservation(const FGameplayEvent& Event);
	void ProcessHitEvent(const FGameplayEvent& Event);
	void ProcessKillEvent(const FGameplayEvent& Event);

	UPROPERTY()
		TArray<FGameplayEvent> Events;
};
