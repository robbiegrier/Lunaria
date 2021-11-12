// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEventObserver.h"
#include "Boon.generated.h"

USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()
public:
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
		FAttributeModifier GetAttributeModifier(const FString& Attribute) const;

	UFUNCTION(BlueprintCallable)
		void SetAttributeModifier(const FString& Attribute, const FAttributeModifier& Modifier);

	UFUNCTION(BlueprintCallable)
		void SetAttributeModifierAdditive(const FString& Attribute, float Additive);

	UFUNCTION(BlueprintCallable)
		void SetAttributeModifierMultiplier(const FString& Attribute, float Multiplier);

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
		TMap<FString, FAttributeModifier> AttributeModifiers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* MyOwnerAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (AllowPrivateAccess = "true"))
		float DurationAsStatusEffect = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect", meta = (AllowPrivateAccess = "true"))
		int32 MaxStacksAsStatusEffect = 3;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void BeforeAttributeQueried(const FString& Attribute);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnAdded();
};
