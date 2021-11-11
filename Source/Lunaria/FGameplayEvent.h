// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "FGameplayEvent.generated.h"

UENUM(BlueprintType)
enum class ENativeEventType : uint8
{
	Hit,
	Kill,
	MovementStart,
	MovementEnd,
	DefenseStart,
	DefenseEnd,
	DefenseBlock,
	Summon,
	Heal,
	ApplyStatus
};

USTRUCT(BlueprintType)
struct FGameplayEvent
{
	GENERATED_BODY()

public:
	FGameplayEvent() = default;

	FGameplayEvent(AActor* InAgent, const FString& InType, AActor* InSubject)
		: Agent(InAgent), EventType(InType), Subject(InSubject) {}

	FGameplayEvent(AActor* InAgent, ENativeEventType InAction, AActor* InSubject)
		: Agent(InAgent), Action(InAction), Subject(InSubject) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENativeEventType Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Subject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, float> Values;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FVector> Vectors;
};