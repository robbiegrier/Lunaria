// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.h"
#include "AbilitySlot.h"
#include "AbilitiesComponent.generated.h"

//USTRUCT(BlueprintType)
//struct FAbilitySlotTmp
//{
//	GENERATED_USTRUCT_BODY()
//public:
//	FAbilitySlotTmp() = default;
//	explicit FAbilitySlotTmp(const FGameplayTag& InTag) : Tag(InTag) {}
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySlot, meta = (AllowPrivateAccess = "true"))
//		AAbility* Ability;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySlot, meta = (AllowPrivateAccess = "true"))
//		FGameplayTag Tag;
//};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilitiesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		AAbility* GetAbility(EAbilityKey Key) const;

	UFUNCTION(BlueprintCallable)
		void PressAbility(EAbilityKey Key);

	UFUNCTION(BlueprintCallable)
		void ReleaseAbility(EAbilityKey Key);

	UFUNCTION(BlueprintCallable)
		void SetAbility(EAbilityKey Key, AAbility* Ability);

	UFUNCTION(BlueprintCallable)
		static FGameplayTag AbilityKeyToTag(EAbilityKey Key);

	UFUNCTION(BlueprintCallable)
		static FString AbilityKeyToSlotName(EAbilityKey Key);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	void EnsureSlotExists(EAbilityKey Key);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		TMap<EAbilityKey, UAbilitySlot*> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		TMap<EAbilityKey, TSubclassOf<AAbility>> AbilityDefaults;
};
