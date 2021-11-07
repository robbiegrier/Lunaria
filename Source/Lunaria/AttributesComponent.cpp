// Robbie Grier

#include "AttributesComponent.h"
#include "GameFramework/Actor.h"
#include "Printer.h"
#include "CpuHealthBar.h"
#include "HealthComponent.h"
#include "AttributeObserver.h"
#include "Boon.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::SetMaxHealth(int32 InMaxHealth)
{
	auto Previous = MaxHealth;
	MaxHealth = InMaxHealth;
	OnAttributesUpdated.Broadcast(this);
	OnMaxHealthUpdated.Broadcast(GetMaxHealth(), Previous);
}

void UAttributesComponent::SetAttackDamage(int32 InAttackDamage)
{
	AttackDamage = InAttackDamage;
	OnAttributesUpdated.Broadcast(this);
}

void UAttributesComponent::SetMoveSpeed(float InMoveSpeed)
{
	MoveSpeed = InMoveSpeed;
	PrintFloat(MoveSpeed);
	OnAttributesUpdated.Broadcast(this);
}

void UAttributesComponent::SetTurnSpeed(float InTurnSpeed)
{
	TurnSpeed = InTurnSpeed;
	OnAttributesUpdated.Broadcast(this);
}

void UAttributesComponent::SetTargetingTurnSpeed(float InTargetingTurnSpeed)
{
	TargetingTurnSpeed = InTargetingTurnSpeed;
	OnAttributesUpdated.Broadcast(this);
}

void UAttributesComponent::AddBoon(UBoon* NewBoon)
{
	if (NewBoon)
	{
		Boons.Add(NewBoon);
		NewBoon->NativeOnAdded(GetOwner());
	}
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributesComponent::BindToActor(AActor* Actor)
{
	for (auto Component : Actor->GetComponents())
	{
		if (auto Observer = Cast<IAttributeObserver>(Component))
		{
			OnAttributesUpdated.AddUFunction(Component, FName("NotifyAttributesUpdated"));
			Observer->SetAttributes(this);
		}
	}

	OnAttributesUpdated.Broadcast(this);
}

void UAttributesComponent::UnBind()
{
	OnAttributesUpdated.Clear();
}