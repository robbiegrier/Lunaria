// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hittable.h"
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
		class USpaceshipWeaponComponent* GetMainWeapon() const { return MainWeaponComponent; }

	UFUNCTION()
		class USpaceshipWeaponComponent* GetSpecialWeapon() const { return SpecialWeaponComponent; }

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
		void Attack();

	UFUNCTION()
		void Special();

	UFUNCTION()
		void Shield();

	UFUNCTION()
		void QueueDash();

	UFUNCTION()
		void ExecuteDash();

	UFUNCTION()
		void StartAim();

	UFUNCTION()
		void EndAim();

protected:
	void BeginPlay() override;

private:
	void InterpolateCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float CameraTrackSpeed{ 3.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* TargetingVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* GenericAwarenessVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* TargetingVisualization;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BlinkVisualization;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USpaceshipMovementComponent* SpaceshipMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class USpaceshipWeaponComponent* MainWeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class USpaceshipWeaponComponent* SpecialWeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UAbilitiesComponent* AbilitiesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DebugViz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class TSubclassOf<class AAbility> AbilityY;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnAttack();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnSpecial();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnQueueDash();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnExecuteDash();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnStartAim();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnEndAim();
};
