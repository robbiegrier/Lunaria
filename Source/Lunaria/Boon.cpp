// Robbie Grier

#include "Boon.h"
#include "Printer.h"

UBoon::UBoon()
{
	BoonName = "Spaceship Upgrade";
}

void UBoon::NativeOnAdded(class AActor* AppliedTo)
{
	Print("NativeOnAdded");
	OnAdded();
}