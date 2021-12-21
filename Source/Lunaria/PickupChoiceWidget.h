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

	void SetName(const FString& InName);
	void SetDescription(const FString& InDesc);
	void SetColor(const FLinearColor& Color);

	void SetChoiceActor(AActor* InActor) { ChoiceActor = InActor; }
	AActor* GetChoiceActor() const { return ChoiceActor; }
	void SetSelectionWidget(class UPickupSelectionWidget* Widget) { SelectionWidget = Widget; }

	class UButton* GetChoiceButton() const { return ChoiceButton; }

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void HandleButtonPressed();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* ChoiceName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* ChoiceDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UBorder* ColoredBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UButton* ChoiceButton;

	AActor* ChoiceActor;
	class UPickupSelectionWidget* SelectionWidget;
};
