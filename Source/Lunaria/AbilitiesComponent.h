// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilitiesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PressAbility(int32 AbilitySlot);
	void ReleaseAbility(int32 AbilitySlot);
	void CancelAbility(int32 AbilitySlot);

	void SetAbility(int32 AbilitySlot, class AAbility* Ability);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		TArray<class AAbility*> Abilities;
};
