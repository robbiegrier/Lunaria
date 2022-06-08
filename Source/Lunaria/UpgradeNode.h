// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Boon.h"
#include "Archetype.h"
#include "UpgradeNode.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct LUNARIA_API FUpgradeNode
{
	GENERATED_USTRUCT_BODY()

public:
	// The Boon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABoon> BoonClass;

	// The Archetype of The Boon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node, meta = (AllowPrivateAccess = "true"))
		EArchetype Archetype;

	// The Prerequisites that enable this. Only need one, so it is an "Enabler"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<ABoon>> Enablers;
};
