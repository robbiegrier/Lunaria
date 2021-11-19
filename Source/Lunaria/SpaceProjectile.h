// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "SpaceProjectile.generated.h"

UCLASS()
class LUNARIA_API ASpaceProjectile : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ASpaceProjectile();
	virtual void Tick(float DeltaTime) override;
	class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovementComponent; };
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; }

	UFUNCTION(BlueprintCallable)
		void SetDamage(float InDamage) { DamagePayload = InDamage; }

	UFUNCTION(BlueprintCallable)
		void SetTravelDistance(float InDistance) { TravelDistance = InDistance; }

	UFUNCTION(BlueprintCallable)
		float GetDamage() const { return DamagePayload; }

	UFUNCTION(BlueprintCallable)
		class UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }

	UFUNCTION(BlueprintCallable)
		void SetPayloadProperties(const FGameplayTagContainer& TagContainer, float InDamage, float InDistance = 9999999.f);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		FGameplayTagContainer GameplayTags;

	UPROPERTY()
		FTimerHandle Handle;

	float DamagePayload = 0;
	float TravelDistance = 9999999.f;
	FVector StartingPoint;
};
