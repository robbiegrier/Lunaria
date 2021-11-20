// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Boon.h"
#include "AreaOfEffect.generated.h"

UCLASS()
class LUNARIA_API AAreaOfEffect : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AAreaOfEffect();
	virtual void Tick(float DeltaTime) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; }

	UFUNCTION(BlueprintCallable)
		void SetColor(const FLinearColor& InColor);

	UFUNCTION(BlueprintCallable)
		void Launch(const FGameplayTagContainer& TagContainer, const TArray<TSubclassOf<ABoon>>& StatusesToApply, float InDamage = 20.f, float InRadius = 150.f, float InDelay = 0.5f);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float DamagePayload = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
		float Thickness = 1.0f;

	float FrameRadius = 0.f;
};
