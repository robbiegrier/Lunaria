// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionCreateProjectile.generated.h"

UCLASS()
class LUNARIA_API UActionCreateProjectile : public UAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		static void PerformCreateProjectile(AActor* InAgent, ATool* InTool, TSubclassOf<class ASpaceProjectile> InProjectileClass, const FTransform& InTransform, float InBaseDamage, float InDistance, float InSpeed);

	virtual void Execute() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UClass* ProjectileClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FTransform Transform;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float BaseDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Distance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;
};
