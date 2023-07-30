// Robbie Grier

#include "Action.h"
#include "Printer.h"
#include "GameFramework/Actor.h"
#include "SpaceProjectile.h"
#include "Ability.h"
#include "AbilitySlot.h"
#include "Stat.h"
#include "AttributesComponent.h"
#include "HealthComponent.h"
#include "FGameplayEvent.h"
#include "GameplayEventManager.h"
#include "CombatComponent.h"
#include "Spaceship.h"
#include "LunariaGameModeBase.h"
#include "Boon.h"
#include "AreaOfEffect.h"
#include "StatCommand.h"

void UActionHit::PerformHit(AActor* Agent, AActor* Subject, AActor* Tool, AActor* Medium)
{
	auto Action = NewObject<UActionHit>();
	Action->Agent = Agent;
	Action->Subject = Subject;
	Action->Tool = Tool;
	Action->Medium = Medium;

	if (Agent && Subject)
	{
		if (auto CombatComponent = Agent->FindComponentByClass<UCombatComponent>())
		{
			CombatComponent->AddAction(Action);
		}
	}
}

void UActionHit::Execute()
{
	//Print("Action Hit: Tool: " + Tool->GetName() + ", Subject: " + Subject->GetName() + ", Medium: " + Medium->GetName());

	auto Event = FGameplayEvent(Agent, ENativeEventType::Hit, Subject);
	Event.Medium = Medium;
	Event.Tool = Tool;

	/// Move this below with true damage as tag
	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(Event);

	//auto DamageTotal = FStatComposite();

	//for (auto Observer : Agent->FindComponentByClass<UCombatComponent>()->GetHitDamageObservers())
	//{
	//	auto ObsStat = NewObject<UStat>();
	//	Observer.ExecuteIfBound(this, ObsStat);
	//	DamageTotal.Add(ObsStat);
	//}

	///// TODO - Damage Dealt Command
	//if (auto Attributes = Agent->FindComponentByClass<UAttributesComponent>())
	//{
	//	DamageTotal.Add(Attributes->Damage);
	//}

	///// TODO - Damage Dealt Command
	//if (auto Ability = Cast<AAbility>(Tool))
	//{
	//	DamageTotal.Add(Ability->GetDamage());
	//	DamageTotal.Add(Ability->GetSlot()->Damage);
	//}
	///// TODO - Damage Dealt Command
	//else if (auto Boon = Cast<ABoon>(Tool))
	//{
	//	DamageTotal.Add(Boon->GetDamage());
	//}

	//auto Damage = DamageTotal.Render();
	auto DamageRequested = 0.f;
	auto DamageApplied = 0.f;

	if (auto AgentAttr = Agent->FindComponentByClass<UAttributesComponent>())
	{
		DamageRequested = AgentAttr->Damage->Render(this);
	}

	if (auto SubjectAttr = Subject->FindComponentByClass<UAttributesComponent>())
	{
		SubjectAttr->DamageReceived->Set(DamageRequested);
		DamageApplied = SubjectAttr->DamageReceived->Render(this);
	}

	if (Subject && !Subject->IsActorBeingDestroyed())
	{
		if (auto CombatComponent = Subject->FindComponentByClass<UCombatComponent>())
		{
			auto TakeDamageAction = NewObject<UActionTakeDamage>();
			TakeDamageAction->Agent = Agent;
			TakeDamageAction->Subject = Subject;
			TakeDamageAction->Tool = Tool;
			TakeDamageAction->Damage = DamageApplied;
			CombatComponent->AddAction(TakeDamageAction);
		}
	}

	/// TODO - Callback on Medium (base class)
	if (Medium)
	{
		if (auto Projectile = Cast<ASpaceProjectile>(Medium))
		{
			Projectile->OnHitBeforeDestroy(Subject);
			Projectile->Die();
		}
	}
}

