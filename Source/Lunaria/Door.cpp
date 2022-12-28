// Robbie Grier

#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Spaceship.h"
#include "GameFramework/PlayerController.h"
#include "Printer.h"
#include "User.h"
#include "LunariaGameModeBase.h"
#include "LunariaLib.h"

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
	IsOpen ? Open() : Close();
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

bool ADoor::ContainsMapDescription() const
{
	return MapDescriptions.Num() > 0 || MapDescriptionId != "";
}

FMapDescription ADoor::GetMapDescription() const
{
	auto Output = FMapDescription();

	if (MapDescriptions.Num() > 0)
	{
		Output = MapDescriptions[FMath::RandRange(0, MapDescriptions.Num() - 1)];
	}
	else if (MapDescriptionId != "")
	{
		Output = ALunariaGameModeBase::Get(GetWorld())->GetGlobalMapDescriptions()[MapDescriptionId];
	}

	Output.Entry = (GetActorLocation() - ALunariaGameModeBase::Get(GetWorld())->GetMap()->GetCenter()).GetSafeNormal() * -1.f;
	return Output;
}

void ADoor::Open()
{
	CurrentState = OpenState;
	OnOpen();
}

void ADoor::Close()
{
	CurrentState = ClosedState;
	OnClose();
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