// Robbie Grier

#include "Destination.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LunariaGameModeBase.h"
#include "DestinationNameWidget.h"
#include "Printer.h"

ADestination::ADestination()
{
	NameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Destination Name Widget Component"));
	NameWidgetComponent->SetupAttachment(RootComponent);

	NameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	NameWidgetComponent->SetDrawAtDesiredSize(true);

	NameWidgetComponent->SetRelativeLocation(FVector(100.f, 0.f, 100.f));

	GetSphereComponent()->SetCollisionProfileName("Interactable");
	GetMeshComponent()->SetCollisionProfileName("OverlapAllDynamic");

	DestinationName = "Station";
}

void ADestination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NameWidgetComponent->SetWorldRotation(FRotator());
}

FString ADestination::GetInteractionText()
{
	return DestinationName;
}

void ADestination::Interact()
{
	Print("This is " + DestinationName);
}

void ADestination::SetDestinationName(const FString& InName)
{
	DestinationName = InName;
	Cast<UDestinationNameWidget>(NameWidgetComponent->GetUserWidgetObject())->SetName(DestinationName);
}

void ADestination::BeginPlay()
{
	Super::BeginPlay();

	NameWidgetComponent->SetWidgetClass(ALunariaGameModeBase::Get(GetWorld())->GetDestinationNameWidgetClass());
	NameWidgetComponent->SetVisibility(true);
	SetDestinationName(DestinationName);
}