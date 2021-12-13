// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "User.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API AUser : public APlayerController
{
	GENERATED_BODY()

public:
	AUser();
	bool IsDetailOn() const { return GlobalDetailFlag; }

	void MakeSelectionFromPickup(class APickup* Pickup);

protected:
	void PlayerTick(float DeltaTime) override;
	void SetupInputComponent() override;
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

	void TickInteractions();

	// Misc inputs
	void HandleDebugAction();
	void HandleInteractActionPressed();
	void HandleMovedUpDown(float Scale);
	void HandleMovedRightLeft(float Scale);
	void HandleToggleDetail();

	// Xbox button names are used to describe controls in code
	void HandleXPressed();
	void HandleXReleased();
	void HandleBPressed();
	void HandleBReleased();
	void HandleAPressed();
	void HandleAReleased();
	void HandleYPressed();
	void HandleYReleased();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ABoon> BoonClass;

	UPROPERTY()
		TArray<AActor*> InteractableActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UButtonTooltip> ButtonTooltipClass;

	UPROPERTY()
		class UButtonTooltip* ButtonTooltipWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		bool GlobalDetailFlag = false;

	UPROPERTY()
		class UPickupSelectionWidget* PickupSelectionWidget;

	float UpDownImpulse{ 0.f };
	float RightLeftImpulse{ 0.f };
	class ASpaceship* Spaceship;

	class ABoon* BoonSpawn;
};
