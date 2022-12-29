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
#include "User.h"
#include "Spaceship.h"

static bool DebugEvents = false;

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
	SetInitialDetailToggleState();
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

	if (DebugWidgetComponent->GetVisibleFlag())
	{
		DebugMaps();
	}
}

void AGameplayEventManager::ToggleDetailOn()
{
	DebugWidgetComponent->SetVisibility(true);
}

void AGameplayEventManager::ToggleDetailOff()
{
	DebugWidgetComponent->SetVisibility(false);
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
	if (ClientObject)
	{
		auto Instance = Get(ClientObject);
		Instance->SubmitEvent(Event);
	}
}

void AGameplayEventManager::CreateEvent(AActor* InAgent, ENativeEventType InAction, AActor* InSubject, const TMap<FString, FString>& InTags,
	const TMap<FString, float>& InValues, const TMap<FString, FVector>& InVectors, const TMap<FString, UClass*>& InClasses, const FGameplayTagContainer& InEventTags)
{
	auto Event = FGameplayEvent(InAgent, InAction, InSubject);
	Event.Tags = InTags;
	Event.Values = InValues;
	Event.Vectors = InVectors;
	Event.Classes = InClasses;
	Event.EventTags = InEventTags;
	Submit(InAgent, Event);
}

void AGameplayEventManager::CreateHit(AActor* InAgent, AActor* InSubject, float Damage, const FGameplayTagContainer& InEventTags)
{
	auto Values = TMap<FString, float>();
	Values.Add("Damage", Damage);
	CreateEvent(InAgent, ENativeEventType::Hit, InSubject, TMap<FString, FString>(), Values, TMap<FString, FVector>(), TMap<FString, UClass*>(), InEventTags);
}

void AGameplayEventManager::CreateHeal(AActor* InAgent, AActor* InSubject, float Scale, const FGameplayTagContainer& InEventTags)
{
	auto Values = TMap<FString, float>();
	Values.Add("Healing", Scale);
	CreateEvent(InAgent, ENativeEventType::Heal, InSubject, TMap<FString, FString>(), Values, TMap<FString, FVector>(), TMap<FString, UClass*>(), InEventTags);
}

