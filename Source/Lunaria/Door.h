// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableState.h"
#include "MapManager.h"
#include "Archetype.h"
#include "Door.generated.h"

UCLASS()
class LUNARIA_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();
	virtual void Tick(float DeltaTime) override;
	virtual FString GetInteractionText() override;
	virtual bool ShowButtonWidget() override;
	virtual void Interact() override;

	// Returns true if the door contains a Map Description entry.
	bool ContainsMapDescription() const;

	// Randomly get one of the Map Descriptions in the door or the global map Id.
	FMapDescription GetMapDescription() const;

	// Open the door.
	UFUNCTION(BlueprintCallable)
		void Open();

	// Close the door.
	UFUNCTION(BlueprintCallable)
		void Close();

	UFUNCTION(BlueprintCallable)
		EArchetype GetArchetype() const { return Archetype; }

	UFUNCTION(BlueprintCallable)
		void SetArchetype(EArchetype InArchetype);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	// The set of maps that can be entered from this door.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
		TArray<FMapDescription> MapDescriptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
		FString MapDescriptionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
		EArchetype Archetype;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* SpriteWidgetComponent;

	// Should the door be open on spawn.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
		bool IsOpen = false;

	TSharedPtr<class InteractableDoorClosedState> ClosedState;
	TSharedPtr<class InteractableDoorOpenState> OpenState;
	TSharedPtr<class InteractableState> CurrentState;

	UPROPERTY()
		AActor* WorldSprite;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnDoorInteractWhenClosed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnDoorInteractWhenOpen();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnOpen();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnClose();
};

/// Door States

class InteractableDoorClosedState : public InteractableState
{
public:
	InteractableDoorClosedState() {
		Message = "Closed Door!";
		CanInteract = false;
	}
	void Execute(class AActor* Subject) override;
};

class InteractableDoorOpenState : public InteractableState
{
public:
	InteractableDoorOpenState() {
		Message = "Enter?";
		CanInteract = true;
	}
	void Execute(class AActor* Subject) override;
};