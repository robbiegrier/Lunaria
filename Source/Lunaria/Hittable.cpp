// Robbie Grier

#include "Hittable.h"
#include "SpaceProjectile.h"
#include "HealthComponent.h"
#include "CombatComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Printer.h"
#include "EventLibraryComponent.h"

void IHittable::TakeProjectileHit(ASpaceProjectile* Projectile)
{
	TakeHit(Projectile->GetDamage(), Projectile->GetOwner());
}

void IHittable::TakeHit(int32 DamageToApply, AActor* Other)
{
	auto OtherEventLibrary = Other->FindComponentByClass<UEventLibraryComponent>();
	auto Self = Cast<AActor>(this);
	if (!Self) return;
	if (!Other) return;

	auto DamageShouldBeApplied = true;

	auto SelfPawn = Cast<APawn>(Self);
	auto OtherPawn = Cast<APawn>(Other);

	if (OtherPawn && OtherPawn->GetController())
	{
		if (auto OtherCombatComponent = OtherPawn->GetController()->FindComponentByClass<UCombatComponent>())
		{
			if (SelfPawn && SelfPawn->GetController())
			{
				if (auto CombatComponent = SelfPawn->GetController()->FindComponentByClass<UCombatComponent>())
				{
					DamageShouldBeApplied = (CombatComponent->GetTeam() != OtherCombatComponent->GetTeam());
				}
			}
		}
	}

	if (DamageShouldBeApplied)
	{
		if (auto HealthComponent = Self->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->ApplyDamage(DamageToApply);
		}

		Execute_OnHit(Cast<UObject>(this));
	}
	else
	{
		Execute_OnHitIgnored(Cast<UObject>(this));
	}
}

void IHittable::TakeLethalDamage()
{
	NativeTakeLethalDamage();
	Execute_OnTakeLethalDamage(Cast<UObject>(this));
}