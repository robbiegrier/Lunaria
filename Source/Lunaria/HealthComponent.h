// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/*
* This component manages an actor's health. It is the source of truth for "current health". The max health is
* found dynamically from an AttributesComponent. This component can be bound to a HealthBar widget.
*/
DECLARE_EVENT_TwoParams(UHealthComponent, FHealthEvent, UHealthComponent*, int32); // health events pass in the component and the health amount changed
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
		void ApplyDamage(float Scale);

	UFUNCTION(BlueprintCallable)
		void SetHealth(float Value, float Min = 1);

	UFUNCTION(BlueprintCallable)
		void BindHealthBar(class UCpuHealthBar* Widget);

	UFUNCTION(BlueprintCallable)
		bool IsHealthDepleted() const;

protected:
	virtual void BeginPlay() override;

private:
	void SendDataToWidgets();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		class UCpuHealthBar* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		float MaxHealthSeed = 100.f;

	UPROPERTY()
		float CurrentHealth = MaxHealthSeed;

	FHealthEvent DamageTakenEvent;
	FHealthEvent HealthGainedEvent;

	class UAttributesComponent* Attributes;
};
