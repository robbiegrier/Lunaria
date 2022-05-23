// Robbie Grier

#include "LunariaLib.h"
#include "Helpers.h"
#include "GameplayTagAssetInterface.h"

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