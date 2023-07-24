// Robbie Grier

#include "Modification.h"
#include "Stat.h"

void UModification::Open()
{
	Attribute->OpenModification(this);
	IsOpen = true;
}

void UModification::Close()
{
	Attribute->CloseModification(this);
	IsOpen = false;
}

void UModificationStat::Set(UStat* InAttribute, float InBase, float InScalar)
{
	Attribute = InAttribute;
	Base = InBase;
	Scalar = InScalar;
}

void UModificationColor::Set(class UColorAttribute* InAttribute, const FLinearColor& InColor)
{
	Attribute = InAttribute;
	Color = InColor;
}