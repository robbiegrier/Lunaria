// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Action.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute() {}

	// The actor, owned by the agent, that is responsible for the action
	AActor* Tool = nullptr;

	// The subject of the action, if any
	AActor* Subject = nullptr;

	// the agent of the action
	AActor* Agent = nullptr;
};

UCLASS(BlueprintType)
class LUNARIA_API UActionHit : public UAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void PerformHit(AActor* Agent, AActor* Subject, AActor* Tool, AActor* Medium);

	virtual void Execute() override;

	// The actor medium through which the hit occured
	AActor* Medium = nullptr;
};

UCLASS()
class LUNARIA_API UActionTakeDamage : public UAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;

	float Damage;
};

UCLASS()
class LUNARIA_API UActionDie : public UAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;

	float DamageOfKillingBlow;
};

UCLASS(BlueprintType)
class LUNARIA_API UActionUseAbility : public UAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
};

UCLASS(BlueprintType)
class LUNARIA_API UActionCreateAreaOfEffect : public UAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void PerformAreaOfEffect(AActor* Agent, AActor* Tool, TSubclassOf<class AAreaOfEffect> AreaOfEffectClass, const FVector& Location, float Damage, float Radius, float Delay);

	virtual void Execute() override;

	UClass* AreaOfEffectClass;
	FVector Location;
	float Damage;
	float Radius;
	float Delay;
};