void AGameplayEventManager::ApplyStatusEffect(AActor* InAgent, AActor* InSubject, TSubclassOf<ABoon> InEffect, const FGameplayTagContainer& InEventTags)
{
	auto Classes = TMap<FString, UClass*>();
	Classes.Add("StatusEffectClass", InEffect);
	CreateEvent(InAgent, ENativeEventType::ApplyStatus, InSubject, TMap<FString, FString>(), TMap<FString, float>(), TMap<FString, FVector>(), Classes, InEventTags);
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
		if (Event.Action == ENativeEventType::Heal)
		{
			ProcessHealEvent(Event);
		}
		else if (Event.Action == ENativeEventType::Kill)
		{
			ProcessKillEvent(Event);
		}
		else if (Event.Action == ENativeEventType::ApplyStatus)
		{
			ProcessApplyStatusEvent(Event);
		}
		else if (Event.Action == ENativeEventType::AbilityUsed)
		{
			if (DebugEvents) Print(Event.Agent->GetName() + " used " + Event.EventTags.ToStringSimple());
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

		if (auto Pawn = Cast<APawn>(Actor))
		{
			if (auto Observer = Cast<IGameplayEventObserver>(Pawn->GetController()))
			{
				Observer->ExecuteSubjectOf(Event);
			}
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

		if (auto Pawn = Cast<APawn>(Actor))
		{
			if (auto Observer = Cast<IGameplayEventObserver>(Pawn->GetController()))
			{
				Observer->ExecuteAgentOf(Event);
			}
		}
	}
}

void AGameplayEventManager::ProcessHitEvent(const FGameplayEvent& Event)
{
	if (Event.Subject && !Event.Subject->IsActorBeingDestroyed())
	{
		if (auto HealthComp = Event.Subject->FindComponentByClass<UHealthComponent>())
		{
			auto FindScale = Event.Values.Find("Damage");
			auto Scale = FindScale ? *FindScale : 0.f;

			if (Event.Agent)
			{
				// Any tags attached to this hit event should potentially modify the damage scale
				if (auto Attributes = Event.Agent->FindComponentByClass<UAttributesComponent>())
				{
					auto DamageTags = Event.EventTags;
					DamageTags.AddTag(FGameplayTag::RequestGameplayTag("Attribute.Damage"));
					Scale = Attributes->GetFromTagContainer(DamageTags, Scale);
				}
			}

			if (auto Attributes = Event.Subject->FindComponentByClass<UAttributesComponent>())
			{
				auto ReductionPercent = Attributes->Get("Reduction.Damage", 0.f) * 0.01f;
				auto ReductionAmount = Scale * ReductionPercent;
				Scale -= ReductionAmount;

				auto AmpPercent = Attributes->Get("Amplify.Damage", 0.f) * 0.01f;
				auto AmpAmount = Scale * AmpPercent;
				Scale += AmpAmount;
			}

			HealthComp->ApplyDamage(Scale);

			if (HealthComp->IsHealthDepleted())
			{
				auto KillEvent = Event;
				KillEvent.Action = ENativeEventType::Kill;
				SubmitEvent(KillEvent);
			}

			if (DebugEvents) Print(Event.Agent->GetName() + " did " + FString::FromInt((int)Scale) + " damage to " + Event.Subject->GetName() + " (" + Event.EventTags.ToStringSimple() + ")");
		}
	}
}

void AGameplayEventManager::ProcessHealEvent(const FGameplayEvent& Event)
{
	if (Event.Subject && !Event.Subject->IsActorBeingDestroyed())
	{
		if (auto HealthComp = Event.Subject->FindComponentByClass<UHealthComponent>())
		{
			auto FindScale = Event.Values.Find("Healing");
			auto Scale = FindScale ? *FindScale : 0.f;

			if (auto Attributes = Event.Subject->FindComponentByClass<UAttributesComponent>())
			{
				auto ReductionPercent = Attributes->Get("Reduction.Healing", 0.f) * 0.01f;
				auto ReductionAmount = Scale * ReductionPercent;
				Scale -= ReductionAmount;

				auto AmpPercent = Attributes->Get("Amplify.Healing", 0.f) * 0.01f;
				auto AmpAmount = Scale * AmpPercent;
				Scale += AmpAmount;
			}

			HealthComp->ApplyHealing(Scale);

			if (DebugEvents) Print(Event.Agent->GetName() + " provided " + FString::FromInt((int)Scale) + " healing to " + Event.Subject->GetName() + " (" + Event.EventTags.ToStringSimple() + ")");
		}
	}
}

void AGameplayEventManager::ProcessKillEvent(const FGameplayEvent& Event)
{
	if (Event.Subject && !Event.Subject->IsActorBeingDestroyed())
	{
		auto Ship = Cast<ASpaceship>(Event.Subject);

		if (Ship && Ship->IsPlayerControlled())
		{
			ALunariaGameModeBase::Get(GetWorld())->OnPlayerDeath();
		}
		else
		{
			Event.Subject->Destroy();
		}

		if (ShouldWriteKillStatements) Print(Event.Agent->GetName() + " killed " + Event.Subject->GetName() + " (" + Event.EventTags.ToStringSimple() + ")");
	}
}

void AGameplayEventManager::ProcessApplyStatusEvent(const FGameplayEvent& Event)
{
	if (Event.Subject && !Event.Subject->IsActorBeingDestroyed())
	{
		if (auto Attributes = Event.Subject->FindComponentByClass<UAttributesComponent>())
		{
			if (auto Find = Event.Classes.Find("StatusEffectClass"))
			{
				if (*Find)
				{
					Attributes->AddStatusEffectFromClass(*Find, Event.Agent);
					if (DebugEvents) Print(Event.Agent->GetName() + " applied " + (*Find)->GetName() + " to " + Event.Subject->GetName() + " (" + Event.EventTags.ToStringSimple() + ")");
				}
			}
		}
	}
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

FString AGameplayEventManager::PrintDelegateMap(const ClassDelegateMapType& Map)
{
	auto Output = FString();

	for (auto& ActionMapping : Map)
	{
		auto Action = ActionMapping.Key;
		Output += "\t\t";
		Output += Action == ENativeEventType::Hit ? "Hit" : Action == ENativeEventType::Kill ? "Kill" : "Other";
		Output += ":\n";

		for (auto& ClassMapping : ActionMapping.Value)
		{
			Output += "\t\t\t";
			auto ClassName = ClassMapping.Key->GetName();
			Output += ClassName + ": " + FString::FromInt(ClassMapping.Value.Num());
			Output += "\n";
		}
	}

	return Output;
}

void AGameplayEventManager::DebugMaps()
{
	if (auto DebugWidget = Cast<UDebugWidget>(DebugWidgetComponent->GetUserWidgetObject()))
	{
		DebugWidget->SetText("Gameplay Event Manager");

		auto DelegateMapDebug = FString("Delegate Map:\n");
		DelegateMapDebug += "\tAgent Of Map:\n";
		DelegateMapDebug += PrintDelegateMap(AgentOfClassDelegates);

		DelegateMapDebug += "\n";

		DelegateMapDebug += "\tSubject Of Map:\n";
		DelegateMapDebug += PrintDelegateMap(SubjectOfClassDelegates);

		DebugWidget->SetSubText(DelegateMapDebug);
	}
}