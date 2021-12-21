// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupSelectionWidget.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UPickupSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPickupSelectionWidget(const FObjectInitializer& ObjectInitializer);
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void MakeSelectionFromPickup(class APickup* Pickup);

	void SetTitle(const FString& InTitle);
	void SetColor(const FLinearColor& Color);

	void SignalChoiceMade(class UPickupChoiceWidget* ChoiceWidget);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* TitleText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UBorder* ColoredBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UVerticalBox* ChoicesVerticalBox;

	TArray<AActor*> TransientChoiceActors;

	bool SelectionInProgress = false;
	class APickup* CurrentPickup;
};
