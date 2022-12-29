// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hittable.h"
#include "GameplayEventObserver.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Spaceship.generated.h"

UCLASS()
class LUNARIA_API ASpaceship : public ACharacter, public IHittable, public IGameplayEventObserver, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ASpaceship();
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RespawnOnMap(class AMapManager* MapManager);
	virtual void NativeWhenAgentOf(const FGameplayEvent& Event) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; }
	void SetRutterScaleModifier(float InModifier) { RutterScaleModifier = InModifier; }

	void EnterSpawningState();
	void EnterCombatState();

	void StartPlayerDeath();
	void EndPlayerDeath();

	UFUNCTION()
		void HandleThrottleInput(float Scale);

	UFUNCTION()
		void HandleRutterInput(float Scale);

	UFUNCTION(BlueprintCallable)
		void TurnToFaceActor(AActor* InActor, float MinDeltaAngle = 0.98f);

	UFUNCTION(BlueprintCallable)
		void TurnToFaceLocation(FVector InLocation, float MinDeltaAngle = 0.98f);

	UFUNCTION(BlueprintCallable)
		void TurnToFaceDirection(const FVector& Direction, float MinDeltaAngle = 0.98f);

	UFUNCTION(BlueprintCallable)
		class USpaceshipMovementComponent* GetSpaceshipMovement() const { return SpaceshipMovementComponent; }

	UFUNCTION(BlueprintCallable)
		class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable)
		class UAttributesComponent* GetAttributesComponent() const { return Attributes; }

	UFUNCTION()
		class USphereComponent* GetInteractionVolume() const { return InteractionVolume; }

	UFUNCTION(BlueprintCallable)
		class UAbilitiesComponent* GetAbilitiesComponent() const { return AbilitiesComponent; }

	UFUNCTION(BlueprintCallable)
		class UCombatComponent* GetCombatComponent() const { return CombatComponent; }

	UFUNCTION(BlueprintCallable)
		class UInventoryComponent* GetInventoryComponent() const { return Inventory; }

	UFUNCTION(BlueprintCallable)
		float GetCurrentTurnSpeed() const;

	UFUNCTION(BlueprintCallable)
		void InitializeEnemy();

	UFUNCTION(BlueprintCallable)
		void InitializeAlly();

	UFUNCTION(BlueprintCallable)
		void SetSpawnTime(float Time);

	UFUNCTION(BlueprintCallable)
		void SetMovementScale(float Scale) { MovementScale = Scale; }

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USpaceshipMovementComponent* SpaceshipMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		class UAbilitiesComponent* AbilitiesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		class UAttributesComponent* Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DebugViz;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float RutterScaleModifier = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MovementScale = 1.f;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawn Events")
		void OnRespawn();
};
