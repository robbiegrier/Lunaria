// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DestinationNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class LUNARIA_API UDestinationNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDestinationNameWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetName(const FString& InName);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* Name;
};
