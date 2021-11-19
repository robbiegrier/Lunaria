// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEventObserver.h"
#include "GameplayTagContainer.h"
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
};

/**
 *
 */
UCLASS()
class LUNARIA_API ABoon : public AActor, public IGameplayEventObserver
{
	GENERATED_BODY()

public:

	ABoon();
	void NativeOnAdded(class UAttributesComponent* Attributes);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Metadata, meta = (AllowPrivateAccess = "true"))
		FString BoonName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Testing, meta = (AllowPrivateAccess = "true"))
		TArray<FAttributeModifier> AttributeModifierList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

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
