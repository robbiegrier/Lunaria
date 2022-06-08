// Robbie Grier

#include "Pickup.h"
#include "LunariaGameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Printer.h"
#include "User.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Shell = CreateDefaultSubobject<USphereComponent>(TEXT("Shell Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	RootComponent = Shell;
	Mesh->SetupAttachment(RootComponent);

	Shell->SetCollisionProfileName(TEXT("Interactable"));

	Archetype = EArchetype::Beta;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString APickup::GetInteractionText()
{
	return "Pick up";
}

void APickup::Interact()
{
	if (auto User = Cast<AUser>(GetWorld()->GetFirstPlayerController()))
	{
		User->MakeSelectionFromPickup(this);
	}

	OnPickedUp();
}

void APickup::SignalInteractionComplete()
{
	OnInteractionComplete();

	if (Task)
	{
		Task->SignalTaskComplete();
	}

	Destroy();
}

void APickup::SetArchetype(EArchetype InArchetype)
{
	Archetype = InArchetype;
	auto Color = ALunariaGameModeBase::Get(GetWorld())->GetArchetypeColor(Archetype);
	Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Color.R, Color.G, Color.B));
}