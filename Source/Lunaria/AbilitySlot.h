// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

	void Begin();

	UPROPERTY()
		class UStat* Damage;

	UPROPERTY()
		class UColorAttribute* Color;

private:
	class UAbilitiesComponent* Parent;

	UPROPERTY()
		FString SlotName;

	UPROPERTY()
		class AAbility* Ability;
};
