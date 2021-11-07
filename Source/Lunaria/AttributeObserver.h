// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeObserver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttributeObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LUNARIA_API IAttributeObserver
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void NotifyAttributesUpdated(class UAttributesComponent* Attributes) {}

	UFUNCTION()
		virtual void SetAttributes(class UAttributesComponent* InAttributes);
};
