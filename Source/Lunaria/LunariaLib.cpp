// Robbie Grier

#include "LunariaLib.h"
#include "Helpers.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/World.h"
#include "CombatComponent.h"

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