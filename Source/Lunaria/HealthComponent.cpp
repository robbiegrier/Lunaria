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
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SendDataToWidgets();
}

float UHealthComponent::GetCurrentHealth() const
{
	return GetCurrentHealthFast(GetMaxHealth());
}

float UHealthComponent::GetCurrentHealthFast(float Max) const
{
	return Max - MissingHealth;
}

void UHealthComponent::ApplyDamage(float Scale)
{
	Scale = FMath::Max(Scale, 0.f);
	MissingHealth += Scale;
};

void UHealthComponent::SendDataToWidgets()
{
	if (HealthBarWidget)
	{
		auto Max = GetMaxHealth();
		HealthBarWidget->SetHealthValues(GetCurrentHealthFast(Max), Max);
	}
}

void UHealthComponent::BindHealthBar(UCpuHealthBar* Widget)
{
	HealthBarWidget = Widget;
	SendDataToWidgets();
}

bool UHealthComponent::IsHealthDepleted() const
{
	return GetCurrentHealth() <= 0.f;
}

float UHealthComponent::GetMaxHealth() const
{
	return Attributes->Get("MaxHealth", MaxHealthSeed);
}