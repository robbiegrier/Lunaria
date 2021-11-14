// Robbie Grier

#include "GameplayEventManager.h"
#include "Printer.h"
#include "Engine/World.h"
#include "LunariaGameModeBase.h"
#include "HealthComponent.h"
#include "SpaceProjectile.h"
#include "AttributesComponent.h"
#include "GameplayEventObserver.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"

AGameplayEventManager::AGameplayEventManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	RootComponent = SceneComponent;

	DebugWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Debug Widget Component"));
	DebugWidgetComponent->SetupAttachment(RootComponent);

	DebugWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DebugWidgetComponent->SetDrawAtDesiredSize(true);
}

void AGameplayEventManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto GameMode = ALunariaGameModeBase::Get(GetWorld()))
	{
		DebugWidgetComponent->SetWidgetClass(GameMode->GetDebugWidgetClass());
	}

	auto CullDirective = [this] {
		CullHangingDelegates(AgentOfClassDelegates);
		CullHangingDelegates(SubjectOfClassDelegates);
	};

	GetWorld()->GetTimerManager().SetTimer(DelegateCullTimerHandle, CullDirective, CullInterval, true, CullInterval);
}

void AGameplayEventManager::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	DelegateCullTimerHandle.Invalidate();
}

void AGameplayEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessGameplayEvents();

	if (auto DebugWidget = Cast<UDebugWidget>(DebugWidgetComponent->GetUserWidgetObject()))
	{
		DebugWidget->SetText("Gameplay Event Manager");

		auto DelegateMapDebug = FString("Delegate Map:\n");
		DelegateMapDebug += "\tAgent Of Map:\n";
		for (auto& ActionMapping : AgentOfClassDelegates)
		{
			auto Action = ActionMapping.Key;
			DelegateMapDebug += "\t\t";
			DelegateMapDebug += Action == ENativeEventType::Hit ? "Hit" : Action == ENativeEventType::Kill ? "Kill" : "Other";
			DelegateMapDebug += ":\n";

			for (auto& ClassMapping : ActionMapping.Value)
			{
				DelegateMapDebug += "\t\t\t";
				auto ClassName = ClassMapping.Key->GetName();
				DelegateMapDebug += ClassName + ": " + FString::FromInt(ClassMapping.Value.Num());
				DelegateMapDebug += "\n";
			}
		}

		DelegateMapDebug += "\n";

		DelegateMapDebug += "\tSubject Of Map:\n";
		for (auto& ActionMapping : SubjectOfClassDelegates)
		{
			auto Action = ActionMapping.Key;
			DelegateMapDebug += "\t\t";
			DelegateMapDebug += Action == ENativeEventType::Hit ? "Hit" : Action == ENativeEventType::Kill ? "Kill" : "Other";
			DelegateMapDebug += ":\n";

			for (auto& ClassMapping : ActionMapping.Value)
			{
				DelegateMapDebug += "\t\t\t";
				auto ClassName = ClassMapping.Key->GetName();
				DelegateMapDebug += ClassName + ": " + FString::FromInt(ClassMapping.Value.Num());
				DelegateMapDebug += "\n";
			}
		}

		DebugWidget->SetSubText(DelegateMapDebug);
	}
}

AGameplayEventManager* AGameplayEventManager::Get(AActor* ActorContext)
{
	return Get(ActorContext->GetWorld());
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

void AGameplayEventManager::Submit(AActor* ClientObject, const FGameplayEvent& Event)
{
	auto Instance = Get(ClientObject);
	Instance->SubmitEvent(Event);
}

void AGameplayEventManager::ProcessGameplayEvents()
{
	while (Events.Num() > 0)
	{
		auto Event = Events.Pop();

		TriggerAgentObservation(Event);
		TriggerSubjectObservation(Event);
		TriggerAgentOfClassDelegates(Event);
		TriggerSubjectOfClassDelegates(Event);

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

void AGameplayEventManager::ProcessKillEvent(const FGameplayEvent& Event)
{
	Print("KO!");
	Event.Subject->Destroy();
}

void AGameplayEventManager::WhenClassAgentOf(ENativeEventType Action, UClass* Class, AActor* ClientObject, FGameplayEventDynamicDelegate ClientFunction)
{
	if (ClientObject)
	{
		if (auto Instance = Get(ClientObject))
		{
			Instance->AddClassEventDelegate(Instance->AgentOfClassDelegates, Action, Class, ClientFunction);
		}
	}
}

void AGameplayEventManager::WhenClassSubjectOf(ENativeEventType Action, UClass* Class, AActor* ClientObject, FGameplayEventDynamicDelegate ClientFunction)
{
	if (ClientObject)
	{
		if (auto Instance = Get(ClientObject))
		{
			Instance->AddClassEventDelegate(Instance->SubjectOfClassDelegates, Action, Class, ClientFunction);
		}
	}
}

void AGameplayEventManager::AddClassEventDelegate(ClassDelegateMapType& Map, ENativeEventType Action, UClass* Class, FGameplayEventDynamicDelegate ClientFunction)
{
	Map.FindOrAdd(Action).FindOrAdd(Class).Add(ClientFunction);
}

void AGameplayEventManager::TriggerAgentOfClassDelegates(const FGameplayEvent& Event)
{
	if (Event.Agent)
	{
		BroadcastClassEventDelegate(AgentOfClassDelegates, Event.Agent->GetClass(), Event);
	}
}

void AGameplayEventManager::TriggerSubjectOfClassDelegates(const FGameplayEvent& Event)
{
	if (Event.Subject)
	{
		BroadcastClassEventDelegate(SubjectOfClassDelegates, Event.Subject->GetClass(), Event);
	}
}

void AGameplayEventManager::BroadcastClassEventDelegate(ClassDelegateMapType& Map, UClass* Class, const FGameplayEvent& Event)
{
	if (auto FoundClassMapForAction = Map.Find(Event.Action))
	{
		const auto& ClassMap = *FoundClassMapForAction;

		for (const auto& Pair : ClassMap)
		{
			const auto& ClassKey = Pair.Key;

			if (Class->IsChildOf(ClassKey))
			{
				// Using for-ranged loop triggers false positive for changing the array
				for (auto i = 0; i < Pair.Value.Num(); i++)
				{
					const auto& Delegate = Pair.Value[i];
					Delegate.ExecuteIfBound(Event);
				}
			}
		}
	}
}

void AGameplayEventManager::CullHangingDelegates(ClassDelegateMapType& Map)
{
	for (auto& ActionClassMapPair : Map)
	{
		for (auto& ClassDelegateMapPair : ActionClassMapPair.Value)
		{
			auto& Delegates = ClassDelegateMapPair.Value;

			Delegates.RemoveAll([](const auto& Delegate) {
				return !Delegate.IsBound();
			});
		}
	}
}