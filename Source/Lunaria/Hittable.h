// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

UINTERFACE(MinimalAPI)
class UHittable : public UInterface
{
	GENERATED_BODY()
};

class LUNARIA_API IHittable
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void TakeProjectileHit(class ASpaceProjectile* Projectile);

	UFUNCTION()
		virtual void TakeHit(int32 DamageToApply, class AActor* Other);

	UFUNCTION()
		virtual void TakeLethalDamage();

	virtual void NativeTakeLethalDamage() {}

	UFUNCTION(BlueprintNativeEvent, Category = Hittable)
		void OnHit();
	void OnHit_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Hittable)
		void OnHitIgnored();
	void OnHitIgnored_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = Hittable)
		void OnTakeLethalDamage();
	void OnTakeLethalDamage_Implementation() {}

	UFUNCTION()
		virtual void Tmp(class UAttributesComponent* Attributes) {};
};
