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

		TriggerDelegates(Event);

		if (Event.EventType == "Hit")
		{
			ProcessHitEvent(Event);
		}
		else if (Event.EventType == "Kill")
		{
			ProcessKillEvent(Event);
		}
	}
}

void AGameplayEventManager::TriggerDelegates(const FGameplayEvent& Event)
{
	TriggerOneToManyDelegates(Event.Agent, Event.EventType, Event.Subject->GetClass(), Event);
	TriggerManyToOneDelegates(Event.Agent->GetClass(), Event.EventType, Event.Subject, Event);
}

void AGameplayEventManager::TriggerOneToManyDelegates(AActor* Agent, const FString& Action, UClass* SubjectClass, const FGameplayEvent& Event)
{
	WithOneToMany(Agent, Action, SubjectClass, [&Event](auto& Delegate) {
		Delegate.Broadcast(Event);
	});
}

void AGameplayEventManager::TriggerManyToOneDelegates(UClass* AgentClass, const FString& Action, AActor* Subject, const FGameplayEvent& Event)
{
	WithManyToOne(AgentClass, Action, Subject, [&Event](auto& Delegate) {
		Delegate.Broadcast(Event);
	});
}

void AGameplayEventManager::WhenOneToMany(AActor* Agent, const FString& Action, UClass* SubjectClass, UObject* Object, const FName& FuncName)
{
	auto& ClassToDelegates = GetOneToManyClassMap(Agent, Action);
	auto Find = ClassToDelegates.Find(SubjectClass);

	if (!Find)
	{
		ClassToDelegates.Add(SubjectClass, FGameplayEventSignature());
	}

	WithOneToMany(Agent, Action, SubjectClass, [Object, &FuncName](auto& Delegate) {
		Delegate.AddUFunction(Object, FuncName);
	});
}

void AGameplayEventManager::WhenManyToOne(UClass* AgentClass, const FString& Action, AActor* Subject, UObject* Object, const FName& FuncName)
{
	WithManyToOne(AgentClass, Action, Subject, [Object, &FuncName](auto& Delegate) {
		Delegate.AddUFunction(Object, FuncName);
	});
}

TMap<UClass*, FGameplayEventSignature>& AGameplayEventManager::GetOneToManyClassMap(AActor* Agent, const FString& Action)
{
	auto Find1 = OneToManyMap.Find(Agent);

	if (!Find1)
	{
		OneToManyMap.Add(Agent, TMap<FString, TMap<UClass*, FGameplayEventSignature>>());
	}

	auto& AgentMap = *OneToManyMap.Find(Agent);

	auto Find2 = AgentMap.Find(Action);

	if (!Find2)
	{
		AgentMap.Add(Action, TMap<UClass*, FGameplayEventSignature>());
	}

	return *AgentMap.Find(Action);
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
				KillEvent.EventType = "Kill";
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