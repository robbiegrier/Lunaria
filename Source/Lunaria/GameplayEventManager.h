// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGameplayEvent.h"
#include "GameplayEventManager.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FGameplayEventDynamicDelegate, const FGameplayEvent&, Event);
UCLASS()
class LUNARIA_API AGameplayEventManager : public AActor
{
	GENERATED_BODY()

public:
	AGameplayEventManager();
	virtual void Tick(float DeltaTime) override;

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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	using ClassDelegateMapType = TMap<ENativeEventType, TMap<UClass*, TArray<FGameplayEventDynamicDelegate>>>;

	void ProcessGameplayEvents();
	void TriggerSubjectObservation(const FGameplayEvent& Event);
	void TriggerAgentObservation(const FGameplayEvent& Event);
	void TriggerAgentOfClassDelegates(const FGameplayEvent& Event);
	void TriggerSubjectOfClassDelegates(const FGameplayEvent& Event);
	void ProcessHitEvent(const FGameplayEvent& Event);
	void ProcessKillEvent(const FGameplayEvent& Event);
	void CullHangingDelegates(ClassDelegateMapType& Map);

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

	static void BroadcastClassEventDelegate(ClassDelegateMapType& Map, UClass* Class, const FGameplayEvent& Event);
	static void AddClassEventDelegate(ClassDelegateMapType& Map, ENativeEventType Action, UClass* Class, FGameplayEventDynamicDelegate ClientFunction);
};