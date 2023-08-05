// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEventObserver.h"
#include "Boon.h"
#include "GameplayTagContainer.h"
#include "UpgradeNode.h"
#include "Stat.h"
#include "Ability.h"
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

	// Gameplay event observer interface to trigger events on Boons
	virtual void NativeWhenAgentOf(class UAction* Action) override;
	virtual void NativeWhenSubjectOf(class UAction* Action) override;

	UFUNCTION(BlueprintCallable)
		void AddBoon(ABoon* NewBoon);

	UFUNCTION(BlueprintCallable)
		void AddBoonFromClass(TSubclassOf<ABoon> NewBoonClass);

	UFUNCTION(BlueprintCallable)
		ABoon* AddStatusEffectFromClass(TSubclassOf<ABoon> NewEffectClass, AActor* Creator, float Duration);

	UFUNCTION(BlueprintCallable)
		void RemoveAndDestroyBoon(ABoon* TheBoon);

	UFUNCTION(BlueprintCallable)
		void RemoveAndDestroyStatusEffect(class AStatusEffect* TheStatusEffect);

	UFUNCTION(BlueprintCallable)
		void RemoveAndDestroyStatusEffectByName(const FString& StatusEffectName);

	UFUNCTION(BlueprintCallable)
		const TMap<UClass*, class AStatusEffect*>& GetCurrentStatusEffects() const { return StatusEffects; }

	UFUNCTION(BlueprintCallable)
		const TArray<ABoon*>& GetCurrentBoons() const { return Boons; }

	UFUNCTION(BlueprintCallable)
		void Reset();

	bool IsNodeViable(const FUpgradeNode& Node);

	// Register an Attribute object with a tag name
	void RegisterAttribute(const FString& AttributeName, class UAttribute* Attribute);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* AreaOfEffectDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* AreaOfEffectRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* DamageReceived;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* StatusEffectAppliedDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class ULunariaStat* Charges;

	UPROPERTY()
		TMap<FGameplayTag, UAttribute*> StatMap;

	UFUNCTION(BlueprintCallable)
		ULunariaColor* GetColorAttribute(const FString& Name);

private:
	void ClearBoons();
	void ClearStatusEffects();

	UPROPERTY()
		TArray<ABoon*> Boons;

	UPROPERTY()
		TMap<UClass*, class AStatusEffect*> StatusEffects;
};
