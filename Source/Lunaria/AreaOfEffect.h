// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Boon.h"
#include "Medium.h"
#include "AreaOfEffect.generated.h"

UCLASS()
class LUNARIA_API AAreaOfEffect : public AMedium, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AAreaOfEffect();
	virtual void Tick(float DeltaTime) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; }
	virtual void SetColor(const FLinearColor& InColor) override;
	virtual void LaunchCombatMedium() override;

	void SetAreaOfEffectProperties(float InRadius = 150.f, float InDelay = 0.5f);

	//UFUNCTION(BlueprintCallable)
	//	float GetDamage() const { return DamagePayload; }

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Owners, meta = (AllowPrivateAccess = "true"))
	//	class AActor* Agent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
	//	class AActor* Tool;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float Delay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (AllowPrivateAccess = "true"))
		float Radius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<ABoon>> StatusPayload;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	//	float DamagePayload = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float Thickness = 1.0f;

	float FrameRadius = 0.f;
};
