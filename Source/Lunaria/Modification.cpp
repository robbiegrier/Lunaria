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

void UModification::SetAttribute(class UAttribute* InAttribute)
{
	Attribute = InAttribute;
}