// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectRingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UObjectRingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectRingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Clear();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		float BaseRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		float RadiusVariance = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		int32 TotalObjects = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		float BaseObjectScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bobble, meta = (AllowPrivateAccess = "true"))
		float ObjectScaleVariance = 0.5f;

	UPROPERTY()
		TArray<AActor*> RingActors;
};
