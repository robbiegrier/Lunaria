// Robbie Grier

#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Spaceship.h"
#include "GameFramework/PlayerController.h"
#include "Printer.h"
#include "User.h"
#include "LunariaGameModeBase.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	RootComponent = SceneRoot;
	Mesh->SetupAttachment(SceneRoot);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName("Awareness");

	ClosedState = MakeShared<InteractableDoorClosedState>();
	OpenState = MakeShared<InteractableDoorOpenState>();
	CurrentState = ClosedState;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	CurrentState = ClosedState;
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString ADoor::GetInteractionText()
{
	return CurrentState->GetMessage();
}

bool ADoor::ShowButtonWidget()
{
	return CurrentState->OfferInteract();
}

void ADoor::Interact()
{
	CurrentState->Execute(this);
}

void ADoor::Open()
{
	CurrentState = OpenState;
}

void ADoor::Close()
{
	CurrentState = ClosedState;
}

void InteractableDoorOpenState::Execute(AActor* Subject)
{
	if (auto GameMode = Cast<ALunariaGameModeBase>(Subject->GetWorld()->GetAuthGameMode()))
	{
		Cast<ADoor>(Subject)->OnDoorInteractWhenOpen();
		GameMode->StartNewAreaFromDoor(Cast<ADoor>(Subject));
	}
}

void InteractableDoorClosedState::Execute(AActor* Subject)
{
	Cast<ADoor>(Subject)->OnDoorInteractWhenClosed();
}