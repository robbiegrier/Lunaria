// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEventManager.generated.h"

UENUM(BlueprintType)
enum class EGameplayEventType : uint8
{
	Hit,
	Kill
};

USTRUCT(BlueprintType)
struct FGameplayEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Subject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Medium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameplayEventType EventType;
};

UCLASS()
class LUNARIA_API AGameplayEventManager : public AActor
{
	GENERATED_BODY()

public:
	AGameplayEventManager();
	virtual void Tick(float DeltaTime) override;

	static AGameplayEventManager* Get(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable)
		void SubmitEvent(const FGameplayEvent& Event);

protected:
	virtual void BeginPlay() override;

private:
	void ProcessGameplayEvents();
	void ProcessHitEvent(const FGameplayEvent& Event);
	void ProcessKillEvent(const FGameplayEvent& Event);

	UPROPERTY()
		TArray<FGameplayEvent> Events;
};
