// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorState.h"
#include "BehaviorStateComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UBehaviorStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBehaviorStateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetState(TSubclassOf<UBehaviorState> StateClass);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		UBehaviorState* CurrentState;
};
