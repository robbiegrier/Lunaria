// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DetailTogglable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDetailTogglable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Objects that can be detail toggled. Actors will be toggled automagically when teh toggle key is pressed.
 */
class LUNARIA_API IDetailTogglable
{
	GENERATED_BODY()

public:
	virtual void SetInitialDetailToggleState();
	virtual void ToggleDetailOn() = 0;
	virtual void ToggleDetailOff() = 0;
};
