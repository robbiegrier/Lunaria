// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionApplyStatus.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UActionApplyStatus : public UAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		static void PerformApplyStatus(AActor* InAgent, AActor* InSubject, ATool* InTool, TSubclassOf<class ABoon> InStatusClass, float InDuration);

	virtual void Execute() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UClass* StatusClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Duration = 3.f;
};
