// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GreatMessageWidget.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UGreatMessageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UGreatMessageWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetGreatMessage(const FString& InText);

	void StartDisplay(const FString& InText);
	void EndDisplay();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* GreatMessage;
};
