// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpaceshipWeaponComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API USpaceshipWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USpaceshipWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void FireWeapon();

	UFUNCTION(BlueprintCallable)
		void SetTargetingVolume(class USphereComponent* InTargetingVolume) { TargetingVolume = InTargetingVolume; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnWeaponFired();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeProjectileBeforeSpawn(class ASpaceProjectile* Projectile);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ASpaceProjectile> ProjectileClass;

	class USphereComponent* TargetingVolume;
	class UAttributesComponent* Attributes;
};
