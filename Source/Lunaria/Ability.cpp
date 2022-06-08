// Robbie Grier

#include "Ability.h"
#include "Printer.h"
#include "Helpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "AttributesComponent.h"
#include "GameplayEventManager.h"
#include "LunariaLib.h"

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

	auto Now = GetWorld()->GetTimeSeconds();

	auto MaxCharges = GetCooldownCharges();

	if (CurrentCharges != MaxCharges)
	{
		if (Now - LastUsed > GetCooldown())
		{
			if (AbilityTag == FGameplayTag::RequestGameplayTag(TEXT("Ability.Defensive")))
			{
				Print("Regenerated Charge for ability with cooldown " + FString::Printf(TEXT("%f"), GetCooldown()));
			}
			CurrentCharges = MaxCharges;
		}
	}

	if (Toggled)
	{
		TickOn(DeltaTime);
	}
	else
	{
		TickOff(DeltaTime);
	}
}

void AAbility::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
	TagContainer.AddTag(AbilityTag);
}

void AAbility::Attach(AActor* InOwner, EAbilityKey Key)
{
	MyOwner = InOwner;

	switch (Key)
	{
	case EAbilityKey::A:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Movement"));
		break;
	case EAbilityKey::B:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Defensive"));
		break;
	case EAbilityKey::X:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"));
		break;
	case EAbilityKey::Y:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Special"));
		break;
	case EAbilityKey::LT:
		AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Other"));
		break;
	default:
		break;
	}
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

FLinearColor AAbility::GetAbilityColor() const
{
	if (MyOwner)
	{
		return GetAttributes()->GetColor(AbilityTag);
	}

	return FLinearColor();
}

void AAbility::ExecuteContext()
{
	OnExecute();
	UpdateCooldownOnExecute();
	PlaySound(AbilityUsedSounds, AudioPlayer);

	if (AutoGenerateEvent)
	{
		auto Event = FGameplayEvent();
		Event.Agent = MyOwner;
		Event.Action = ENativeEventType::AbilityUsed;
		Event.EventTags = ULunariaLib::GetTags(this);
		AGameplayEventManager::Submit(this, Event);
	}
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

ASpaceProjectile* AAbility::CreateAbilityProjectile(TSubclassOf<ASpaceProjectile> SpawnClass, float Damage, float Distance)
{
	return CreateAbilityProjectileWithTransform(SpawnClass, GetTransform(), Damage, Distance);
}

ASpaceProjectile* AAbility::CreateAbilityProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float Damage, float Distance)
{
	auto Params = FActorSpawnParameters();
	Params.Owner = MyOwner;
	auto Projectile = GetWorld()->SpawnActor<ASpaceProjectile>(SpawnClass, Transform, Params);

	if (Projectile)
	{
		Projectile->SetPayloadProperties(ULunariaLib::GetTags(this), Damage, Distance, GetAbilityColor());
	}

	return Projectile;
}

bool AAbility::IsOffCooldown() const
{
	return CurrentCharges > 0;
}

void AAbility::UpdateCooldownOnExecute()
{
	--CurrentCharges;
	LastUsed = GetWorld()->GetTimeSeconds();
}

bool AAbility::ShouldAiUse() const
{
	return IsOffCooldown() && ShouldBeUsed();
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