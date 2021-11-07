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

private:
	void HandleShipDeath(class UHealthComponent* HealthComponent, int32 KillingBlow);

	// Dropdown selector for behavior tree in blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviors", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* CombatComponent;
};
