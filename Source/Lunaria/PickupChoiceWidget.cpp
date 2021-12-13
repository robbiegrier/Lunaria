// Robbie Grier

#include "PickupChoiceWidget.h"
#include "Components/TextBlock.h"

UPickupChoiceWidget::UPickupChoiceWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UPickupChoiceWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
}