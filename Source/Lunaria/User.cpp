// Robbie Grier

#include "User.h"
#include "Printer.h"
#include "Spaceship.h"
#include "SpaceshipWeaponComponent.h"
#include "CombatComponent.h"
#include "SpaceshipMovementComponent.h"
#include "Boon.h"
#include "AttributesComponent.h"
#include "Components/SphereComponent.h"
#include "Interactable.h"
#include "Door.h"
#include "ButtonTooltip.h"
#include "Helpers.h"
#include "LunariaGameModeBase.h"
#include "MapManager.h"
#include "Kismet/GameplayStatics.h"
#include "DetailTogglable.h"
#include "AbilitiesComponent.h"

AUser::AUser()
{
}

void AUser::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move Up Down", this, &AUser::HandleMovedUpDown);
	InputComponent->BindAxis("Move Right Left", this, &AUser::HandleMovedRightLeft);

	InputComponent->BindAction("Ability X", IE_Pressed, this, &AUser::HandleXPressed);
	InputComponent->BindAction("Ability X", IE_Released, this, &AUser::HandleXReleased);
	InputComponent->BindAction("Ability B", IE_Pressed, this, &AUser::HandleBPressed);
	InputComponent->BindAction("Ability B", IE_Released, this, &AUser::HandleBReleased);
	InputComponent->BindAction("Ability A", IE_Pressed, this, &AUser::HandleAPressed);
	InputComponent->BindAction("Ability A", IE_Released, this, &AUser::HandleAReleased);
	InputComponent->BindAction("Ability Y", IE_Pressed, this, &AUser::HandleYPressed);
	InputComponent->BindAction("Ability Y", IE_Released, this, &AUser::HandleYReleased);

	InputComponent->BindAction("Debug Action", IE_Pressed, this, &AUser::HandleDebugAction);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AUser::HandleInteractActionPressed);
	InputComponent->BindAction("Toggle Detail", IE_Pressed, this, &AUser::HandleToggleDetail);
}

void AUser::BeginPlay()
{
	Super::BeginPlay();

	ButtonTooltipWidget = NewObject<UButtonTooltip>(GetWorld(), ButtonTooltipClass);
	ButtonTooltipWidget->AddToViewport();
	ButtonTooltipWidget->SetVisibility(ESlateVisibility::Hidden);
	ButtonTooltipWidget->SetText("Enter...");
}

void AUser::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Spaceship = Cast<ASpaceship>(InPawn);
	checkf(Spaceship, TEXT("%s pawn is not a ASpaceship"), *GetFullName());
}

void AUser::OnUnPossess()
{
	Super::OnUnPossess();
}

void AUser::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	TickInteractions();

	auto MovementDirection = FVector(UpDownImpulse, RightLeftImpulse, 0.f).GetSafeNormal();
	auto MovementScale = FMath::Clamp(FMath::Abs(UpDownImpulse) + FMath::Abs(RightLeftImpulse), 0.f, 1.f);

	Spaceship->HandleThrottleInput(MovementScale);
	Spaceship->TurnToFaceDirection(MovementDirection);
}

void AUser::TickInteractions()
{
	InteractableActors.Empty();
	Spaceship->GetInteractionVolume()->GetOverlappingActors(InteractableActors);

	for (auto i = 0; i < InteractableActors.Num(); i++)
	{
		auto Actor = InteractableActors[i];
		if (!Cast<IInteractable>(Actor))
		{
			InteractableActors.RemoveAt(i);
		}
	}

	if (InteractableActors.Num() > 0)
	{
		auto Closest = Helpers::GetClosestToLocation(GetPawn()->GetActorLocation(), InteractableActors);
		auto ClosestInteractable = Cast<IInteractable>(Closest);
		ButtonTooltipWidget->LoadInteraction(ClosestInteractable);
		ButtonTooltipWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ButtonTooltipWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AUser::HandleMovedUpDown(float Scale)
{
	UpDownImpulse = Scale;
}

void AUser::HandleMovedRightLeft(float Scale)
{
	RightLeftImpulse = Scale;
}

void AUser::HandleToggleDetail()
{
	GlobalDetailFlag = !GlobalDetailFlag;

	auto DetailTogglables = TArray<AActor*>();
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDetailTogglable::StaticClass(), DetailTogglables);

	for (auto Actor : DetailTogglables)
	{
		auto Togglable = Cast<IDetailTogglable>(Actor);
		GlobalDetailFlag ? Togglable->ToggleDetailOn() : Togglable->ToggleDetailOff();
	}
}

void AUser::HandleXPressed()
{
	Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::X);
}

void AUser::HandleXReleased()
{
	Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::X);
}

void AUser::HandleBPressed()
{
	Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::B);
}

void AUser::HandleBReleased()
{
	Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::B);
}

void AUser::HandleAPressed()
{
	Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::A);
}

void AUser::HandleAReleased()
{
	Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::A);
}

void AUser::HandleYPressed()
{
	Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::Y);
}

void AUser::HandleYReleased()
{
	Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::Y);
}

void AUser::HandleDebugAction()
{
	Print("Debug Action");

	//Spaceship->GetAttributesComponent()->SetMoveSpeed(Spaceship->GetAttributesComponent()->GetMoveSpeed() + 100.0f);
	//Spaceship->GetAttributesComponent()->SetAttackDamage(Spaceship->GetAttributesComponent()->GetAttackDamage() + 5.0f);
	//Spaceship->GetAttributesComponent()->SetMaxHealth(Spaceship->GetAttributesComponent()->GetMaxHealth() + 5.0f);
	//Spaceship->GetAttributesComponent()->SetTurnSpeed(Spaceship->GetAttributesComponent()->GetTurnSpeed() + 25.0f);

	//Spaceship->GetAttributesComponent()->AddBoon(NewObject<UBoon>((UObject*)GetTransientPackage(), BoonClass));
	Spaceship->GetAttributesComponent()->AddBoon(GetWorld()->SpawnActor<ABoon>(BoonClass));
	//if (auto GameMode = Cast<ALunariaGameModeBase>(GetWorld()->GetAuthGameMode()))
	//{
	//	GameMode->StartNewArea();
	//}

	if (auto GameMode = Cast<ALunariaGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->GetMap()->OpenCurrentDoors();
	}
}

void AUser::HandleInteractActionPressed()
{
	if (InteractableActors.Num() > 0)
	{
		auto Closest = Helpers::GetClosestToLocation(GetPawn()->GetActorLocation(), InteractableActors);
		auto ClosestInteractable = Cast<IInteractable>(Closest);

		ClosestInteractable->Interact();
	}
}