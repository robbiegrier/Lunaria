// Robbie Grier

#include "Ability.h"
#include "Printer.h"
#include "Helpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "AttributesComponent.h"
#include "GameplayEventManager.h"
#include "LunariaLib.h"
#include "AbilitySlot.h"
#include "AbilitiesComponent.h"
#include "Action.h"
#include "CombatComponent.h"

AAbility::AAbility()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Level Component"));
	RootComponent = Root;

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioPlayer->SetupAttachment(RootComponent);
	AudioPlayer->SetAutoActivate(false);
	AudioPlayer->SetVolumeMultiplier(0.3f);

	ContextAudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Context Audio Component"));
	ContextAudioPlayer->SetupAttachment(RootComponent);
	ContextAudioPlayer->SetAutoActivate(false);
	ContextAudioPlayer->SetVolumeMultiplier(0.3f);
}

AActor* AAbility::GetAgent() const
{
	return GetSlot()->GetParent()->GetOwner();
}

void AAbility::BeginPlay()
{
	Super::BeginPlay();
	UpdateStrategy(ExecutionType);

	CurrentCharges = GetCooldownCharges();
	LastUsed = -9999.f;
}

void AAbility::UpdateStrategy(EAbilityExecution Type)
{
	ExecutionType = Type;

	switch (ExecutionType)
	{
	case EAbilityExecution::Queue:
		AbilityStrategy = MakeShared<FAbilityStrategyQueue>();
		break;
	case EAbilityExecution::Hold:
		AbilityStrategy = MakeShared<FAbilityStrategyHold>();
		break;
	case EAbilityExecution::Toggle:
		AbilityStrategy = MakeShared<FAbilityStrategyToggle>();
		break;
	default:
		ExecutionType = EAbilityExecution::Instant;
		AbilityStrategy = MakeShared<FAbilityStrategyInstant>();
		break;
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

void AAbility::Attach(class UAbilitySlot* InSlot)
{
	Slot = InSlot;
	MyOwner = InSlot->GetParent()->GetOwner();
	AbilityTag = FGameplayTag::RequestGameplayTag("Attack");
	Key = EAbilityKey::X;
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
	/*if (MyOwner)
	{
		return GetAttributes()->GetColor(AbilityTag);
	}

	return FLinearColor();*/

	return Slot->Color->Render(nullptr);
}

const TArray<class ASpaceProjectile*>& AAbility::GetProjectilesInFlight() const
{
	return ProjectilesInFlight;
}

void AAbility::ExecuteContext()
{
	auto Action = NewObject<UActionUseAbility>();
	Action->Agent = GetSlot()->GetParent()->GetOwner();
	Action->Subject = this;
	Action->Tool = this;

	Action->Agent->FindComponentByClass<UCombatComponent>()->AddAction(Action);
}

void AAbility::Execute()
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

ASpaceProjectile* AAbility::CreateAbilityProjectile(TSubclassOf<ASpaceProjectile> SpawnClass, float InDamage, float Distance)
{
	return CreateAbilityProjectileWithTransform(SpawnClass, GetTransform(), InDamage, Distance);
}

ASpaceProjectile* AAbility::CreateAbilityProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float InDamage, float Distance)
{
	//auto Params = FActorSpawnParameters();
	//Params.Owner = MyOwner;
	//auto Projectile = GetWorld()->SpawnActor<ASpaceProjectile>(SpawnClass, Transform, Params);

	//if (Projectile)
	//{
	//	Projectile->SetPayloadProperties(ULunariaLib::GetTags(this), InDamage, Distance, GetAbilityColor());
	//}

	//AddProjectile(Projectile);

	//return Projectile;

	Print("ability proj create is deprecated: " + GetMyOwner()->GetName());

	return nullptr;
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

void AAbility::AddProjectile(ASpaceProjectile* Projectile)
{
	if (Projectile)
	{
		ProjectilesInFlight.Add(Projectile);
		Projectile->SetAbilityCreatedFrom(this);
	}
}

void AAbility::OnProjectileEnd(ASpaceProjectile* Projectile)
{
	ProjectilesInFlight.Remove(Projectile);
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