// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupChoiceWidget.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UPickupChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPickupChoiceWidget(const FObjectInitializer& ObjectInitializer);
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* ChoiceName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* ChoiceDescription;
};
