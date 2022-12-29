// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "SpaceObject.h"
#include "Interactable.h"
#include "Destination.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API ADestination : public ASpaceObject, public IInteractable
{
	GENERATED_BODY()

public:
	ADestination();
	virtual void Tick(float DeltaTime) override;
	virtual FString GetInteractionText() override;
	virtual void Interact() override;

	void SetDestinationName(const FString& InName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* NameWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Name, meta = (AllowPrivateAccess = "true"))
		FString DestinationName;
};
