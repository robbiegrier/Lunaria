// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEventObserver.h"
#include "Boon.h"
#include "AttributesComponent.generated.h"

/*
* This component contains the "stats" of an actor that influence how strong it is. Other components will draw on these
* stats to update health, damage, and speed behaviors of the actor.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UAttributesComponent : public UActorComponent, public IGameplayEventObserver
{
	GENERATED_BODY()

public:
	UAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void NativeWhenAgentOf(const FGameplayEvent& Event) override;
	virtual void NativeWhenSubjectOf(const FGameplayEvent& Event) override;

	UFUNCTION(BlueprintCallable)
		float Get(const FString& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		void AddBoon(ABoon* NewBoon);

	UFUNCTION(BlueprintCallable)
		void AddBoonFromClass(TSubclassOf<ABoon> NewBoonClass);

	UFUNCTION(BlueprintCallable)
		void AddStatusEffectFromClass(TSubclassOf<ABoon> NewEffectClass);

	UFUNCTION(BlueprintCallable)
		void RemoveAndDestroyBoon(ABoon* TheBoon);

	UFUNCTION(BlueprintCallable)
		void RemoveAndDestroyStatusEffect(class AStatusEffect* TheStatusEffect);

	UFUNCTION(BlueprintCallable)
		const TMap<UClass*, class AStatusEffect*>& GetCurrentStatusEffects() const { return StatusEffects; }

	UFUNCTION(BlueprintCallable)
		const TArray<ABoon*>& GetCurrentBoons() const { return Boons; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	void ClearBoons();
	void ClearStatusEffects();

	UPROPERTY()
		TArray<ABoon*> Boons;

	UPROPERTY()
		TMap<UClass*, class AStatusEffect*> StatusEffects;
};
