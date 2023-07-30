// Robbie Grier

#include "Stat.h"
#include "Action.h"
#include "Modification.h"
#include "Printer.h"

float ULunariaStat::Render(class UAction* Action)
{
	auto TotalBase = Base;
	auto TotalScalar = Scalar;

	for (auto Mod : Mods)
	{
		if (auto StatMod = Cast<UModificationLunariaStat>(Mod))
		{
			TotalScalar += StatMod->GetScalar(Action);
		}
		else
		{
			Print("Wrong type of modification found for stat: " + AttributeName.ToString());
		}
	}

	return TotalBase * TotalScalar;
}

FLinearColor ULunariaColor::Render(UAction* Action)
{
	if (Mods.Num() > 0)
	{
		if (auto ColorMod = Cast<UModificationLunariaColor>(Mods[0]))
		{
			return ColorMod->GetColor(Action);
		}
		else
		{
			Print("Wrong type of modification found for color: " + AttributeName.ToString());
		}
	}

	return FLinearColor::White;
}