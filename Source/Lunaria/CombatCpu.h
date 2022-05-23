// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatCpu.generated.h"

UCLASS()
class LUNARIA_API ACombatCpu : public AAIController
{
	GENERATED_BODY()

public:
	ACombatCpu();
	void OnPossess(APawn* MyPawn) override;

	UFUNCTION(BlueprintCallable)
		void MoveShipTo(AActor* Target);

	void Tick(float DeltaTime) override;

	void EnterSpawningState();
	void EnterCombatState();

	UFUNCTION(BlueprintCallable)
		void SetSpawnTime(float Time);

	UFUNCTION(BlueprintCallable)
		class ASpaceship* GetSpaceshipPawn() const { return Spaceship; }

	UFUNCTION(BlueprintCallable)
		FVector GetMoveFocus() const;

private:
	void InitializeBeforeStartingTree();
	void FindLeader();
	void HandleShipDeath(class UHealthComponent* HealthComponent, int32 KillingBlow);

	// Dropdown selector for behavior tree in blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		float SpawnIndicatorOffset = -30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		float SpawnTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class ASpawnIndicator* SpawnIndicator;

	class ASpaceship* Spaceship;
};
