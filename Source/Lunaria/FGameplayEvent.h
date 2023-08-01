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
	AbilityUsed,
	Block,
	Heal,
	TakeDamage
};

USTRUCT(BlueprintType)
struct FGameplayEvent
{
	GENERATED_BODY()

public:
	FGameplayEvent() = default;

	FGameplayEvent(AActor* InAgent, ENativeEventType InAction, AActor* InSubject, AActor* InTool = nullptr, AActor* InMedium = nullptr)
		: Agent(InAgent), Action(InAction), Subject(InSubject), Medium(InMedium), Tool(InTool) {
		if (Agent) {
			if (auto Tagged = Cast<IGameplayTagAssetInterface>(Agent)) {
				Tagged->GetOwnedGameplayTags(AgentTags);
			}
		}
		if (Subject) {
			if (auto Tagged = Cast<IGameplayTagAssetInterface>(Subject)) {
				Tagged->GetOwnedGameplayTags(SubjectTags);
			}
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Agent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTagContainer AgentTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENativeEventType Action = ENativeEventType::Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Subject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Medium = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Tool = nullptr;

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