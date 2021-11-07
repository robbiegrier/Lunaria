// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		int32 GetTeam() { return Team; }

	UFUNCTION(BlueprintCallable)
		void SetTeam(int32 InTeam) { Team = InTeam; }

	UFUNCTION(BlueprintCallable)
		int32 GetAttackDamage();

	const static int32 PlayerTeam = 0;
	const static int32 EnemyTeam = 1;
	const static int32 NeutralTeam = 2;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
		int32 Team = EnemyTeam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		int32 BaseAttackDamage = 10;
};
