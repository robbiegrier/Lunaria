// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FGameplayEvent.h"
#include "GameplayEventObserver.generated.h"

UINTERFACE(MinimalAPI)
class UGameplayEventObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LUNARIA_API IGameplayEventObserver
{
	GENERATED_BODY()

public:
	virtual void ExecuteAgentOf(const FGameplayEvent& Event);
	virtual void ExecuteSubjectOf(const FGameplayEvent& Event);

	virtual void NativeWhenAgentOf(const FGameplayEvent& Event) {}
	virtual void NativeWhenSubjectOf(const FGameplayEvent& Event) {}

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenAgentOf(const FGameplayEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenSubjectOf(const FGameplayEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenHitSomething(const FGameplayEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenKilledSomething(const FGameplayEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenHitBySomething(const FGameplayEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenKilledBySomething(const FGameplayEvent& Event);
};
