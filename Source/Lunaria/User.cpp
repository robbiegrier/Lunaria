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
#include "AreaOfEffect.h"
#include "LunariaGameInstance.h"
#include "GreatMessageWidget.h"
#include "ScreenFadeWidget.h"
#include "TimerManager.h"

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
	InputComponent->BindAction("Ability LT", IE_Pressed, this, &AUser::HandleLTPressed);
	InputComponent->BindAction("Ability LT", IE_Released, this, &AUser::HandleLTReleased);

	InputComponent->BindAction("Debug Action", IE_Pressed, this, &AUser::HandleDebugAction);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AUser::HandleInteractActionPressed);
	InputComponent->BindAction("Toggle Detail", IE_Pressed, this, &AUser::HandleToggleDetail);

	InputComponent->BindAction("Debug01", IE_Pressed, this, &AUser::HandleDebugAction1);
	InputComponent->BindAction("Debug02", IE_Pressed, this, &AUser::HandleDebugAction2);
}

void AUser::BeginPlay()
{
	Super::BeginPlay();

	ButtonTooltipWidget = NewObject<UButtonTooltip>(GetWorld(), ButtonTooltipClass);
	ButtonTooltipWidget->AddToViewport();
	ButtonTooltipWidget->SetVisibility(ESlateVisibility::Hidden);
	ButtonTooltipWidget->SetText("Enter...");
	ButtonTooltipWidget->SetOwningPlayer(this);

	PickupSelectionWidget = NewObject<UPickupSelectionWidget>(GetWorld(), ALunariaGameModeBase::Get(GetWorld())->GetPickupSelectionWidgetClass());
	PickupSelectionWidget->AddToViewport(100);
	PickupSelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	PickupSelectionWidget->SetOwningPlayer(this);

	UserHudWidget = NewObject<UUserHud>(GetWorld(), UserHudClass);
	UserHudWidget->AddToViewport();
	UserHudWidget->SetVisibility(ESlateVisibility::Visible);
	UserHudWidget->SetOwningPlayer(this);

	GreatMessageWidget = NewObject<UGreatMessageWidget>(GetWorld(), ALunariaGameModeBase::Get(GetWorld())->GetGreatMessageWidgetClass());
	GreatMessageWidget->AddToViewport(10000);
	GreatMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	GreatMessageWidget->SetOwningPlayer(this);

	ScreenFadeWidget = NewObject<UScreenFadeWidget>(GetWorld(), ALunariaGameModeBase::Get(GetWorld())->GetScreenFadeWidgetClass());
	ScreenFadeWidget->AddToViewport(20000);
	ScreenFadeWidget->SetVisibility(ESlateVisibility::Hidden);
	ScreenFadeWidget->SetOwningPlayer(this);
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

void AUser::MakeSelectionFromPickup(APickup* Pickup)
{
	PickupSelectionWidget->MakeSelectionFromPickup(Pickup);
}

void AUser::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	TickInteractions();

	auto MovementDirection = FVector(UpDownImpulse, RightLeftImpulse, 0.f).GetSafeNormal();
	auto MovementScale = FMath::Clamp(FMath::Abs(UpDownImpulse) + FMath::Abs(RightLeftImpulse), 0.f, 1.f);

	if (InGame) Spaceship->HandleThrottleInput(MovementScale);
	if (InGame) Spaceship->TurnToFaceDirection(MovementDirection);

	auto Instance = Cast<ULunariaGameInstance>(GetGameInstance());
	bShowMouseCursor = !(InGame || Instance->LastInputWasGamePad());
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
	if (InGame) Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::X);
}

void AUser::HandleXReleased()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::X);
}

void AUser::HandleBPressed()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::B);
}

void AUser::HandleBReleased()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::B);
}

void AUser::HandleAPressed()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::A);
}

void AUser::HandleAReleased()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::A);
}

void AUser::HandleYPressed()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::Y);
}

void AUser::HandleYReleased()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::Y);
}

void AUser::HandleLTPressed()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->PressAbility(EAbilityKey::LT);
}

void AUser::HandleLTReleased()
{
	if (InGame) Spaceship->GetAbilitiesComponent()->ReleaseAbility(EAbilityKey::LT);
}

void AUser::ToggleUIControl(bool IsUIOn)
{
	InGame = !IsUIOn;

	if (IsUIOn)
	{
		SetInputMode(FInputModeGameAndUI());

		auto Instance = Cast<ULunariaGameInstance>(GetGameInstance());

		if (Instance && Instance->LastInputWasGamePad())
		{
			SetMouseLocation(0, 0);
		}
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}

void AUser::NativeOnDeath()
{
	GreatMessageWidget->StartDisplay("YOU DIED");
	OnDeath();

	auto Handle = FTimerHandle();
	GetWorldTimerManager().SetTimer(Handle, [this]() {
		ScreenFadeWidget->Fade();

		auto InnerHandle = FTimerHandle();
		GetWorldTimerManager().SetTimer(InnerHandle, [this]() {
			ScreenFadeWidget->Unfade();
			ALunariaGameModeBase::Get(GetWorld())->StartSpecificArea("Home");
			Spaceship->EndPlayerDeath();
			}, 1.f, false);
		}, 5.f, false);
}

void AUser::NativeOnRespawn()
{
	GreatMessageWidget->EndDisplay();
	OnRespawn();
}

void AUser::NativeOnUsedDoor(ADoor* Door)
{
	DisableInput(this);
	Spaceship->SetMovementScale(0.f);
	ScreenFadeWidget->Fade();

	auto Handle = FTimerHandle();
	GetWorldTimerManager().SetTimer(Handle, [this, Door]() {
		EnableInput(this);
		Spaceship->SetMovementScale(1.f);
		ALunariaGameModeBase::Get(GetWorld())->StartNewAreaFromDoor(Door);
		ScreenFadeWidget->Unfade();
		}, 1.f, false);

	OnUsedDoor(Door);
}

void AUser::HandleDebugAction()
{
	Print("Debug Action");

	if (!BoonSpawn || BoonSpawn->IsActorBeingDestroyed())
	{
		BoonSpawn = GetWorld()->SpawnActor<ABoon>(BoonClass);
		Spaceship->GetAttributesComponent()->AddBoon(BoonSpawn);
	}
	else
	{
		Spaceship->GetAttributesComponent()->RemoveAndDestroyBoon(BoonSpawn);
		BoonSpawn = nullptr;
	}
}

void AUser::HandleDebugAction1()
{
	Print("Debug Action 1");

	if (!BoonSpawn1 || BoonSpawn1->IsActorBeingDestroyed())
	{
		BoonSpawn1 = GetWorld()->SpawnActor<ABoon>(BoonClass1);
		Spaceship->GetAttributesComponent()->AddBoon(BoonSpawn1);
	}
	else
	{
		Spaceship->GetAttributesComponent()->RemoveAndDestroyBoon(BoonSpawn1);
	}
}

void AUser::HandleDebugAction2()
{
	Print("Debug Action 2");

	if (!BoonSpawn2 || BoonSpawn2->IsActorBeingDestroyed())
	{
		BoonSpawn2 = GetWorld()->SpawnActor<ABoon>(BoonClass2);
		Spaceship->GetAttributesComponent()->AddBoon(BoonSpawn2);
	}
	else
	{
		Spaceship->GetAttributesComponent()->RemoveAndDestroyBoon(BoonSpawn2);
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