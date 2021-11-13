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

void AStatusEffect::AddStack()
{
	if (BoonClass)
	{
		if (Statuses.Num() < MyOwnerAttributes->Get("MaxStatusEffectStacks", MaxStacksSeed))
		{
			auto AddedBoon = GetWorld()->SpawnActor<ABoon>(BoonClass);
			Statuses.Add(AddedBoon);
			SetDuration(AddedBoon->GetDurationAsStatusEffect());
			SetMaxStacks(AddedBoon->GetMaxStacksAsStatusEffect());
			AddedBoon->NativeOnAdded(MyOwnerAttributes);
		}

		LastStackTime = GetWorld()->GetTimeSeconds();
	}
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

	if (TimePassed > MyOwnerAttributes->Get("StatusEffectDuration", DurationSeed))
	{
		Remove();
	}
}

void AStatusEffect::NativeOnAdded(UAttributesComponent* Attributes)
{
	MyOwnerAttributes = Attributes;
}