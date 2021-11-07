// Robbie Grier

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class LUNARIA_API InteractableState
{
public:
	virtual ~InteractableState() = default;

	const FString& GetMessage() const { return Message; }

	virtual void Execute(class AActor* Subject) {}
	virtual bool OfferInteract() { return CanInteract; }

protected:
	FString Message;
	bool CanInteract = true;
};
