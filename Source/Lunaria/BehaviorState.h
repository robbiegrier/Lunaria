// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorState.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LUNARIA_API UBehaviorState : public UObject
{
	GENERATED_BODY()

public:
	UBehaviorState();

	void Initialize(class UBehaviorStateComponent* InStateMachine);

	virtual void NativeEnter() { Enter(); }
	virtual void NativeExit() { Exit(); }
	virtual void NativeExecute(float DeltaTime) { Execute(DeltaTime); }

protected:
	UFUNCTION(BlueprintCallable)
		class ACombatCpu* GetOwner();

private:
	class UBehaviorStateComponent* StateMachine;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void Enter();
	UFUNCTION(BlueprintImplementableEvent)
		void Exit();
	UFUNCTION(BlueprintImplementableEvent)
		void Execute(float DeltaTime);
};
