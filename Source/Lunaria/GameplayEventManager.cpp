// Robbie Grier

#include "GameplayEventManager.h"
#include "Printer.h"
#include "Engine/World.h"
#include "LunariaGameModeBase.h"
#include "HealthComponent.h"
#include "SpaceProjectile.h"
#include "AttributesComponent.h"

AGameplayEventManager::AGameplayEventManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayEventManager::BeginPlay()
{
	Super::BeginPlay();
}

void AGameplayEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessGameplayEvents();
}

AGameplayEventManager* AGameplayEventManager::Get(UWorld* WorldContext)
{
	if (auto Mode = Cast<ALunariaGameModeBase>(WorldContext->GetAuthGameMode()))
	{
		if (auto Manager = Mode->GetEventManager())
		{
			return Manager;
		}
		else
		{
			auto NewManager = WorldContext->SpawnActor<AGameplayEventManager>();
			Mode->SetEventManager(NewManager);
			return NewManager;
		}
	}

	return nullptr;
}

void AGameplayEventManager::SubmitEvent(const FGameplayEvent& Event)
{
	Events.Add(Event);
}

void AGameplayEventManager::ProcessGameplayEvents()
{
	while (Events.Num() > 0)
	{
		auto Event = Events.Pop();

		if (Event.EventType == EGameplayEventType::Hit)
		{
			ProcessHitEvent(Event);
		}
		else if (Event.EventType == EGameplayEventType::Kill)
		{
			ProcessKillEvent(Event);
		}
	}
}

void AGameplayEventManager::ProcessHitEvent(const FGameplayEvent& Event)
{
	if (auto HealthComp = Event.Subject->FindComponentByClass<UHealthComponent>())
	{
		if (auto AgentAttributes = Event.Agent->FindComponentByClass<UAttributesComponent>())
		{
			if (auto Projectile = Cast<ASpaceProjectile>(Event.Medium))
			{
				auto Damage = AgentAttributes->Get(*Event.Tags.Find("Ability Type") + " Ability Damage", Projectile->GetDamage());
				HealthComp->ApplyDamage(Damage);

				if (HealthComp->IsHealthDepleted())
				{
					auto KillEvent = Event;
					KillEvent.EventType = EGameplayEventType::Kill;
					SubmitEvent(KillEvent);
				}
			}
		}
	}
}

void AGameplayEventManager::ProcessKillEvent(const FGameplayEvent& Event)
{
	Print("KO!");
	Event.Subject->Destroy();
}