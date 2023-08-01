// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Action.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class LUNARIA_API UAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute() {}

	// The actor, owned by the agent, that is responsible for the action
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* Tool = nullptr;

	// The subject of the action, if any
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* Subject = nullptr;

	// the agent of the action
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* Agent = nullptr;
};

UCLASS(BlueprintType)
class LUNARIA_API UActionHit : public UAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void PerformHit(AActor* InAgent, AActor* InSubject, AActor* InTool, class AMedium* InMedium);

	virtual void Execute() override;

	// The actor medium through which the hit occured
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AMedium* Medium = nullptr;

protected:
	float GetBaseDamage() const;
	float AddAgentDamage(float Damage) const;
	float AddSubjectDamage(float Damage) const;
};

UCLASS(BlueprintType)
class LUNARIA_API UActionTakeDamage : public UAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;

	UActionTakeDamage* With(AActor* InAgent, AActor* InSubject, AActor* InTool, float InDamage);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Damage;
};

UCLASS(BlueprintType)
class LUNARIA_API UActionDie : public UAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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
		static void PerformAreaOfEffect(AActor* InAgent, AActor* InTool, TSubclassOf<class AAreaOfEffect> InAreaOfEffectClass, const FVector& InLocation, float InDamage, float InRadius, float InDelay);

	virtual void Execute() override;

	UClass* AreaOfEffectClass;
	FVector Location;
	float Damage;
	float Radius;
	float Delay;
};