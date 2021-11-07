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

	bool IsDetailOn() { return GlobalDetailFlag; }

protected:
	void PlayerTick(float DeltaTime) override;
	void SetupInputComponent() override;
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

	void TickInteractions();

	UFUNCTION()
		void HandleDebugAction();

	UFUNCTION()
		void HandleAttackAbilityAction();

	UFUNCTION()
		void HandleSpecialAbilityAction();

	UFUNCTION()
		void HandleTargetingAbilityActionPressed();

	UFUNCTION()
		void HandleTargetingAbilityActionReleased();

	UFUNCTION()
		void HandleBlinkAbilityActionPressed();

	UFUNCTION()
		void HandleBlinkAbilityActionReleased();

	UFUNCTION()
		void HandleInteractActionPressed();

	UFUNCTION()
		void HandleMovedUpDown(float Scale);

	UFUNCTION()
		void HandleMovedRightLeft(float Scale);

	UFUNCTION()
		void HandleToggleDetail();

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
		class UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UBoon> BoonClass;

	UPROPERTY()
		TArray<AActor*> InteractableActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UButtonTooltip> ButtonTooltipClass;

	UPROPERTY()
		class UButtonTooltip* ButtonTooltipWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		bool GlobalDetailFlag = false;

	float UpDownImpulse{ 0.f };
	float RightLeftImpulse{ 0.f };

	class ASpaceship* Spaceship;
};
