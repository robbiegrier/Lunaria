// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEventObserver.h"
#include "GameplayTagContainer.h"
#include "Choosable.h"
#include "Archetype.h"
#include "Boon.generated.h"

USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		FGameplayTagContainer Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		UClass* Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		float Additive = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		float Multiplier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
		FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ModifiersPerLevel)
		float AdditiveGainPerLevel = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ModifiersPerLevel)
		float MultiplierGainPerLevel = 0.f;
};

/**
 *
 */
UCLASS()
class LUNARIA_API ABoon : public AActor, public IGameplayEventObserver, public IChoosable
{
	GENERATED_BODY()

public:

	ABoon();
	void NativeOnAdded(class UAttributesComponent* Attributes, AActor* InCreator = nullptr);
	void NativeOnRemoved();
	void NativeLevelUp();
	virtual FString GetChoiceName() override;
	virtual FString GetChoiceDescription() override;
	void Choose(class AActor* Chooser) override;

	UFUNCTION(BlueprintCallable)
		const FString& GetBoonName() const { return BoonName; }

	UFUNCTION(BlueprintCallable)
		FAttributeModifier GetModifierForTagContainer(UClass* InClass, const FGameplayTagContainer& Attribute) const;

	UFUNCTION(BlueprintCallable)
		void SetAttributeModifier(const FGameplayTagContainer& Attribute, const FAttributeModifier& Modifier);

	UFUNCTION(BlueprintCallable)
		void Remove();

	UFUNCTION(BlueprintCallable)
		float GetDurationAsStatusEffect() const;

	UFUNCTION(BlueprintCallable)
		int32 GetMaxStacksAsStatusEffect() const { return MaxStacksAsStatusEffect; }

	UFUNCTION(BlueprintCallable)
		int32 GetBoonLevel() const { return BoonLevel; }

	UFUNCTION(BlueprintCallable)
		const FString& GetLevelUpDescription() const { return BoonLevelUpDescription; }

	UFUNCTION(BlueprintCallable)
		EArchetype GetArchetype() const { return Archetype; }

	UFUNCTION(BlueprintCallable)
		void SetArchetype(EArchetype InArchetype) { Archetype = InArchetype; }

	UFUNCTION(BlueprintCallable)
		class ASpaceProjectile* CreateBoonProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float Damage, float Distance, const FGameplayTagContainer& InTags, const TArray<AActor*>& ActorsToIgnore, int32 Bounces);

	UFUNCTION(BlueprintCallable)
		void MakeStatModification(FGameplayTag Stat, float Base, float Scalar);

	UFUNCTION(BlueprintCallable)
		void MakeColorModification(FGameplayTag Name, const FLinearColor& Color);

	UFUNCTION(BlueprintCallable)
		float GetDamage() const { return Damage; }

	void Close();

private:
	FAttributeModifier* FindModifier(const FGameplayTagContainer& Attribute) const;
	FAttributeModifier CalculateModifier(UClass* InClass, const FGameplayTagContainer& Attribute) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		FString BoonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		FString BoonDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		FString BoonLevelUpDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Testing, meta = (AllowPrivateAccess = "true"))
		TArray<FAttributeModifier> AttributeModifierList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

	// Creator is the actor who applied this boon and acts as the agent of event generated from the boon.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Agent, meta = (AllowPrivateAccess = "true"))
		AActor* Creator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* MyOwnerAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (AllowPrivateAccess = "true"))
		float Damage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (AllowPrivateAccess = "true"))
		float DurationAsStatusEffect = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (AllowPrivateAccess = "true"))
		int32 MaxStacksAsStatusEffect = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scene, meta = (AllowPrivateAccess = "true"))
		int32 BoonLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		EArchetype Archetype;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mods, meta = (AllowPrivateAccess = "true"))
		TArray<class UModification*> Modifications;

	static FAttributeModifier NullModifier;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void BeforeAttributeQueried(const FGameplayTagContainer& Attribute);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnAdded();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnRemoved();

	UFUNCTION(BlueprintImplementableEvent, Category = "Level Up Events")
		void OnLeveledUp();
};
