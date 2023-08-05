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
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Attributes = Helpers::GetSister<UAttributesComponent>(this);

	MaxHealth = NewObject<ULunariaStat>();
	MaxHealth->Set(500.f);

	Attributes->RegisterAttribute("MaxHealth", MaxHealth);
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
}

void UHealthComponent::ApplyHealing(float Scale)
{
	Scale = FMath::Max(Scale, 0.f);
	MissingHealth = FMath::Max(0.f, MissingHealth - Scale);
}

void UHealthComponent::BindHealthBar(UCpuHealthBar* Widget)
{
	HealthBarWidget = Widget;
}

bool UHealthComponent::IsHealthDepleted() const
{
	return GetCurrentHealth() <= 0.f;
}

void UHealthComponent::Reset()
{
	MissingHealth = 0.f;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth->Render(nullptr);
}