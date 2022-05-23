// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/*
* This component manages an actor's health. It is the source of truth for "current health". The max health is
* found dynamically from an AttributesComponent. This component can be bound to a HealthBar widget.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
		float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
		float GetCurrentHealthFast(float Max) const;

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
		void ApplyDamage(float Scale);

	UFUNCTION(BlueprintCallable)
		void ApplyHealing(float Scale);

	UFUNCTION(BlueprintCallable)
		void BindHealthBar(class UCpuHealthBar* Widget);

	UFUNCTION(BlueprintCallable)
		bool IsHealthDepleted() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		class UCpuHealthBar* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		float MaxHealthSeed = 100.f;

	UPROPERTY()
		float MissingHealth = 0.f;

	class UAttributesComponent* Attributes;
};
