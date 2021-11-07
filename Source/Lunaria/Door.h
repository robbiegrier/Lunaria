// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableState.h"
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

	UFUNCTION(BlueprintCallable)
		void Open();

	UFUNCTION(BlueprintCallable)
		void Close();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	TSharedPtr<class InteractableDoorClosedState> ClosedState;
	TSharedPtr<class InteractableDoorOpenState> OpenState;
	TSharedPtr<class InteractableState> CurrentState;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnDoorInteractWhenClosed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action Events")
		void OnDoorInteractWhenOpen();
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