// Robbie Grier

#include "ActionCreateProjectile.h"
#include "Printer.h"
#include "SpaceProjectile.h"
#include "FGameplayEvent.h"
#include "GameplayEventManager.h"
#include "AttributesComponent.h"
#include "Engine/World.h"
#include "CombatComponent.h"

void UActionCreateProjectile::PerformCreateProjectile(AActor* InAgent, ATool* InTool, TSubclassOf<class ASpaceProjectile> InProjectileClass, const FTransform& InTransform, float InBaseDamage, float InDistance, float InSpeed)
{
	auto Action = NewObject<UActionCreateProjectile>();
	Action->Agent = InAgent;
	Action->ProjectileClass = InProjectileClass;
	Action->Subject = nullptr;
	Action->Tool = InTool;
	Action->Transform = InTransform;
	Action->BaseDamage = InBaseDamage;
	Action->Distance = InDistance;
	Action->Speed = InSpeed;

	if (auto CombatComponent = InAgent->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->AddAction(Action);
	}
}

void UActionCreateProjectile::Execute()
{
	Print("projectile action from " + Tool->GetName() + " -> " + ProjectileClass->GetName(), FColor::Orange);

	if (auto Attributes = Agent->FindComponentByClass<UAttributesComponent>())
	{
		Attributes->ProjectileSpeed->Set(Speed);
		Speed = Attributes->ProjectileSpeed->Render(this);
	}

	auto MediumSpawn = Cast<ASpaceProjectile>(AMedium::Spawn(ProjectileClass, Agent, Tool, Transform, BaseDamage));
	MediumSpawn->SetProjectileProperties(Distance, Speed);
	MediumSpawn->LaunchCombatMedium();
}