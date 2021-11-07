// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateBrush.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LUNARIA_API IInteractable
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FString GetInteractionText();
	virtual bool ShowButtonWidget() { return true; }

	virtual void Interact() = 0;
};
