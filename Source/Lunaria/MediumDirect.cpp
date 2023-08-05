// Robbie Grier

#include "MediumDirect.h"
#include "Tool.h"

float AMediumDirect::NativeGetBaseDamage() const
{
	if (Tool)
	{
		return Tool->GetBaseDamage();
	}

	return BaseDamage;
}