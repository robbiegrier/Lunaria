// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BobbleMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UBobbleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBobbleMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateBobbling(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		float BobbleSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		float BobbleDistance = 10.f;

	float InitialHeight{ 0.f };
	bool CurrentlyGoingUp{ true };
	class USceneComponent* FlairMesh;
};
