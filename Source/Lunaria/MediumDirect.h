// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Medium.h"
#include "MediumDirect.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API AMediumDirect : public AMedium
{
	GENERATED_BODY()

public:
	virtual float NativeGetBaseDamage() const override;
};
