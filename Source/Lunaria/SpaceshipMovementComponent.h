// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceshipMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API USpaceshipMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpaceshipMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetTargetingVolume(class USphereComponent* InTargetingVolume) { TargetingVolume = InTargetingVolume; }

	UFUNCTION(BlueprintCallable)
		void SetTargetingVisualization(class UStaticMeshComponent* InVisualization) { TargetingVisualization = InVisualization; }

	UFUNCTION(BlueprintCallable)
		void SetBlinkVisualization(class UStaticMeshComponent* InVisualization) { BlinkVisualization = InVisualization; }

	UFUNCTION(BlueprintCallable)
		void ActivateTargeting();

	UFUNCTION(BlueprintCallable)
		void DeactivateTargeting();

	UFUNCTION(BlueprintCallable)
		void QueueBlink();

	UFUNCTION(BlueprintCallable)
		void ExecuteBlink();

	UFUNCTION(BlueprintCallable)
		bool IsTargeting() const { return TargetingIsActive; }

	UFUNCTION(BlueprintCallable)
		void Accelerate(float Scale);

	UFUNCTION(BlueprintCallable)
		void Turn(float Scale);

protected:
	virtual void BeginPlay() override;

private:
	void UpdateBanking();
	void UpdateTargeting();
	void UpdateBlinkQueue();
	FVector GetBlinkLocation();
	void ExecuteTurning(float Scale, float Speed);

	// Customization
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MaxBankLeft{ 50.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MaxBankRight{ 50.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float BankFalloff{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float BankAmount{ 400.f };

	// Volume references
	class USphereComponent* TargetingVolume;
	class UStaticMeshComponent* TargetingVisualization;
	class UStaticMeshComponent* BlinkVisualization;

	class UAttributesComponent* Attributes;

	// AI Turns slightly slower than the player without this compensation
	static float CpuTurnCompensation;

	// State Helpers
	bool TargetingIsActive{ false };
	bool TargetIsAquired{ false };
	bool BlinkIsQueued{ false };
	float CurrentBankValue{ 0.f };
};
