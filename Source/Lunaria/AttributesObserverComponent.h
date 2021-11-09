// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Hittable.h"
#include "AttributesObserverComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UAttributesObserverComponent : public UActorComponent, public IHittable
{
	GENERATED_BODY()

public:
	UAttributesObserverComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		virtual void NotifyAttributesUpdated(class UAttributesComponent* Attributes) {}

protected:
	virtual void BeginPlay() override;
};
