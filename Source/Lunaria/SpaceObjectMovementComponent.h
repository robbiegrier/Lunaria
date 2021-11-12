// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceObjectMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API USpaceObjectMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpaceObjectMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetMass() const { return Mass; }

	// Called when spawned
	void InitializeOrbitalParent(AActor* Actor);

	// Called when changed
	void SetOrbitalParent(AActor* Actor);

	AActor* GetOrbitalParent() const { return OrbitalParent; }

	FVector GetTargetLocation();

	void SetOrbitalSpeed(float InSpeed) { OrbitalSpeed = InSpeed; }

	float GetOrbitalSpeed() const { return OrbitalSpeed; }

	void RandomizeOrbitalDirection();

	void SetOrbitalDirectionClockwise() { OrbitalSpeed = -FMath::Abs(OrbitalSpeed); }

	void SetOrbitalDirectionCounterClockwise() { OrbitalSpeed = FMath::Abs(OrbitalSpeed); }

	void SetOrbitalDirection(bool Clockwise) { Clockwise ? SetOrbitalDirectionClockwise() : SetOrbitalDirectionCounterClockwise(); }

	void FlipOrbitalDirection() { OrbitalSpeed = -OrbitalSpeed; }

protected:
	virtual void BeginPlay() override;

private:
	void QueryAndUpdateOrbitalParent();
	void UpdateOrbit(float DeltaTime);
	void UpdateFlairRotation(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Orbit, meta = (AllowPrivateAccess = "true"))
		float OrbitalSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Orbit, meta = (AllowPrivateAccess = "true"))
		bool OrbitStuff = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Orbit, meta = (AllowPrivateAccess = "true"))
		float Mass = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Orbit, meta = (AllowPrivateAccess = "true"))
		float OrbitalDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Flair, meta = (AllowPrivateAccess = "true"))
		float Height = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Flair, meta = (AllowPrivateAccess = "true"))
		float Spin = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Flair, meta = (AllowPrivateAccess = "true"))
		float Roll = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Flair, meta = (AllowPrivateAccess = "true"))
		float Tilt = 0.f;

	FVector Velocity;
	FVector LocationLastFrame;
	AActor* OrbitalParent;
	float CurrentOrbitalRotation{ 0.f };
	class UAttributesComponent* Attributes;
};
