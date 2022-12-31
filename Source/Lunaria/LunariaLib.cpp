// Robbie Grier

#include "LunariaLib.h"
#include "Helpers.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/World.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"

FGameplayTagContainer ULunariaLib::AddTagContainers(const FGameplayTagContainer& A, const FGameplayTagContainer& B)
{
	auto Output = FGameplayTagContainer();
	Output.AppendTags(A);
	Output.AppendTags(B);
	return Output;
}

bool ULunariaLib::AreDifferentTeams(AActor* Left, AActor* Right)
{
	return Helpers::AreDifferentTeams(Left, Right);
}

bool ULunariaLib::AreSameTeam(AActor* Left, AActor* Right)
{
	return !AreDifferentTeams(Left, Right);
}

FGameplayTagContainer ULunariaLib::GetTags(UObject* Asset)
{
	auto Output = FGameplayTagContainer();

	if (auto Tagged = Cast<IGameplayTagAssetInterface>(Asset))
	{
		Tagged->GetOwnedGameplayTags(Output);
	}

	return Output;
}

ASpaceship* ULunariaLib::Summon(AActor* Summoner, TSubclassOf<ASpaceship> SummonClass, const FTransform& Transform)
{
	if (Summoner)
	{
		auto Summon = Summoner->GetWorld()->SpawnActor<ASpaceship>(SummonClass, Transform);

		Summon->InitializeAlly();

		if (auto Combat = Summoner->FindComponentByClass<UCombatComponent>())
		{
			Combat->AddSummon(Summon);
		}

		return Summon;
	}

	return nullptr;
}

AActor* ULunariaLib::GetClosestActorToLocation(const FVector& Location, const TArray<AActor*>& Array, AActor* Ignore)
{
	return Helpers::GetClosestToLocation(Location, Array, Ignore);
}

TArray<AActor*> ULunariaLib::FilterActorsByTeam(const TArray<AActor*>& Array, int32 Team)
{
	auto Output = TArray<AActor*>();

	for (auto Actor : Array)
	{
		if (auto Combat = Actor->FindComponentByClass<UCombatComponent>())
		{
			if (Combat->GetTeam() == Team)
			{
				Output.Add(Actor);
			}
		}
	}

	return Output;
}

TArray<AActor*> ULunariaLib::FilterActorsByTeamExclusive(const TArray<AActor*>& Array, int32 Team)
{
	auto Output = TArray<AActor*>();

	for (auto Actor : Array)
	{
		if (auto Combat = Actor->FindComponentByClass<UCombatComponent>())
		{
			if (Combat->GetTeam() != Team)
			{
				Output.Add(Actor);
			}
		}
	}

	return Output;
}

int32 ULunariaLib::GetRivalTeam(int32 Team)
{
	if (Team == UCombatComponent::PlayerTeam)
	{
		return UCombatComponent::EnemyTeam;
	}

	return UCombatComponent::PlayerTeam;
}