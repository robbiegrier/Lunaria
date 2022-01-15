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
		void Accelerate(float Scale);

	UFUNCTION(BlueprintCallable)
		void Turn(float Scale);

protected:
	virtual void BeginPlay() override;

private:
	void UpdateBanking();
	void ExecuteTurning(float Scale, float Speed);

	// AI Turns slightly slower than the player without this compensation
	static float CpuTurnCompensation;

	// Banking customization
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MaxBankLeft{ 50.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MaxBankRight{ 50.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float BankFalloff{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float BankAmount{ 400.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float TurnSpeedSeed{ 400.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MoveSpeedSeed{ 400.f };

	class UAttributesComponent* Attributes;
	class UCharacterMovementComponent* CharacterMovement;
	float CurrentBankValue{ 0.f };
};
