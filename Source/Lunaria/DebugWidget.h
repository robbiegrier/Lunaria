// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugWidget.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UDebugWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetText(const FString& InText);
	void SetSubText(const FString& InText);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* DebugText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* DebugSubText;
};
