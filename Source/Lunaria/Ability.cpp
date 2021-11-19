// Robbie Grier

#include "Ability.h"
#include "Printer.h"
#include "Helpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "AttributesComponent.h"

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

	AbilityType = "Generic";
}

void AAbility::BeginPlay()
{
	Super::BeginPlay();
	UpdateStrategy(ExecutionType);

	CurrentCharges = GetCooldownCharges();
	LastUsed = -9999.f;
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

UAttributesComponent* AAbility::GetAttributes() const
{
	return MyOwner->FindComponentByClass<UAttributesComponent>();
}

float AAbility::GetCooldown()
{
	return GetAttributeValue("Attribute.Cooldown.Ability", CooldownSeed);
}

int32 AAbility::GetCooldownCharges()
{
	return GetAttributeValue("Attribute.Charges.Ability", CooldownChargesSeed);
}

float AAbility::GetAttributeValue(const FString& Attribute, float Seed) const
{
	return GetAttributeValueFromTag(FGameplayTag::RequestGameplayTag(*Attribute), Seed);
}

float AAbility::GetAttributeValueFromTag(const FGameplayTag& Attribute, float Seed) const
{
	if (MyOwner)
	{
		return GetAttributes()->GetForAbilityType(AbilityTag, Attribute, Seed);
	}

	return 0.f;
}

void AAbility::ExecuteContext()
{
	OnExecute();
	UpdateCooldownOnExecute();
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

bool AAbility::IsOffCooldown()
{
	auto Now = GetWorld()->GetTimeSeconds();

	if (Now - LastUsed > GetCooldown())
	{
		CurrentCharges = GetCooldownCharges();
	}

	return CurrentCharges > 0;
}

void AAbility::UpdateCooldownOnExecute()
{
	--CurrentCharges;
	LastUsed = GetWorld()->GetTimeSeconds();
}

void AAbility::Press()
{
	if (IsOffCooldown())
	{
		AbilityStrategy->OnKeyPressed();
	}
}

void AAbility::Release()
{
	AbilityStrategy->OnKeyReleased();
}

void FAbilityStrategyQueue::OnKeyPressed()
{
	Ability->Queue();
}

void FAbilityStrategyQueue::OnKeyReleased()
{
	if (Ability->IsToggled())
	{
		Ability->EndQueue();
		Ability->ExecuteContext();
	}
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