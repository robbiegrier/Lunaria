// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.h"
#include "AbilitySlot.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UAbilitySlot : public UObject
{
	GENERATED_BODY()

public:
	void SetAbility(class AAbility* InAbility);
	class AAbility* GetAbility() const { return Ability; }

	void SetParent(class UAbilitiesComponent* InParent) { Parent = InParent; }
	class UAbilitiesComponent* GetParent() const { return Parent; }

	void SetSlotName(const FString& InName) { SlotName = InName; }
	const FString& GetSlotName() const { return SlotName; }

	void SetAbilityKey(EAbilityKey InKey) { Key = InKey; }

	void Begin();

	UPROPERTY()
		class ULunariaColor* Color;

protected:
	class UAbilitiesComponent* Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AAbility* Ability;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EAbilityKey Key;
};
