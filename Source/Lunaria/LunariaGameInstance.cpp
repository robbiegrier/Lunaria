// Robbie Grier
// References https://gist.github.com/sinbad/9b8f8007fb1e55f1a952cce2d12aaac1

#include "LunariaGameInstance.h"
#include "Framework/Application/SlateApplication.h"

void ULunariaGameInstance::Init()
{
	Super::Init();
	CreateInputDetector();
}

void ULunariaGameInstance::Shutdown()
{
	Super::Shutdown();
	DestroyInputDetector();
}

void ULunariaGameInstance::CreateInputDetector()
{
	if (!InputDetector.IsValid())
	{
		InputDetector = MakeShareable(new FInputModeDetector());
		FSlateApplication::Get().RegisterInputPreProcessor(InputDetector);

		InputDetector->OnInputModeChanged.BindUObject(this, &ULunariaGameInstance::OnInputDetectorModeChanged);
	}
}

void ULunariaGameInstance::DestroyInputDetector()
{
	if (InputDetector.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputDetector);
		InputDetector.Reset();
	}
}

void ULunariaGameInstance::OnInputDetectorModeChanged(int PlayerIndex, EInputMode NewMode)
{
	OnInputModeChanged.Broadcast(PlayerIndex, NewMode);
}