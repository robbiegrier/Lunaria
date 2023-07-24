// Robbie Grier

#include "Stat.h"
#include "Modification.h"

void UStat::Set(float InBase, float InScalar)
{
	Base = InBase;
	Scalar = InScalar;
}

void UStat::OpenModification(UModification* Mod)
{
	if (auto Stat = Cast<UModificationStat>(Mod))
	{
		Base += Stat->GetBase();
		Scalar += Stat->GetScalar();
	}
}

void UStat::CloseModification(UModification* Mod)
{
	if (auto Stat = Cast<UModificationStat>(Mod))
	{
		Base -= Stat->GetBase();
		Scalar -= Stat->GetScalar();
	}
}

void UColorAttribute::Set(const FLinearColor& InBase)
{
	Base = InBase;
	Color = InBase;
}

void UColorAttribute::OpenModification(UModification* Mod)
{
	if (auto ColorAttr = Cast<UModificationColor>(Mod))
	{
		Color = ColorAttr->GetColor();
	}
}

void UColorAttribute::CloseModification(UModification* Mod)
{
	if (auto ColorAttr = Cast<UModificationColor>(Mod))
	{
		Color = Base;
	}
}