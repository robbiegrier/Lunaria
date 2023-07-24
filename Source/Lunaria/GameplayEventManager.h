// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGameplayEvent.h"
#include "DetailTogglable.h"
#include "Boon.h"
#include "GameplayEventManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FGameplayEventDynamicDelegate, const FGameplayEvent&, Event);
UCLASS()
class LUNARIA_API AGameplayEventManager : public AActor, public IDetailTogglable
{
	GENERATED_BODY()

public:
	AGameplayEventManager();
	virtual void Tick(float DeltaTime) override;
	virtual void ToggleDetailOn() override;
	virtual void ToggleDetailOff() override;

	UFUNCTION(BlueprintCallable)
		static AGameplayEventManager* Get(AActor* ActorContext);

	static AGameplayEventManager* Get(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable)
		void SubmitEvent(const FGameplayEvent& Event);

	UFUNCTION(BlueprintCallable)
		static void Submit(AActor* ClientObject, const FGameplayEvent& Event);

	UFUNCTION(BlueprintCallable)
		static void WhenClassAgentOf(ENativeEventType Action, UClass* Class, AActor* ClientObject, FGameplayEventDynamicDelegate ClientFunction);

	UFUNCTION(BlueprintCallable)
		static void WhenClassSubjectOf(ENativeEventType Action, UClass* Class, AActor* ClientObject, FGameplayEventDynamicDelegate ClientFunction);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InTags,InValues,InVectors,InClasses,InEventTags"))
		static void CreateEvent(AActor* InAgent, ENativeEventType InAction, AActor* InSubject, const TMap<FString, FString>& InTags,
			const TMap<FString, float>& InValues, const TMap<FString, FVector>& InVectors, const TMap<FString, UClass*>& InClasses, const FGameplayTagContainer& InEventTags);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InEventTags"))
		static void CreateHit(AActor* InAgent, AActor* InSubject, float Damage, const FGameplayTagContainer& InEventTags);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InEventTags"))
		static void CreateHeal(AActor* InAgent, AActor* InSubject, float Scale, const FGameplayTagContainer& InEventTags);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InEventTags"))
		static void ApplyStatusEffect(AActor* InAgent, AActor* InSubject, TSubclassOf<ABoon> InEffect, const FGameplayTagContainer& InEventTags);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	using ClassDelegateMapType = TMap<ENativeEventType, TMap<UClass*, TArray<FGameplayEventDynamicDelegate>>>;

	void ProcessGameplayEvents();
	void ProcessGameplayEvent(const FGameplayEvent& Event);
	void TriggerSubjectObservation(const FGameplayEvent& Event);
	void TriggerAgentObservation(const FGameplayEvent& Event);
	void TriggerAgentOfClassDelegates(const FGameplayEvent& Event);
	void TriggerSubjectOfClassDelegates(const FGameplayEvent& Event);
	void ProcessHitEvent(const FGameplayEvent& Event);
	void ProcessKillEvent(const FGameplayEvent& Event);
	void ProcessHealEvent(const FGameplayEvent& Event);
	void ProcessApplyStatusEvent(const FGameplayEvent& Event);
	void CullHangingDelegates(ClassDelegateMapType& Map);
	FString PrintDelegateMap(const ClassDelegateMapType& Map);
	void DebugMaps();

	UPROPERTY()
		TArray<FGameplayEvent> Events;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* DebugWidgetComponent;

	ClassDelegateMapType AgentOfClassDelegates;
	ClassDelegateMapType SubjectOfClassDelegates;

	FTimerHandle DelegateCullTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Garbage, meta = (AllowPrivateAccess = "true"))
		float CullInterval = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Garbage, meta = (AllowPrivateAccess = "true"))
		bool ShouldWriteKillStatements = false;

	static void BroadcastClassEventDelegate(ClassDelegateMapType& Map, UClass* Class, const FGameplayEvent& Event);
	static void AddClassEventDelegate(ClassDelegateMapType& Map, ENativeEventType Action, UClass* Class, FGameplayEventDynamicDelegate ClientFunction);
};
