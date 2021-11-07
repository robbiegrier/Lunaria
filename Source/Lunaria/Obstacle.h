// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "SpaceObject.h"
#include "Hittable.h"
#include "Obstacle.generated.h"

UCLASS()
class LUNARIA_API AObstacle : public ASpaceObject, public IHittable
{
	GENERATED_BODY()

public:
	AObstacle();
	virtual void Tick(float DeltaTime) override;
	void HandleCollisionRecognized(AActor* OtherActor);

protected:
	virtual void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	void HandleObstacleDestroyed(class UHealthComponent* HealthComp, int KillingBlow);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		int32 CollisionDamage = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		int32 SelfDamageOnCollision = 8;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBarComponent;
};
