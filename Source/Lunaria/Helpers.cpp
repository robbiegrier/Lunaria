// Robbie Grier

#include "Helpers.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "LunariaGameModeBase.h"

USceneComponent* Helpers::GetMeshForFlairMovement(AActor* Actor)
{
	if (!Actor) return nullptr;

	auto Mesh = static_cast<USceneComponent*>(Actor->FindComponentByClass<USkeletalMeshComponent>());

	if (!Mesh)
	{
		Mesh = static_cast<USceneComponent*>(Actor->FindComponentByClass<UStaticMeshComponent>());
	}

	return Mesh;
}

float Helpers::Dilate(float Val, UWorld* WorldRelativeTo)
{
	if (WorldRelativeTo)
	{
		return Val * WorldRelativeTo->GetDeltaSeconds();
	}

	return Val;
}

bool Helpers::IsPointInsideSphere(const FVector& InPoint, const FVector& Center, float Radius)
{
	auto Distance = (InPoint - Center).Size();
	return Distance <= Radius;
}

FVector Helpers::NearestPointInsideSphere(const FVector& InPoint, const FVector& Center, float Radius)
{
	if (IsPointInsideSphere(InPoint, Center, Radius))
	{
		return InPoint;
	}

	auto Direction = (InPoint - Center).GetSafeNormal();
	return Center + (Direction * Radius);
}

bool Helpers::AreDifferentTeams(AActor* Left, AActor* Right)
{
	if (Left && Right)
	{
		if (auto LeftComp = Left->FindComponentByClass<UCombatComponent>())
		{
			if (auto RightComp = Right->FindComponentByClass<UCombatComponent>())
			{
				return LeftComp->GetTeam() != RightComp->GetTeam();
			}
		}
	}

	return false;
}

bool Helpers::AreSameTeam(AActor* Left, AActor* Right)
{
	return !AreDifferentTeams(Left, Right);
}

void Helpers::BindHealthBar(AActor* Actor, UWidgetComponent* HealthBarWidgetComponent)
{
	if (Actor)
	{
		if (HealthBarWidgetComponent->GetWidgetClass() == nullptr)
		{
			if (auto Gamemode = Cast<ALunariaGameModeBase>(Actor->GetWorld()->GetAuthGameMode()))
			{
				HealthBarWidgetComponent->SetWidgetClass(Gamemode->GetHealthBarClass());
			}
		}

		if (auto HealthBar = Cast<UCpuHealthBar>(HealthBarWidgetComponent->GetUserWidgetObject()))
		{
			HealthBar->SetMyOwner(Actor);
		}
	}
}

void Helpers::SafelyKillMyself(AActor* Actor)
{
	Actor->SetLifeSpan(Actor->GetWorld()->GetDeltaSeconds() * 2.f);
}