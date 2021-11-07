// Robbie Grier

#include "Ability.h"
#include "Printer.h"
#include "Helpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

AAbility::AAbility()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Level Component"));
	RootComponent = Root;

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioPlayer->SetupAttachment(RootComponent);
	AudioPlayer->SetAutoActivate(false);

	ContextAudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Context Audio Component"));
	ContextAudioPlayer->SetupAttachment(RootComponent);
	ContextAudioPlayer->SetAutoActivate(false);
}

void AAbility::BeginPlay()
{
	Super::BeginPlay();
	UpdateStrategy(ExecutionType);
}

void AAbility::UpdateStrategy(TEnumAsByte<EAbilityExecution> Type)
{
	ExecutionType = Type;

	if (ExecutionType == EAbilityExecution::Queue)
	{
		AbilityStrategy = MakeShared<FAbilityStrategyQueue>();
	}
	else if (ExecutionType == EAbilityExecution::Hold)
	{
		AbilityStrategy = MakeShared<FAbilityStrategyHold>();
	}
	else if (ExecutionType == EAbilityExecution::Toggle)
	{
		AbilityStrategy = MakeShared<FAbilityStrategyToggle>();
	}
	else
	{
		ExecutionType = EAbilityExecution::Instant;
		AbilityStrategy = MakeShared<FAbilityStrategyInstant>();
	}

	AbilityStrategy->SetAbility(this);
}

void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Toggled)
	{
		TickOn(DeltaTime);
	}
	else
	{
		TickOff(DeltaTime);
	}
}

void AAbility::Attach(AActor* InOwner)
{
	MyOwner = InOwner;
}

void AAbility::ExecuteContext()
{
	OnExecute();
	PlaySound(AbilityUsedSounds, AudioPlayer);
}

void AAbility::Queue()
{
	Toggled = true;
	OnQueue();
	PlaySound(ContextSounds, ContextAudioPlayer);
}

void AAbility::EndQueue()
{
	Toggled = false;
	OnQueueEnded();
	StopSound(ContextAudioPlayer);
}

void AAbility::ToggleOn()
{
	Toggled = true;
	OnToggleOn();
	ExecuteContext();
}

void AAbility::ToggleOff()
{
	Toggled = false;
	OnToggleOff();
	PlaySound(ContextSounds, ContextAudioPlayer);
}

void AAbility::PlaySound(const TArray<USoundBase*>& Sounds, UAudioComponent* Player)
{
	if (Sounds.Num() > 0)
	{
		Player->SetSound(Helpers::GetRandomArrayElement(Sounds));
		Player->Play();
	}
}

void AAbility::StopSound(UAudioComponent* Player)
{
	Player->Stop();
}

void AAbility::Press()
{
	AbilityStrategy->OnKeyPressed();
}

void AAbility::Release()
{
	AbilityStrategy->OnKeyReleased();
}

void AAbility::Cancel()
{
}

void FAbilityStrategyQueue::OnKeyPressed()
{
	Ability->Queue();
}

void FAbilityStrategyQueue::OnKeyReleased()
{
	Ability->EndQueue();
	Ability->ExecuteContext();
}

void FAbilityStrategyInstant::OnKeyPressed()
{
	Ability->ExecuteContext();
}

void FAbilityStrategyInstant::OnKeyReleased()
{
	// Do Nothing
}

void FAbilityStrategyToggle::OnKeyPressed()
{
	if (Ability->IsToggled())
	{
		Ability->ToggleOff();
	}
	else
	{
		Ability->ToggleOn();
	}
}

void FAbilityStrategyToggle::OnKeyReleased()
{
	// Do Nothing
}

void FAbilityStrategyHold::OnKeyPressed()
{
	Ability->ToggleOn();
}

void FAbilityStrategyHold::OnKeyReleased()
{
	Ability->ToggleOff();
}