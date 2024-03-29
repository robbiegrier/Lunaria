// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "SpaceProjectile.h"
#include "Tool.h"
#include "Ability.generated.h"

UENUM(BlueprintType)
enum class EAbilityExecution : uint8
{
	Instant = 0,
	Queue = 1,
	Toggle = 2,
	Hold = 3,
	Passive = 4
};

UENUM(BlueprintType)
enum class EAbilityKey : uint8
{
	A, B, X, Y, LT
};

UCLASS()
class LUNARIA_API AAbility : public ATool, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AAbility();
	virtual void Tick(float DeltaTime) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual FLinearColor GetColor() override;

	void Attach(class UAbilitySlot* InSlot);

	virtual void Press();
	virtual void Release();

	virtual void Queue();
	virtual void EndQueue();

	virtual void ToggleOn();
	virtual void ToggleOff();

	virtual void ExecuteContext();
	virtual void Execute();
	virtual void UpdateCooldownOnExecute();
	void AddProjectile(class ASpaceProjectile* Projectile);
	void OnProjectileEnd(class ASpaceProjectile* Projectile);

	UFUNCTION(BlueprintCallable)
		virtual bool ShouldAiUse() const;

	UFUNCTION(BlueprintCallable)
		virtual bool IsOffCooldown() const;

	UFUNCTION(BlueprintCallable)
		bool IsToggled() const { return Toggled; }

	UFUNCTION(BlueprintCallable)
		class UAttributesComponent* GetAttributes() const;

	UFUNCTION(BlueprintCallable)
		float GetCooldown();

	UFUNCTION(BlueprintCallable)
		int32 GetCooldownCharges();

	UFUNCTION(BlueprintCallable)
		FLinearColor GetAbilityColor() const;

	UFUNCTION(BlueprintCallable)
		const TArray<class ASpaceProjectile*>& GetProjectilesInFlight() const;

	UFUNCTION(BlueprintCallable)
		AActor* GetMyOwner() const { return MyOwner; }

	UFUNCTION(BlueprintCallable)
		const FGameplayTag& GetAbilityTag() const { return AbilityTag; }

	UFUNCTION(BlueprintCallable)
		class UAbilitySlot* GetSlot() const { return Slot; }

	UFUNCTION(BlueprintCallable)
		float GetDamage() { return Damage; }

protected:
	virtual void BeginPlay() override;

	void PlaySound(const TArray<class USoundBase*>& Sounds, class UAudioComponent* Player);
	void StopSound(class UAudioComponent* Player);

	UFUNCTION(BlueprintCallable)
		class ASpaceProjectile* CreateAbilityProjectile(TSubclassOf<ASpaceProjectile> SpawnClass, float Damage = 50.f, float Distance = 3000.f, float Speed = 1000.f);

	UFUNCTION(BlueprintCallable)
		class ASpaceProjectile* CreateAbilityProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float Damage = 50.f, float Distance = 3000.f, float Speed = 1000.f);

private:
	void UpdateStrategy(EAbilityExecution Type);

	// native damage of the ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
		float Damage = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
		class UAbilitySlot* Slot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		EAbilityExecution ExecutionType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Event, meta = (AllowPrivateAccess = "true"))
		bool AutoGenerateEvent = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
		bool Toggled = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* ContextAudioPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		TArray<class USoundBase*> AbilityUsedSounds;

	// Secondary sound depending on ability type. (queued, un-toogle, release, etc)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		TArray<class USoundBase*> ContextSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cooldown, meta = (AllowPrivateAccess = "true"))
		float CooldownSeed = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Cooldown, meta = (AllowPrivateAccess = "true"))
		int32 CooldownChargesSeed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cooldown, meta = (AllowPrivateAccess = "true"))
		float LastUsed = -CooldownSeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
		int32 CurrentCharges = CooldownChargesSeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
		FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectiles, meta = (AllowPrivateAccess = "true"))
		TArray<class ASpaceProjectile*> ProjectilesInFlight;

	UPROPERTY()
		class UAction* GeneralAction;

	EAbilityKey Key;

	TSharedPtr<class FAbilityStrategy> AbilityStrategy;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnQueue();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnQueueEnded();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnToggleOn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnToggleOff();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnExecute();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tick Events")
		void TickOn(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tick Events")
		void TickOff(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Ai Checks")
		bool ShouldBeUsed() const;
	bool ShouldBeUsed_Implementation() const {
		return true;
	}
};

class FAbilityStrategy
{
public:
	virtual ~FAbilityStrategy() = default;
	void SetAbility(class AAbility* InAbility) { Ability = InAbility; }

	virtual void OnKeyPressed() = 0;
	virtual void OnKeyReleased() = 0;

protected:
	class AAbility* Ability;
};

class FAbilityStrategyQueue : public FAbilityStrategy
{
public:
	virtual void OnKeyPressed() override;
	virtual void OnKeyReleased() override;
};

class FAbilityStrategyInstant : public FAbilityStrategy
{
public:
	virtual void OnKeyPressed() override;
	virtual void OnKeyReleased() override;
};

class FAbilityStrategyToggle : public FAbilityStrategy
{
public:
	virtual void OnKeyPressed() override;
	virtual void OnKeyReleased() override;
};

class FAbilityStrategyHold : public FAbilityStrategy
{
public:
	virtual void OnKeyPressed() override;
	virtual void OnKeyReleased() override;
};

class FAbilityStrategyPassive : public FAbilityStrategy
{
public:
	virtual void OnKeyPressed() override {}
	virtual void OnKeyReleased() override {}
};
