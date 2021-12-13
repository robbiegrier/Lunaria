// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Archetype.h"
#include "Interactable.h"
#include "Pickup.generated.h"

UCLASS()
class LUNARIA_API APickup : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	APickup();
	virtual void Tick(float DeltaTime) override;
	virtual FString GetInteractionText();
	virtual void Interact();

	UFUNCTION(BlueprintCallable)
		EArchetype GetArchetype() const { return Archetype; }

	UFUNCTION(BlueprintCallable)
		void SetArchetype(EArchetype InArchetype);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		EArchetype Archetype;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* Shell;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;
};
