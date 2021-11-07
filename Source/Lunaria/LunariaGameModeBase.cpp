// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LunariaGameModeBase.h"
#include "User.h"
#include "Spaceship.h"
#include "MapManager.h"
#include "Components/StaticMeshComponent.h"
#include "Helpers.h"
#include "Printer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Door.h"

ALunariaGameModeBase::ALunariaGameModeBase()
{
	PlayerControllerClass = AUser::StaticClass();
	DefaultPawnClass = ASpaceship::StaticClass();
}

void ALunariaGameModeBase::StartPlay()
{
	Super::StartPlay();

	StartNewArea();
}

void ALunariaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MapManager = Cast<AMapManager>(GetWorld()->SpawnActor(MapManagerClass));
}

void ALunariaGameModeBase::StartNewArea()
{
	static auto TestScale = 1000.f;
	static auto TestEntDir = FVector(1.f, 1.f, 0.f);
	MapManager->LoadNewMap(TestScale, TestEntDir, 1);

	auto Nearest = Helpers::NearestPointInsideSphere(MapManager->GetEntrance()->GetActorLocation(), MapManager->GetCenter(), MapManager->GetRadius() - 100.f);
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(Nearest);
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorRotation(UKismetMathLibrary::MakeRotFromX(TestEntDir));

	TestScale *= 1.2f;

	TestEntDir = FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f);
}

void ALunariaGameModeBase::StartNewAreaFromDoor(ADoor* Door)
{
	auto ExitDirection = (Door->GetActorLocation() - MapManager->GetCenter()).GetSafeNormal();
	MapManager->LoadNewMap(FMath::RandRange(1000.f, 2000.f), ExitDirection, FMath::RandRange(1, 8));

	if (auto Spaceship = Cast<ASpaceship>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Spaceship->RespawnOnMap(MapManager);
	}
}