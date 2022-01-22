// Robbie Grier

#include "LunariaLib.h"

FGameplayTagContainer ULunariaLib::AddTagContainers(const FGameplayTagContainer& A, const FGameplayTagContainer& B)
{
	auto Output = FGameplayTagContainer();
	Output.AppendTags(A);
	Output.AppendTags(B);
	return Output;
}