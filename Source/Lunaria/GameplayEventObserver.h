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
	virtual void ExecuteAgentOf(class UAction* Action);
	virtual void ExecuteSubjectOf(class UAction* Action);

	virtual void NativeWhenAgentOf(class UAction* Action) {}
	virtual void NativeWhenSubjectOf(class UAction* Action) {}

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenAgentOf(class UAction* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenSubjectOf(class UAction* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenHitSomething(class UActionHit* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenHitBySomething(class UActionHit* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenKilledSomething(class UActionDie* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void WhenKilledBySomething(class UActionDie* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenHitSomethingWithAttack(class UActionHit* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenHitSomethingWithSpecial(class UActionHit* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenHitSomethingWithMovement(class UActionHit* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenHitSomethingWithDefensive(class UActionHit* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenAttackUsed(class UActionUseAbility* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenSpecialUsed(class UActionUseAbility* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenMovementUsed(class UActionUseAbility* Action);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Action Events")
		void WhenDefensiveUsed(class UActionUseAbility* Action);
};
