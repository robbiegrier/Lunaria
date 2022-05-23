// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEventObserver.h"
#include "GameplayTagContainer.h"
#include "Choosable.h"
#include "Boon.generated.h"

USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		FGameplayTagContainer Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		float Additive = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Modifiers)
		float Multiplier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
		FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
		TArray<TSubclassOf<class ABoon>> StatusEffects;
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
	virtual FString GetChoiceName() override;
	virtual FString GetChoiceDescription() override;

	UFUNCTION(BlueprintCallable)
		const FString& GetBoonName() const { return BoonName; }

	UFUNCTION(BlueprintCallable)
		const FAttributeModifier& GetModifierForTagContainer(const FGameplayTagContainer& Attribute) const;

	UFUNCTION(BlueprintCallable)
		void SetAttributeModifier(const FGameplayTagContainer& Attribute, const FAttributeModifier& Modifier);

	UFUNCTION(BlueprintCallable)
		void Remove();

	UFUNCTION(BlueprintCallable)
		float GetDurationAsStatusEffect() const { return DurationAsStatusEffect; }

	UFUNCTION(BlueprintCallable)
		int32 GetMaxStacksAsStatusEffect() const { return MaxStacksAsStatusEffect; }

private:
	FAttributeModifier* FindModifier(const FGameplayTagContainer& Attribute) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		FString BoonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		FString BoonDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Testing, meta = (AllowPrivateAccess = "true"))
		TArray<FAttributeModifier> AttributeModifierList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

	// Creator is the actor who applied this boon and acts as the agent of event generated from the boon.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Agent, meta = (AllowPrivateAccess = "true"))
		AActor* Creator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* MyOwnerAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (AllowPrivateAccess = "true"))
		float DurationAsStatusEffect = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (AllowPrivateAccess = "true"))
		int32 MaxStacksAsStatusEffect = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComponent;

	static FAttributeModifier NullModifier;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void BeforeAttributeQueried(const FGameplayTagContainer& Attribute);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnAdded();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnRemoved();
};
