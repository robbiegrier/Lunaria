// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "FGameplayEvent.generated.h"

UENUM(BlueprintType)
enum class ENativeEventType : uint8
{
	Hit,
	Kill,
	ApplyStatus,
	AbilityUsed
};

USTRUCT(BlueprintType)
struct FGameplayEvent
{
	GENERATED_BODY()

public:
	FGameplayEvent() = default;

	FGameplayEvent(AActor* InAgent, ENativeEventType InAction, AActor* InSubject)
		: Agent(InAgent), Action(InAction), Subject(InSubject) {
		if (auto Tagged = Cast<IGameplayTagAssetInterface>(Agent)) {
			Tagged->GetOwnedGameplayTags(AgentTags);
		}
		if (auto Tagged = Cast<IGameplayTagAssetInterface>(Subject)) {
			Tagged->GetOwnedGameplayTags(SubjectTags);
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTagContainer AgentTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENativeEventType Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Subject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTagContainer SubjectTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, float> Values;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FVector> Vectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, UClass*> Classes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTagContainer EventTags;
};