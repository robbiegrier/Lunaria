// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.h"
#include "AbilitiesComponent.generated.h"

UENUM()
enum class EAbilityKey : uint8
{
	A, B, X, Y
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilitiesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AAbility* GetAbility(EAbilityKey Key) const;
	void PressAbility(EAbilityKey Key);
	void ReleaseAbility(EAbilityKey Key);
	void SetAbility(EAbilityKey Key, AAbility* Ability);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		TMap<EAbilityKey, AAbility*> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		TMap<EAbilityKey, TSubclassOf<AAbility>> AbilityDefaults;
};
