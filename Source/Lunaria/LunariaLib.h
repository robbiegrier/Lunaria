// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LunariaLib.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API ULunariaLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void Test() {}
};
