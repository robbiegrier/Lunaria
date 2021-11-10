// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEventManager.generated.h"

USTRUCT(BlueprintType)
struct FGameplayEvent
{
	GENERATED_BODY()

public:
	FGameplayEvent() = default;

	FGameplayEvent(AActor* InAgent, const FString& InType, AActor* InSubject)
		: Agent(InAgent), EventType(InType), Subject(InSubject) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString EventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Subject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, float> Values;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FVector> Vectors;
};

DECLARE_EVENT_OneParam(AGameplayEventManager, FGameplayEventSignature, const FGameplayEvent&)
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

	UFUNCTION(BlueprintCallable)
		void WhenOneToMany(AActor* Agent, const FString& Action, UClass* SubjectClass, UObject* Object, const FName& FuncName);

	UFUNCTION(BlueprintCallable)
		void WhenManyToOne(UClass* AgentClass, const FString& Action, AActor* Subject, UObject* Object, const FName& FuncName);

protected:
	virtual void BeginPlay() override;

private:
	void ProcessGameplayEvents();
	void ProcessHitEvent(const FGameplayEvent& Event);
	void ProcessKillEvent(const FGameplayEvent& Event);
	void TriggerDelegates(const FGameplayEvent& Event);

	void TriggerOneToManyDelegates(AActor* Agent, const FString& Action, UClass* SubjectClass, const FGameplayEvent& Event);
	void TriggerManyToOneDelegates(UClass* AgentClass, const FString& Action, AActor* Subject, const FGameplayEvent& Event);

	TMap<UClass*, FGameplayEventSignature>& GetOneToManyClassMap(AActor* Agent, const FString& Action);

	FGameplayEventSignature Tmp;

	UPROPERTY()
		TArray<FGameplayEvent> Events;

	TMap<UObject*, TMap<FString, TMap<UClass*, FGameplayEventSignature>>> OneToManyMap;
	TMap<UClass*, TMap<FString, TMap<UObject*, FGameplayEventSignature>>> ManyToOneMap;

	template<typename CallableType>
	void WithOneToMany(AActor* Agent, const FString& Action, UClass* SubjectClass, CallableType Callable) {
		auto& Map = GetOneToManyClassMap(Agent, Action);
		for (auto& Pair : Map) {
			auto PairClass = Pair.Key;
			if (SubjectClass->IsChildOf(PairClass)) {
				Callable(Pair.Value);
			}
		}
	}

	template<typename CallableType>
	void WithManyToOne(UClass* AgentClass, const FString& Action, AActor* Subject, CallableType Callable) {
		auto Find1 = ManyToOneMap.Find(AgentClass);

		if (!Find1) {
			ManyToOneMap.Add(AgentClass, TMap<FString, TMap<UObject*, FGameplayEventSignature>>());
		}

		for (auto& Pair : ManyToOneMap) {
			auto PairClass = Pair.Key;
			if (AgentClass->IsChildOf(PairClass)) {
				auto& ActionMap = Pair.Value;

				auto Find2 = ActionMap.Find(Action);

				if (!Find2) {
					ActionMap.Add(Action, TMap<UObject*, FGameplayEventSignature>());
				}

				auto& SubjectMap = *ActionMap.Find(Action);

				auto Find3 = SubjectMap.Find(Subject);

				if (!Find3) {
					SubjectMap.Add(Subject, FGameplayEventSignature());
				}

				auto& Delegate = *SubjectMap.Find(Subject);
				Callable(Delegate);
			}
		}
	}
};
