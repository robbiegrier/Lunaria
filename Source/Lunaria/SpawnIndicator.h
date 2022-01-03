// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnIndicator.generated.h"

DECLARE_EVENT(ASpawnIndicator, FSpawnIndicatorComplete)
UCLASS()
class LUNARIA_API ASpawnIndicator : public AActor
{
	GENERATED_BODY()

public:
	ASpawnIndicator();
	virtual void Tick(float DeltaTime) override;

	FSpawnIndicatorComplete Completion;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior, meta = (AllowPrivateAccess = "true"))
		float Duration = 3.f;

	float SpawnTime = 0.f;
};
