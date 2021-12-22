// Robbie Grier
// References https://gist.github.com/sinbad/9b8f8007fb1e55f1a952cce2d12aaac1

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InputModeDetector.h"
#include "LunariaGameInstance.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputModeChanged, int, PlayerIndex, EInputMode, InputMode);
UCLASS()
class LUNARIA_API ULunariaGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	TSharedPtr<FInputModeDetector> InputDetector;
public:
	// Event raised when input mode changed between gamepad / keyboard / mouse
	UPROPERTY(BlueprintAssignable)
		FOnInputModeChanged OnInputModeChanged;

	UFUNCTION(BlueprintCallable)
		EInputMode GetLastInputModeUsed(int PlayerIndex = 0) const { return InputDetector->GetLastInputMode(PlayerIndex); }

	UFUNCTION(BlueprintCallable)
		bool LastInputWasGamePad(int PlayerIndex = 0) const { return GetLastInputModeUsed(PlayerIndex) == EInputMode::Gamepad; }

	virtual void Init() override;
	virtual void Shutdown() override;

private:
	void CreateInputDetector();
	void DestroyInputDetector();
	void OnInputDetectorModeChanged(int PlayerIndex, EInputMode NewMode);
};
