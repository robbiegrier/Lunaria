// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Choosable.generated.h"

UINTERFACE(MinimalAPI)
class UChoosable : public UInterface
{
	GENERATED_BODY()
};

class LUNARIA_API IChoosable
{
	GENERATED_BODY()

public:
	virtual FString GetChoiceName() = 0;
	virtual FString GetChoiceDescription() = 0;

	// don't try to pass a beggar into this
	virtual void Choose(class AActor* Chooser) = 0;
};
