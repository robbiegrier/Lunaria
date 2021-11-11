// Robbie Grier

#include "GameplayEventManager.h"
#include "Printer.h"
#include "Engine/World.h"
#include "LunariaGameModeBase.h"
#include "HealthComponent.h"
#include "SpaceProjectile.h"
#include "AttributesComponent.h"
#include "GameplayEventObserver.h"

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

		TriggerAgentObservation(Event);
		TriggerSubjectObservation(Event);

		if (Event.Action == ENativeEventType::Hit)
		{
			ProcessHitEvent(Event);
		}
		else if (Event.Action == ENativeEventType::Kill)
		{
			ProcessKillEvent(Event);
		}
	}
}

void AGameplayEventManager::TriggerSubjectObservation(const FGameplayEvent& Event)
{
	if (auto Actor = Event.Subject)
	{
		if (auto Observer = Cast<IGameplayEventObserver>(Actor))
		{
			Observer->ExecuteSubjectOf(Event);
		}

		for (auto Component : Actor->GetComponentsByInterface(UGameplayEventObserver::StaticClass()))
		{
			Cast<IGameplayEventObserver>(Component)->ExecuteSubjectOf(Event);
		}
	}
}

void AGameplayEventManager::TriggerAgentObservation(const FGameplayEvent& Event)
{
	if (auto Actor = Event.Agent)
	{
		if (auto Observer = Cast<IGameplayEventObserver>(Actor))
		{
			Observer->ExecuteAgentOf(Event);
		}

		for (auto Component : Actor->GetComponentsByInterface(UGameplayEventObserver::StaticClass()))
		{
			Cast<IGameplayEventObserver>(Component)->ExecuteAgentOf(Event);
		}
	}
}

void AGameplayEventManager::ProcessHitEvent(const FGameplayEvent& Event)
{
	if (auto HealthComp = Event.Subject->FindComponentByClass<UHealthComponent>())
	{
		if (auto AgentAttributes = Event.Agent->FindComponentByClass<UAttributesComponent>())
		{
			auto FindDamage = Event.Values.Find("Damage");
			auto Damage = FindDamage ? *FindDamage : 0.f;

			HealthComp->ApplyDamage(Damage);

			if (HealthComp->IsHealthDepleted())
			{
				auto KillEvent = Event;
				KillEvent.Action = ENativeEventType::Kill;
				SubmitEvent(KillEvent);
			}
		}
	}
}

void AGameplayEventManager::ProcessKillEvent(const FGameplayEvent& Event)
{
	Print("KO!");
	Event.Subject->Destroy();
}