// https://gist.github.com/sinbad/9b8f8007fb1e55f1a952cce2d12aaac1

#include "InputModeDetector.h"
#include "Input/Events.h"

FInputModeDetector::FInputModeDetector()
{
	// 4 local players should be plenty usually (will expand if necessary)
	LastInputModeByPlayer.Init(EInputMode::Mouse, 4);
}

bool FInputModeDetector::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	// Key down also registers for gamepad buttons
	ProcessKeyOrButton(InKeyEvent.GetUserIndex(), InKeyEvent.GetKey());

	// Don't consume
	return false;
}

bool FInputModeDetector::HandleAnalogInputEvent(FSlateApplication& SlateApp,
	const FAnalogInputEvent& InAnalogInputEvent)
{
	if (InAnalogInputEvent.GetAnalogValue() > GamepadAxisThreshold)
		SetMode(InAnalogInputEvent.GetUserIndex(), EInputMode::Gamepad);
	// Don't consume
	return false;
}

bool FInputModeDetector::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	FVector2D Dist = MouseEvent.GetScreenSpacePosition() - MouseEvent.GetLastScreenSpacePosition();
	if (FMath::Abs(Dist.X) > MouseMoveThreshold || FMath::Abs(Dist.Y) > MouseMoveThreshold)
	{
		SetMode(MouseEvent.GetUserIndex(), EInputMode::Mouse);
	}
	// Don't consume
	return false;
}

bool FInputModeDetector::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	// We don't care which button
	SetMode(MouseEvent.GetUserIndex(), EInputMode::Mouse);
	// Don't consume
	return false;
}

bool FInputModeDetector::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent,
	const FPointerEvent* InGestureEvent)
{
	SetMode(InWheelEvent.GetUserIndex(), EInputMode::Mouse);
	// Don't consume
	return false;
}

void FInputModeDetector::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	// Required, but do nothing
}

EInputMode FInputModeDetector::GetLastInputMode(int PlayerIndex)
{
	if (PlayerIndex >= 0 && PlayerIndex < LastInputModeByPlayer.Num())
		return LastInputModeByPlayer[PlayerIndex];

	// Assume default if never told
	return DefaultInputMode;
}

void FInputModeDetector::ProcessKeyOrButton(int PlayerIndex, FKey Key)
{
	if (Key.IsGamepadKey())
	{
		SetMode(PlayerIndex, EInputMode::Gamepad);
	}
	else if (Key.IsMouseButton())
	{
		// Assuming mice don't have analog buttons!
		SetMode(PlayerIndex, EInputMode::Mouse);
	}
	else
	{
		// We assume anything that's not mouse and not gamepad is a keyboard
		// Assuming keyboards don't have analog buttons!
		SetMode(PlayerIndex, EInputMode::Keyboard);
	}
}

void FInputModeDetector::SetMode(int PlayerIndex, EInputMode NewMode)
{
	if (NewMode != EInputMode::Unknown && NewMode != GetLastInputMode(PlayerIndex))
	{
		if (PlayerIndex >= LastInputModeByPlayer.Num())
			LastInputModeByPlayer.SetNum(PlayerIndex + 1);

		LastInputModeByPlayer[PlayerIndex] = NewMode;
		OnInputModeChanged.ExecuteIfBound(PlayerIndex, NewMode);
		//UE_LOG(LogTemp, Warning, TEXT("Input mode for player %d changed: %s"), PlayerIndex, *UEnum::GetValueAsString(NewMode));
	}
}