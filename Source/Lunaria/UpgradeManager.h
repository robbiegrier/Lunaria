// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boon.h"
#include "Archetype.h"
#include "UpgradeNode.h"
#include "UpgradeManager.generated.h"

UCLASS()
class LUNARIA_API AUpgradeManager : public AActor
{
	GENERATED_BODY()

public:
	AUpgradeManager();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		TArray<AActor*> GetChoicesFromPickup(class APickup* Pickup);

	UFUNCTION(BlueprintCallable)
		EArchetype GetCurrentRoomArchetype() const { return CurrentRoomArchetype; };

	UFUNCTION(BlueprintCallable)
		void SetCurrentRoomArchetype(EArchetype Archetype) { CurrentRoomArchetype = Archetype; };

protected:
	TArray<UClass*> GetChoiceClassesFromPickup(class APickup* Pickup, int32 Num, class UAttributesComponent* Attributes);
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tasks, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<AActor>> PotentialChoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Upgrades, meta = (AllowPrivateAccess = "true"))
		TArray<FUpgradeNode> UpgradeNodes;

	EArchetype CurrentRoomArchetype;
};
