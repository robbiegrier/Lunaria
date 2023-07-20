// Robbie Grier

#include "Pickup.h"
#include "LunariaGameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Printer.h"
#include "User.h"
#include "SpriteWidget.h"
#include "Components/WidgetComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Shell = CreateDefaultSubobject<USphereComponent>(TEXT("Shell Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	RootComponent = Shell;
	Mesh->SetupAttachment(RootComponent);

	Shell->SetCollisionProfileName(TEXT("Interactable"));

	Archetype = EArchetype::Beta;

	SpriteWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Sprite Widget Component"));
	SpriteWidgetComponent->SetupAttachment(RootComponent);
	SpriteWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	SpriteWidgetComponent->SetDrawAtDesiredSize(true);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	SpriteWidgetComponent->SetWidgetClass(ALunariaGameModeBase::Get(GetWorld())->GetSpriteWidgetClass());
	SpriteWidgetComponent->SetVisibility(false);
}

void APickup::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (WorldSprite)
	{
		WorldSprite->Destroy();
	}
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WorldSprite)
	{
		WorldSprite->SetActorTransform(GetActorTransform());
	}
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

	//if (auto SpriteWidget = Cast<USpriteWidget>(SpriteWidgetComponent->GetUserWidgetObject()))
	//{
	//	SpriteWidget->SetSprite(ALunariaGameModeBase::Get(GetWorld())->GetArchetypeIcon(Archetype));
	//}

	WorldSprite = GetWorld()->SpawnActor(ALunariaGameModeBase::Get(GetWorld())->GetArchetypeWorldSpriteClass(Archetype), &GetTransform());
}