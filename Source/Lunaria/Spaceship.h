// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hittable.h"
#include "Ability.h"
#include "Spaceship.generated.h"

UCLASS()
class LUNARIA_API ASpaceship : public ACharacter, public IHittable
{
	GENERATED_BODY()

public:
	ASpaceship();
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RespawnOnMap(class AMapManager* MapManager);

	UFUNCTION()
		void HandleThrottleInput(float Scale);

	UFUNCTION()
		void HandleRutterInput(float Scale);

	UFUNCTION()
		void TurnToFaceDirection(const FVector& Direction, float MinDeltaAngle = 0.95f);

	UFUNCTION()
		class USpaceshipMovementComponent* GetSpaceshipMovement() const { return SpaceshipMovementComponent; }

	UFUNCTION()
		class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION()
		class UAttributesComponent* GetAttributesComponent() const { return Attributes; }

	UFUNCTION()
		class USphereComponent* GetInteractionVolume() const { return InteractionVolume; }

	UFUNCTION()
		class UAbilitiesComponent* GetAbilitiesComponent() const { return AbilitiesComponent; }

	UFUNCTION()
		class UCombatComponent* GetCombatComponent() const { return CombatComponent; }

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* GenericAwarenessVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USpaceshipMovementComponent* SpaceshipMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitiesComponent* AbilitiesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DebugViz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityY;
};
