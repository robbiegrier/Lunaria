// https://gist.github.com/sinbad/9b8f8007fb1e55f1a952cce2d12aaac1

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Framework/Application/IInputProcessor.h"

#include "UObject/ObjectMacros.h" // for UENUM

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Mouse,
	Keyboard,
	Gamepad,
	Unknown
};

DECLARE_DELEGATE_TwoParams(FOnInputModeForPlayerChanged, int /* PlayerIndex */, EInputMode)

/**
 * This class should be registered as an input processor in order to capture all input events & detect
 * what kind of devices are being used. We can't use PlayerController to do this reliably because in UMG
 * mode, all the mouse move events are consumed by Slate and you never see them, so it's not possible to
 * detect when the user moved a mouse.
 *
 * This class should be instantiated and used from some UObject of your choice, e.g. your GameInstance class,
 * something like this:
 *
 *    InputDetector = MakeShareable(new FInputModeDetector());
 *    FSlateApplication::Get().RegisterInputPreProcessor(InputDetector);
 *    InputDetector->OnInputModeChanged.BindUObject(this, &UMyGameInstance::OnInputDetectorModeChanged);
 *
 *    Note how the OnInputModeChanged on this object is a simple delegate, not a dynamic multicast etc, because
 *    this is not a UObject. You should relay the input mode event changed through the owner if you want to distribute
 *    the information further.
 */
class LUNARIA_API FInputModeDetector : public IInputProcessor, public TSharedFromThis<FInputModeDetector>
{
protected:
	TArray<EInputMode> LastInputModeByPlayer;

public:

	EInputMode DefaultInputMode = EInputMode::Mouse;
	float MouseMoveThreshold = 1;
	float GamepadAxisThreshold = 0.2;

	// Single delegate caller, owner should propagate if they want (this isn't a UObject)
	FOnInputModeForPlayerChanged OnInputModeChanged;

	FInputModeDetector();
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override;
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	EInputMode GetLastInputMode(int PlayerIndex = 0);

protected:
	void ProcessKeyOrButton(int PlayerIndex, FKey Key);
	void SetMode(int PlayerIndex, EInputMode NewMode);
};