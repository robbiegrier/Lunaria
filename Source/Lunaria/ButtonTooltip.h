// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonTooltip.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UButtonTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	UButtonTooltip(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetText(const FString& InText);
	void LoadInteraction(class IInteractable* Interactable);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* ButtonTooltipText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UImage* ButtonTooltipImage;
};
