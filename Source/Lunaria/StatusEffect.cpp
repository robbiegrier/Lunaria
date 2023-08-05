// Robbie Grier

#include "StatusEffect.h"
#include "Engine/World.h"
#include "Boon.h"
#include "Printer.h"
#include "AttributesComponent.h"
#include "Components/SceneComponent.h"

AStatusEffect::AStatusEffect()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	RootComponent = SceneComponent;
}

ABoon* AStatusEffect::AddStack(AActor* Creator)
{
	auto AddedBoon = static_cast<ABoon*>(nullptr);

	if (BoonClass)
	{
		if (Statuses.Num() < 10)
		{
			AddedBoon = GetWorld()->SpawnActor<ABoon>(BoonClass);
			Statuses.Add(AddedBoon);
			SetMaxStacks(AddedBoon->GetMaxStacksAsStatusEffect());
			AddedBoon->NativeOnAdded(MyOwnerAttributes, Creator);
		}

		LastStackTime = GetWorld()->GetTimeSeconds();
	}

	return AddedBoon;
}

void AStatusEffect::Remove()
{
	if (MyOwnerAttributes)
	{
		MyOwnerAttributes->RemoveAndDestroyStatusEffect(this);
	}
}

void AStatusEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AStatusEffect::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	ClearStatuses();
}

void AStatusEffect::ClearStatuses()
{
	for (auto Status : Statuses)
	{
		if (Status)
		{
			Status->NativeOnRemoved();
			Status->Destroy();
		}
	}

	Statuses.Empty();
}

void AStatusEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Status : Statuses)
	{
		Status->SetActorTransform(GetActorTransform());
	}

	auto TimePassed = GetWorld()->GetTimeSeconds() - LastStackTime;

	if (Statuses.Num() > 0)
	{
		if (TimePassed > Duration)
		{
			Remove();
		}
	}
}

void AStatusEffect::NativeOnAdded(UAttributesComponent* Attributes)
{
	MyOwnerAttributes = Attributes;
	SetActorTransform(MyOwnerAttributes->GetOwner()->GetActorTransform());
}