// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Boon.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class LUNARIA_API UBoon : public UObject
{
	GENERATED_BODY()

public:
	UBoon();

	UFUNCTION(BlueprintCallable)
		const FString& GetBoonName() const { return BoonName; }

	virtual void NativeOnAdded(class AActor* AppliedTo);

	UFUNCTION(BlueprintImplementableEvent, Category = Hittable)
		void OnAdded();

	/// UNIMPLEMENTED
	//void OnRemoved();
	//void OnTakeDamage(float Amount);
	//void OnDeath();

	//// Movement Events
	//void OnBlink(const FVector& Start, const FVector& End);
	//void OnBlinkQueued(const FVector& Start, const FVector& End);

	//// Hit with anything
	//void OnHitEnemy(/* LunariaHitInteraction* HitInteraction */);
	//void OnHitAlly(class AActor* Other);
	//void OnKillEnemy(class AActor* Other);
	//void OnHitNeutral(class AActor* Other);
	//void OnKillNeutral(class AActor* Other);

	//// Attack (X)
	//void OnAttack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boon", meta = (AllowPrivateAccess = "true"))
		FString BoonName;
};
