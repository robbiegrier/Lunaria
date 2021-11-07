// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DetailTogglable.h"
#include "SpaceObject.generated.h"

UCLASS()
class LUNARIA_API ASpaceObject : public AActor, public IDetailTogglable
{
	GENERATED_BODY()

public:
	ASpaceObject();
	virtual void Tick(float DeltaTime) override;

	virtual void ToggleDetailOn() override;
	virtual void ToggleDetailOff() override;

	class USphereComponent* GetSphereComponent() const { return SphereComponent; }
	class UStaticMeshComponent* GetMeshComponent() const { return Mesh; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UBobbleMovementComponent* BobbleMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class USpaceObjectMovementComponent* SpaceObjectMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* DebugWidgetComponent;
};
