// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UCLASS()
class LUNARIA_API ATool : public AActor
{
	GENERATED_BODY()

public:
	ATool();
	virtual void Tick(float DeltaTime) override;
	void SetAgent(AActor* InAgent) { Agent = InAgent; }
	virtual FLinearColor GetColor() { return FLinearColor::White; }

	UFUNCTION(BlueprintCallable)
		AActor* GetAgent() const { return Agent; }

	UFUNCTION(BlueprintCallable)
		void SetBaseDamage(float InBaseDamage) { BaseDamage = InBaseDamage; }

	UFUNCTION(BlueprintCallable)
		float GetBaseDamage() const { return BaseDamage; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* Agent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseDamage = 0.f;
};
