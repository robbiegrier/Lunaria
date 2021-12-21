// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradeManager.generated.h"

UCLASS()
class LUNARIA_API AUpgradeManager : public AActor
{
	GENERATED_BODY()

public:
	AUpgradeManager();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		TArray<UClass*> GetChoicesFromPickup(class APickup* Pickup);

	UFUNCTION(BlueprintCallable)
		EArchetype GetCurrentRoomArchetype() const { return CurrentRoomArchetype; };

	UFUNCTION(BlueprintCallable)
		void SetCurrentRoomArchetype(EArchetype Archetype) { CurrentRoomArchetype = Archetype; };

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tasks, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<AActor>> PotentialChoices;

	EArchetype CurrentRoomArchetype;
};
