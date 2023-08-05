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
#include "ActionCreateProjectile.h"

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

void AAbility::BeginPlay()
{
	Super::BeginPlay();
	UpdateStrategy(ExecutionType);
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

FLinearColor AAbility::GetColor()
{
	return GetSlot()->Color->Render(nullptr);
}

void AAbility::Attach(class UAbilitySlot* InSlot)
{
	Slot = InSlot;
	MyOwner = InSlot->GetParent()->GetOwner();
	AbilityTag = FGameplayTag::RequestGameplayTag("Attack");
	Key = EAbilityKey::X;

	CurrentCharges = GetCooldownCharges();
	LastUsed = -9999.f;

	GeneralAction = NewObject<UAction>();
	GeneralAction->Agent = Agent;
	GeneralAction->Tool = this;
}

UAttributesComponent* AAbility::GetAttributes() const
{
	return MyOwner->FindComponentByClass<UAttributesComponent>();
}

float AAbility::GetCooldown()
{
	GetAttributes()->Cooldown->Set(CooldownSeed);
	return GetAttributes()->Cooldown->Render(GeneralAction);
}

int32 AAbility::GetCooldownCharges()
{
	GetAttributes()->Charges->Set(CooldownChargesSeed);
	return GetAttributes()->Charges->Render(GeneralAction);
}

FLinearColor AAbility::GetAbilityColor() const
{
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

ASpaceProjectile* AAbility::CreateAbilityProjectile(TSubclassOf<ASpaceProjectile> SpawnClass, float InDamage, float Distance, float Speed)
{
	return CreateAbilityProjectileWithTransform(SpawnClass, GetTransform(), InDamage, Distance, Speed);
}

ASpaceProjectile* AAbility::CreateAbilityProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float InDamage, float Distance, float Speed)
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

	UActionCreateProjectile::PerformCreateProjectile(Agent, this, SpawnClass, Transform, InDamage, Distance, Speed);

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