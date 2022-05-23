// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "SpaceObject.h"
#include "Hittable.h"
#include "GameplayEventObserver.h"
#include "GameplayTagContainer.h"
#include "Obstacle.generated.h"

UCLASS()
class LUNARIA_API AObstacle : public ASpaceObject, public IHittable, public IGameplayEventObserver
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
		int32 CollisionDamage = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
		int32 SelfDamageOnCollision = 35;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		FGameplayTagContainer GameplayTags;
};
