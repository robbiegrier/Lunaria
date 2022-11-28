// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexWall.generated.h"

UCLASS()
class LUNARIA_API AHexWall : public AActor
{
	GENERATED_BODY()

public:
	AHexWall();
	virtual void Tick(float DeltaTime) override;

	FVector GetAdjecentSlot(int ClockSide);

protected:
	virtual void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* Shell;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* SpaceObjectDetector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float SinkSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float SinkRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Position, meta = (AllowPrivateAccess = "true"))
		float Seperation = 140.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Model, meta = (AllowPrivateAccess = "true"))
		float DarknessCreep = 4000.f;

	bool Sinking = false;
	FVector RestPosition;
};
