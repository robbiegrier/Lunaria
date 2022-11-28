// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.generated.h"

UCLASS()
class LUNARIA_API AOrb : public AActor
{
	GENERATED_BODY()

public:
	AOrb();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Launch(AActor* InTarget);

protected:
	virtual void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* Shell;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
		float Speed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
		AActor* Target;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawn Events")
		void OnReachedTarget();
};
