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

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_USTRUCT_BODY()

public:

	// Movement
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	//	FStat MovementSpeed = { 300.f };

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	//	float TurnSpeed = 500.f;

	// Offense
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Offense)
		float ProjectileSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Offense)
		float Damage = 0.f;

	// Defense
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Defense)
		float MaxHealth = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Defense)
		float DamageReduction = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Defense)
		float DamageAmplification = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Defense)
		float HealingReduction = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Defense)
		float HealingAmplification = 0.f;

	// Luck
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Luck)
		float NumberOfChoices = 3.f;
};

USTRUCT(BlueprintType)
struct FBoonStats
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Boon)
		float Damage = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Boon)
		float StatusEffectReceivedDuration = 3.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Boon)
		float StatusEffectAppliedDuration = 3.f;
};

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
		float GetTagged(const FGameplayTag& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		float GetFromTagContainer(const FGameplayTagContainer& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		float ClassGet(UClass* Class, const FString& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		float ClassGetTagged(UClass* Class, const FGameplayTag& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		float ClassGetFromTagContainer(UClass* Class, const FGameplayTagContainer& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		FAttributeModifier GetModifierFromTagContainer(const FGameplayTagContainer& Attribute, float Seed, UClass* Class = nullptr);

	UFUNCTION(BlueprintCallable)
		float GetForAbilityType(const FGameplayTag& Ability, const FGameplayTag& Attribute, float Seed);

	UFUNCTION(BlueprintCallable)
		void AddBoon(ABoon* NewBoon);

	UFUNCTION(BlueprintCallable)
		void AddBoonFromClass(TSubclassOf<ABoon> NewBoonClass);

	UFUNCTION(BlueprintCallable)
		ABoon* AddStatusEffectFromClass(TSubclassOf<ABoon> NewEffectClass, AActor* Creator = nullptr);

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
		FLinearColor GetColor(const FGameplayTag& Attribute);

	UFUNCTION(BlueprintCallable)
		FLinearColor GetColorFromTagContainer(const FGameplayTagContainer& Attribute);

	UFUNCTION(BlueprintCallable)
		void Reset();

	bool IsNodeViable(const FUpgradeNode& Node);

	void RegisterAttribute(const FString& AttributeName, class UAttribute* Attribute);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStat* MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStat* TurnSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStat* Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStat* DamageReceived;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FAbilityStats AttackStats;

	UPROPERTY()
		TMap<FGameplayTag, UAttribute*> StatMap;

	UFUNCTION(BlueprintCallable)
		const FAbilityStats& GetAbilityStats(EAbilityKey Key);

private:
	void ClearBoons();
	void ClearStatusEffects();

	UPROPERTY()
		TArray<ABoon*> Boons;

	UPROPERTY()
		TMap<UClass*, class AStatusEffect*> StatusEffects;
};
