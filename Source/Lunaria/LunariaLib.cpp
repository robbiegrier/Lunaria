// Robbie Grier

#include "LunariaLib.h"
#include "Helpers.h"

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