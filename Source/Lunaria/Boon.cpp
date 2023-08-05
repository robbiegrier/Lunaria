// Robbie Grier

#include "Boon.h"
#include "Printer.h"
#include "AttributesComponent.h"
#include "GameplayEventManager.h"
#include "Components/SceneComponent.h"
#include "SpaceProjectile.h"
#include "Engine/World.h"
#include "LunariaLib.h"
#include "LunariaGameModeBase.h"
#include "Modification.h"

ABoon::ABoon()
{
	BoonName = "Spaceship Upgrade";
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	RootComponent = SceneComponent;
}

void ABoon::NativeOnAdded(UAttributesComponent* Attributes, AActor* InCreator)
{
	MyOwner = Attributes->GetOwner();
	MyOwnerAttributes = Attributes;
	Creator = InCreator ? InCreator : MyOwner;
	SetAgent(Creator);

	OnAdded();
}

void ABoon::NativeOnRemoved()
{
	Close();
	OnRemoved();
}

void ABoon::NativeLevelUp()
{
	BoonLevel++;
	OnLeveledUp();
}

FString ABoon::GetChoiceName()
{
	return BoonName;
}

FString ABoon::GetChoiceDescription()
{
	return BoonDescription;
}

void ABoon::Choose(AActor* Chooser)
{
	if (auto Attributes = Chooser->FindComponentByClass<UAttributesComponent>())
	{
		Attributes->AddBoon(this);
	}
}

void ABoon::Remove()
{
	if (MyOwnerAttributes)
	{
		MyOwnerAttributes->RemoveAndDestroyBoon(this);
	}
}

ASpaceProjectile* ABoon::CreateBoonProjectileWithTransform(TSubclassOf<ASpaceProjectile> SpawnClass, const FTransform& Transform, float InDamage, float Distance, const FGameplayTagContainer& InTags, const TArray<AActor*>& ActorsToIgnore, int32 Bounces)
{
	//auto Params = FActorSpawnParameters();
	//Params.Owner = MyOwner;
	//auto Projectile = GetWorld()->SpawnActor<ASpaceProjectile>(SpawnClass, Transform, Params);

	//if (Projectile)
	//{
	//	auto Color = ALunariaGameModeBase::Get(GetWorld())->GetArchetypeColor(GetArchetype());
	//	Projectile->SetIgnoreActors(ActorsToIgnore);
	//	Projectile->SetBounces(Bounces);
	//	Projectile->SetPayloadProperties(InTags, InDamage, Distance, Color);
	//}

	//return Projectile;

	Print("boon proj create is deprecated: " + MyOwner->GetName());

	return nullptr;
}

void ABoon::ModifyAttribute(FGameplayTag AttributeName, UModification* Mod)
{
	auto FindAttribute = MyOwnerAttributes->StatMap.Find(AttributeName);

	if (FindAttribute)
	{
		auto Attribute = *FindAttribute;
		Mod->SetAttribute(Attribute);
		Modifications.Add(Mod);
		Mod->Open();
	}
	else
	{
		Print("Cannot make Modification. Could not find Attribute: " + AttributeName.ToString());
	}
}

void ABoon::Close()
{
	for (auto Mod : Modifications)
	{
		Mod->Close();
	}
}