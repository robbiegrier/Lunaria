// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Stat.h"
#include "StatCommand.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class LUNARIA_API UStatCommand : public UObject
{
	GENERATED_BODY()
public:
	//virtual void Execute(class UAction* Action, FStatComposite& Composite) {}
};

UCLASS()
class LUNARIA_API UStatCommandHitDamage : public UStatCommand
{
	GENERATED_BODY()
public:
	//virtual void Execute(class UAction* Action, FStatComposite& Composite) override;
};
