// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Medium.generated.h"

UCLASS()
class LUNARIA_API AMedium : public AActor
{
	GENERATED_BODY()

public:
	static AMedium* Spawn(UClass* Class, AActor* Agent, AActor* Tool, FTransform Transform, float BaseDamage);

	AMedium();
	virtual void Tick(float DeltaTime) override;
	virtual void OnHitEnd(class UActionHit* HitAction) {}
	virtual void SetColor(const FLinearColor& InColor) {}
	virtual void LaunchCombatMedium() {}

	UFUNCTION(BlueprintCallable)
		void SetTool(AActor* InTool) { Tool = InTool; }

	UFUNCTION(BlueprintCallable)
		void SetAgent(AActor* InAgent) { Agent = InAgent; }

	UFUNCTION(BlueprintCallable)
		void SetBaseDamage(float InBaseDamage) { BaseDamage = InBaseDamage; }

	UFUNCTION(BlueprintCallable)
		AActor* GetTool() const { return Tool; }

	UFUNCTION(BlueprintCallable)
		AActor* GetAgent() const { return Agent; }

	UFUNCTION(BlueprintCallable)
		float GetBaseDamage() const { return BaseDamage; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* Tool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* Agent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDamage = 10.f;
};
