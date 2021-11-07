// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceProjectile.generated.h"

UCLASS()
class LUNARIA_API ASpaceProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASpaceProjectile();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetDamage(int32 InDamage) { DamagePayload = InDamage; }

	UFUNCTION(BlueprintCallable)
		void SetTravelDistance(float InDistance) { TravelDistance = InDistance; }

	UFUNCTION(BlueprintCallable)
		int32 GetDamage() const { return DamagePayload; }

	class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovementComponent; };

protected:
	virtual void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ProjectileShell;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float SecondsUntilDespawn{ 10.f };

	UPROPERTY()
		FTimerHandle Handle;

	int32 DamagePayload = 0;

	float TravelDistance = 9999999.f;

	FVector StartingPoint;
};
