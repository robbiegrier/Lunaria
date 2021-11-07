// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

/*
* This component contains the "stats" of an actor that influence how strong it is. Other components will draw on these
* stats to update health, damage, and speed behaviors of the actor.
*/
DECLARE_EVENT_OneParam(UAttributesComponent, FAttributesUpdated, UAttributesComponent*);
DECLARE_EVENT_TwoParams(UAttributesComponent, FAttributeUpdated, int32, int32);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BindToActor(class AActor* Pawn);
	void UnBind();

	UFUNCTION(BlueprintCallable)
		int32 GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
		void SetMaxHealth(int32 InMaxHealth);

	UFUNCTION(BlueprintCallable)
		int32 GetAttackDamage() const { return AttackDamage; }

	UFUNCTION(BlueprintCallable)
		void SetAttackDamage(int32 InAttackDamage);

	UFUNCTION(BlueprintCallable)
		float GetMoveSpeed() const { return MoveSpeed; }

	UFUNCTION(BlueprintCallable)
		void SetMoveSpeed(float InMoveSpeed);

	UFUNCTION(BlueprintCallable)
		float GetTurnSpeed() const { return TurnSpeed; }

	UFUNCTION(BlueprintCallable)
		void SetTurnSpeed(float InTurnSpeed);

	UFUNCTION(BlueprintCallable)
		float GetTargetingTurnSpeed() const { return TargetingTurnSpeed; }

	UFUNCTION(BlueprintCallable)
		void SetTargetingTurnSpeed(float InTargetingTurnSpeed);

	UFUNCTION(BlueprintCallable)
		void AddBoon(class UBoon* NewBoon);

	FAttributeUpdated& GetMaxHealthUpdatedEvent() { return OnMaxHealthUpdated; }

protected:
	virtual void BeginPlay() override;

private:
	FAttributesUpdated OnAttributesUpdated;
	FAttributeUpdated OnMaxHealthUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		int32 AttackDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		float MoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		float TurnSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		float TargetingTurnSpeed = 500.f;

	UPROPERTY()
		TArray<class UBoon*> Boons;
};
