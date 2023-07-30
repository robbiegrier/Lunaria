// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

//DECLARE_DYNAMIC_DELEGATE_TwoParams(FHitDamageObserver, class UAction*, Action, class UStat*, Stat);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		int32 GetTeam() const { return Team; }

	UFUNCTION(BlueprintCallable)
		void SetTeam(int32 InTeam) { Team = InTeam; }

	UFUNCTION(BlueprintCallable)
		void AddSummon(AActor* InSummon);

	UFUNCTION(BlueprintCallable)
		const TArray<AActor*>& GetSummons() const { return Summons; }

	void SetSummoner(AActor* InSummoner) { Summoner = InSummoner; }

	UFUNCTION(BlueprintCallable)
		AActor* GetSummoner() const { return Summoner; }

	UFUNCTION(BlueprintCallable)
		bool IsSummon() const { return Summoner != nullptr; }

	UFUNCTION(BlueprintCallable)
		void AddAction(class UAction* Action) { Actions.Add(Action); }

	//UFUNCTION(BlueprintCallable)
	//	void AddHitDamageObserver(const FHitDamageObserver& Observer) { HitDamageObservers.Add(Observer); }

	//const TArray<FHitDamageObserver>& GetHitDamageObservers() const { return HitDamageObservers; }

	const static int32 PlayerTeam = 0;
	const static int32 EnemyTeam = 1;
	const static int32 NeutralTeam = 2;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team, meta = (AllowPrivateAccess = "true"))
		int32 Team = EnemyTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team, meta = (AllowPrivateAccess = "true"))
		TArray<AActor*> Summons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions, meta = (AllowPrivateAccess = "true"))
		TArray<class UAction*> Actions;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actions, meta = (AllowPrivateAccess = "true"))
	//	TArray<FHitDamageObserver> HitDamageObservers;

	AActor* Summoner = nullptr;
};
