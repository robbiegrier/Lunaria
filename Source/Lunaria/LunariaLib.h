// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Spaceship.h"
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

	UFUNCTION(BlueprintCallable)
		static FGameplayTagContainer AddTagContainers(const FGameplayTagContainer& A, const FGameplayTagContainer& B);

	UFUNCTION(BlueprintCallable)
		static bool AreDifferentTeams(class AActor* Left, class AActor* Right);

	UFUNCTION(BlueprintCallable)
		static bool AreSameTeam(class AActor* Left, class AActor* Right);

	UFUNCTION(BlueprintCallable)
		static FGameplayTagContainer GetTags(UObject* Asset);

	UFUNCTION(BlueprintCallable)
		static class ASpaceship* Summon(AActor* Summoner, TSubclassOf<ASpaceship> SummonClass, const FTransform& Transform);

	UFUNCTION(BlueprintCallable)
		static AActor* GetClosestActorToLocation(const FVector& Location, const TArray<AActor*>& Array, AActor* Ignore = nullptr);

	// Get the actors that are on the team
	UFUNCTION(BlueprintCallable)
		static TArray<AActor*> FilterActorsByTeam(const TArray<AActor*>& Array, int32 Team);

	// Get the actors on any other team
	UFUNCTION(BlueprintCallable)
		static TArray<AActor*> FilterActorsByTeamExclusive(const TArray<AActor*>& Array, int32 Team);

	// Get the rival team
	UFUNCTION(BlueprintCallable)
		static int32 GetRivalTeam(int32 Team);
};