void UActionTakeDamage::Execute()
{
	//Print("Action Take Damage: Tool: " + Tool->GetName() + ", Subject: " + Subject->GetName() + ", Damage: " + FString::FromInt(Damage), FColor::Yellow);

	auto Event = FGameplayEvent(Agent, ENativeEventType::TakeDamage, Subject);
	Event.Tool = Tool;
	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(Event);

	/// Take Damage Commands?
	if (auto HealthComp = Subject->FindComponentByClass<UHealthComponent>())
	{
		HealthComp->ApplyDamage(Damage);

		if (HealthComp->IsHealthDepleted())
		{
			if (Subject && !Subject->IsActorBeingDestroyed())
			{
				if (auto CombatComponent = Subject->FindComponentByClass<UCombatComponent>())
				{
					auto DieAction = NewObject<UActionDie>();
					DieAction->Agent = Agent;
					DieAction->Subject = Subject;
					DieAction->Tool = Tool;
					DieAction->DamageOfKillingBlow = Damage;
					CombatComponent->AddAction(DieAction);
				}
			}
		}
	}
}

void UActionDie::Execute()
{
	Print("Action Die: Tool: " + Tool->GetName() + ", Subject: " + Subject->GetName() + ", Damage: " + FString::FromInt(DamageOfKillingBlow), FColor::Red);

	auto Event = FGameplayEvent(Agent, ENativeEventType::Kill, Subject);
	Event.Tool = Tool;
	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(Event);

	/// DIE Commands?
	if (Subject && !Subject->IsActorBeingDestroyed())
	{
		auto Ship = Cast<ASpaceship>(Subject);

		if (Ship && Ship->IsPlayerControlled())
		{
			ALunariaGameModeBase::Get(Subject->GetWorld())->OnPlayerDeath();
		}
		else
		{
			Subject->Destroy();
		}
	}
}

void UActionUseAbility::Execute()
{
	Print("Action Ability: Tool: " + Tool->GetName(), FColor::Blue);

	auto Event = FGameplayEvent(Agent, ENativeEventType::AbilityUsed, nullptr);
	Event.Tool = Tool;
	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(Event);

	if (auto Ability = Cast<AAbility>(Tool))
	{
		Ability->Execute();
	}
}

void UActionCreateAreaOfEffect::PerformAreaOfEffect(AActor* Agent, AActor* Tool, TSubclassOf<class AAreaOfEffect> AreaOfEffectClass, const FVector& Location, float Damage, float Radius, float Delay)
{
	auto Action = NewObject<UActionCreateAreaOfEffect>();
	Action->Agent = Agent;
	Action->AreaOfEffectClass = AreaOfEffectClass;
	Action->Subject = nullptr;
	Action->Tool = Tool;
	Action->Location = Location;
	Action->Damage = Damage;
	Action->Radius = Radius;
	Action->Delay = Delay;

	if (auto CombatComponent = Agent->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->AddAction(Action);
	}
}

void UActionCreateAreaOfEffect::Execute()
{
	Print("aoe action from " + Tool->GetName() + " -> " + AreaOfEffectClass->GetName(), FColor::Orange);

	auto AreaOfEffectSpawn = Agent->GetWorld()->SpawnActor<AAreaOfEffect>(AreaOfEffectClass, FTransform(Location));
	AreaOfEffectSpawn->Agent = Agent;
	AreaOfEffectSpawn->Tool = Tool;

	if (auto Ability = Cast<AAbility>(Tool))
	{
		AreaOfEffectSpawn->SetColor(Ability->GetAbilityColor());
	}

	if (auto Attributes = Agent->FindComponentByClass<UAttributesComponent>())
	{
		Attributes->AreaOfEffectDelay->Set(Delay);
		Delay = Attributes->AreaOfEffectDelay->Render(this);

		Attributes->AreaOfEffectRadius->Set(Radius);
		Radius = Attributes->AreaOfEffectRadius->Render(this);
	}

	AreaOfEffectSpawn->Launch(FGameplayTagContainer(), TArray<TSubclassOf<ABoon>>(), Damage, Radius, Delay);
}