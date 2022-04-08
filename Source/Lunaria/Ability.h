// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Ability.generated.h"

UENUM()
enum class EAbilityExecution : uint8
{
	Instant = 0,
	Queue = 1,
	Toggle = 2,
	Hold = 3,
	Passive = 4
};

UCLASS()
class LUNARIA_API AAbility : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AAbility();
	virtual void Tick(float DeltaTime) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	void Attach(AActor* InOwner);

	virtual void Press();
	virtual void Release();

	virtual void Queue();
	virtual void EndQueue();

	virtual void ToggleOn();
	virtual void ToggleOff();

	virtual void ExecuteContext();

	virtual void UpdateCooldownOnExecute();

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
		float GetAttributeValue(const FString& Attribute, float Seed) const;

	UFUNCTION(BlueprintCallable)
		float GetAttributeValueFromTag(const FGameplayTag& Attribute, float Seed) const;

	UFUNCTION(BlueprintCallable)
		FLinearColor GetAbilityColor() const;

protected:
	virtual void BeginPlay() override;

	void PlaySound(const TArray<class USoundBase*>& Sounds, class UAudioComponent* Player);
	void StopSound(class UAudioComponent* Player);

private:
	void UpdateStrategy(TEnumAsByte<EAbilityExecution> Type);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EAbilityExecution> ExecutionType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cooldown, meta = (AllowPrivateAccess = "true"))
		int32 CurrentCharges = CooldownChargesSeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		FGameplayTagContainer GameplayTags;

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
