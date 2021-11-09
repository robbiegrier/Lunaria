// Robbie Grier

#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Printer.h"
#include "Hittable.h"
#include "CpuHealthBar.h"
#include "AttributesComponent.h"
#include "Helpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Attributes = Helpers::GetSister<UAttributesComponent>(this);
	SetHealth(GetMaxHealth());
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SendDataToWidgets();
}

void UHealthComponent::ApplyDamage(float Scale)
{
	Scale = FMath::Max(Scale, 0.f);

	if (Scale > 0)
	{
		DamageTakenEvent.Broadcast(this, Scale);
	}

	SetHealth(CurrentHealth - Scale, 0.f);
};

void UHealthComponent::SetHealth(float Value, float Min)
{
	if (Attributes)
	{
		auto PreviousHealth = CurrentHealth;
		auto TrueChange = PreviousHealth - Value;
		CurrentHealth = FMath::Min(Value, GetMaxHealth());
	}
}

void UHealthComponent::SendDataToWidgets()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthValues(CurrentHealth, GetMaxHealth());
	}
}

void UHealthComponent::BindHealthBar(UCpuHealthBar* Widget)
{
	HealthBarWidget = Widget;
	SendDataToWidgets();
}

bool UHealthComponent::IsHealthDepleted() const
{
	return CurrentHealth <= 0.f;
}

float UHealthComponent::GetMaxHealth() const
{
	return Attributes->Get("Max Health", MaxHealthSeed);
}