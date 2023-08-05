// Robbie Grier

#include "AttributesComponent.h"
#include "GameFramework/Actor.h"
#include "Printer.h"
#include "CpuHealthBar.h"
#include "HealthComponent.h"
#include "AttributeObserver.h"
#include "StatusEffect.h"
#include "Stat.h"
#include "Helpers.h"
#include "CombatComponent.h"
#include "StatCommand.h"
#include "Action.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	Damage = NewObject<ULunariaStat>();
	Damage->Set(100.f);
	RegisterAttribute("Damage", Damage);

	AreaOfEffectDelay = NewObject<ULunariaStat>();
	AreaOfEffectDelay->Set(0.f);
	RegisterAttribute("AreaOfEffect.Delay", AreaOfEffectDelay);

	AreaOfEffectRadius = NewObject<ULunariaStat>();
	AreaOfEffectRadius->Set(0.f);
	RegisterAttribute("AreaOfEffect.Radius", AreaOfEffectRadius);

	ProjectileSpeed = NewObject<ULunariaStat>();
	ProjectileSpeed->Set(0.f);
	RegisterAttribute("Projectile.Speed", ProjectileSpeed);

	DamageReceived = NewObject<ULunariaStat>();
	DamageReceived->Set(0.f);
	RegisterAttribute("DamageReceived", DamageReceived);

	StatusEffectAppliedDuration = NewObject<ULunariaStat>();
	StatusEffectAppliedDuration->Set(3.f);
	RegisterAttribute("StatusEffect.AppliedDuration", StatusEffectAppliedDuration);

	Cooldown = NewObject<ULunariaStat>();
	Cooldown->Set(0.f);
	RegisterAttribute("Cooldown", Cooldown);

	Charges = NewObject<ULunariaStat>();
	Charges->Set(1.f);
	RegisterAttribute("Charges", Charges);
}

void UAttributesComponent::RegisterAttribute(const FString& AttributeName, UAttribute* Attribute)
{
	auto Tag = FGameplayTag::RequestGameplayTag(FName(*AttributeName));
	Attribute->SetAttributeName(Tag);
	StatMap.Add(Tag, Attribute);
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto Boon : Boons)
	{
		Boon->SetActorTransform(GetOwner()->GetActorTransform());
	}

	for (auto& StatusEffect : StatusEffects)
	{
		StatusEffect.Value->SetActorTransform(GetOwner()->GetActorTransform());
	}
}

void UAttributesComponent::NativeWhenAgentOf(UAction* Action)
{
	for (auto Boon : Boons)
	{
		Boon->ExecuteAgentOf(Action);
	}
}

void UAttributesComponent::NativeWhenSubjectOf(UAction* Action)
{
	for (auto Boon : Boons)
	{
		Boon->ExecuteSubjectOf(Action);
	}
}

void UAttributesComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	ClearBoons();
	ClearStatusEffects();
}

void UAttributesComponent::AddBoon(ABoon* NewBoon)
{
	if (NewBoon)
	{
		NewBoon->SetAgent(GetOwner());
		Boons.Add(NewBoon);
		NewBoon->NativeOnAdded(this);
	}
}

void UAttributesComponent::AddBoonFromClass(TSubclassOf<ABoon> NewBoonClass)
{
	if (NewBoonClass)
	{
		auto NewBoon = GetWorld()->SpawnActor<ABoon>(NewBoonClass);
		AddBoon(NewBoon);
	}
}

ABoon* UAttributesComponent::AddStatusEffectFromClass(TSubclassOf<class ABoon> NewEffectClass, AActor* Creator, float Duration)
{
	auto AddedStack = static_cast<ABoon*>(nullptr);

	if (NewEffectClass)
	{
		auto Effect = static_cast<AStatusEffect*>(nullptr);
		auto Find = StatusEffects.Find(NewEffectClass);

		if (!Find)
		{
			Effect = GetWorld()->SpawnActor<AStatusEffect>();
			StatusEffects.Add(NewEffectClass, Effect);
			Effect->SetBoonClass(NewEffectClass);
			Effect->NativeOnAdded(this);
		}
		else
		{
			Effect = *Find;
		}

		Effect->SetDuration(Duration);
		AddedStack = Effect->AddStack(Creator);
	}

	return AddedStack;
}

void UAttributesComponent::RemoveAndDestroyBoon(ABoon* TheBoon)
{
	if (TheBoon)
	{
		TheBoon->NativeOnRemoved();
		Boons.Remove(TheBoon);
		TheBoon->Destroy();
	}
}

void UAttributesComponent::RemoveAndDestroyStatusEffect(AStatusEffect* TheStatusEffect)
{
	if (TheStatusEffect)
	{
		StatusEffects.Remove(TheStatusEffect->GetBoonClass());
		TheStatusEffect->Destroy();
	}
}

void UAttributesComponent::RemoveAndDestroyStatusEffectByName(const FString& StatusEffectName)
{
	auto TheEffect = static_cast<AStatusEffect*>(nullptr);

	for (auto& Pair : StatusEffects)
	{
		auto Boon = Pair.Value->GetStatuses()[0];
		if (Boon->GetBoonName() == StatusEffectName)
		{
			TheEffect = Pair.Value;
			break;
		}
	}

	RemoveAndDestroyStatusEffect(TheEffect);
}

void UAttributesComponent::ClearBoons()
{
	for (auto Boon : Boons)
	{
		if (Boon)
		{
			Boon->NativeOnRemoved();
			Boon->Destroy();
		}
	}

	Boons.Empty();
}

void UAttributesComponent::ClearStatusEffects()
{
	for (auto& StatusEffect : StatusEffects)
	{
		if (StatusEffect.Value)
		{
			StatusEffect.Value->Destroy();
		}
	}

	StatusEffects.Empty();
}

ULunariaColor* UAttributesComponent::GetColorAttribute(const FString& Name)
{
	auto Find = StatMap.Find(FGameplayTag::RequestGameplayTag(FName(*Name)));

	if (Find)
	{
		if (auto Color = Cast<ULunariaColor>(*Find))
		{
			return Color;
		}
	}

	return nullptr;
}

void UAttributesComponent::Reset()
{
	ClearBoons();
	ClearStatusEffects();
}

bool UAttributesComponent::IsNodeViable(const FUpgradeNode& Node)
{
	bool Viable = Node.Enablers.Num() == 0;

	for (auto Boon : GetCurrentBoons())
	{
		if (Boon->GetClass() == Node.BoonClass)
		{
			return false;
		}

		if (Node.Enablers.Contains(Boon->GetClass()))
		{
			Viable = true;
		}
	}

	return Viable;
}