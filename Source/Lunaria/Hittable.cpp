// Robbie Grier

#include "Hittable.h"
#include "SpaceProjectile.h"
#include "HealthComponent.h"
#include "CombatComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Printer.h"
#include "EventLibraryComponent.h"

void IHittable::TakeHit(int32 DamageToApply, AActor* Other)
{
	auto Self = Cast<AActor>(this);
	if (!Self) return;

	if (auto HealthComponent = Self->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyDamage(DamageToApply);
	}
}