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
#include "Medium.h"

void UActionHit::PerformHit(AActor* Agent, AActor* Subject, AActor* Tool, AMedium* Medium)
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

	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(FGameplayEvent(Agent, ENativeEventType::Hit, Subject, Tool, Medium));

	float Damage = AddSubjectDamage(AddAgentDamage(GetBaseDamage()));

	if (Subject && !Subject->IsActorBeingDestroyed())
	{
		if (auto CombatComponent = Subject->FindComponentByClass<UCombatComponent>())
		{
			CombatComponent->AddAction(NewObject<UActionTakeDamage>()->With(Agent, Subject, Tool, Damage));
		}
	}

	if (Medium)
	{
		Medium->OnHitEnd(this);
	}
}

float UActionHit::GetBaseDamage() const
{
	if (Medium)
	{
		return Medium->GetBaseDamage();
	}

	return 0.f;
}

float UActionHit::AddAgentDamage(float Damage) const
{
	if (auto AgentAttr = Agent->FindComponentByClass<UAttributesComponent>())
	{
		AgentAttr->Damage->Set(Damage);
		return AgentAttr->Damage->Render(this);
	}

	return Damage;
}

float UActionHit::AddSubjectDamage(float Damage) const
{
	if (auto SubjectAttr = Subject->FindComponentByClass<UAttributesComponent>())
	{
		SubjectAttr->DamageReceived->Set(Damage);
		return SubjectAttr->DamageReceived->Render(this);
	}

	return Damage;
}

void UActionTakeDamage::Execute()
{
	//Print("Action Take Damage: Tool: " + Tool->GetName() + ", Subject: " + Subject->GetName() + ", Damage: " + FString::FromInt(Damage), FColor::Yellow);

	auto Event = FGameplayEvent(Agent, ENativeEventType::TakeDamage, Subject);
	Event.Tool = Tool;
	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(Event);

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

UActionTakeDamage* UActionTakeDamage::With(AActor* InAgent, AActor* InSubject, AActor* InTool, float InDamage)
{
	Agent = InAgent;
	Subject = InSubject;
	Tool = InTool;
	Damage = InDamage;
	return this;
}

void UActionDie::Execute()
{
	Print("Action Die: Tool: " + Tool->GetName() + ", Subject: " + Subject->GetName() + ", Damage: " + FString::FromInt(DamageOfKillingBlow), FColor::Red);

	auto Event = FGameplayEvent(Agent, ENativeEventType::Kill, Subject);
	Event.Tool = Tool;
	AGameplayEventManager::Get(Agent->GetWorld())->SubmitEvent(Event);

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

	if (auto Attributes = Agent->FindComponentByClass<UAttributesComponent>())
	{
		Attributes->AreaOfEffectDelay->Set(Delay);
		Delay = Attributes->AreaOfEffectDelay->Render(this);

		Attributes->AreaOfEffectRadius->Set(Radius);
		Radius = Attributes->AreaOfEffectRadius->Render(this);
	}

	auto MediumSpawn = Cast<AAreaOfEffect>(AMedium::Spawn(AreaOfEffectClass, Agent, Tool, FTransform(Location), Damage));
	MediumSpawn->SetAreaOfEffectProperties(Radius, Delay);
	MediumSpawn->LaunchCombatMedium();
}