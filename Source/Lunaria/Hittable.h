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
		virtual void TakeHit(int32 DamageToApply, class AActor* Other);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile")
		void OnHitByProjectile(class ASpaceProjectile* Projectile);
};
