// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenFadeWidget.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UScreenFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UScreenFadeWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Fade();
	void Unfade();

private:
	void ScheduleHide();

	FTimerHandle Handle;
	float OpacityDirection = 1.f;
};
