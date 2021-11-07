// Robbie Grier

#include "DetailTogglable.h"
#include "User.h"
#include "Engine/World.h"

// Add default functionality here for any IDetailTogglable functions that are not pure virtual.
void IDetailTogglable::SetInitialDetailToggleState()
{
	if (auto Actor = Cast<AActor>(this))
	{
		if (auto User = Cast<AUser>(Actor->GetWorld()->GetFirstPlayerController()))
		{
			User->IsDetailOn() ? ToggleDetailOn() : ToggleDetailOff();
		}
	}
}