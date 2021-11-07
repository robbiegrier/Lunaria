// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeList.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct LUNARIA_API FAttributeList
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TurnSpeed;
};
