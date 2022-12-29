// Robbie Grier


#include "DestinationNameWidget.h"
#include "Components/TextBlock.h"

UDestinationNameWidget::UDestinationNameWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UDestinationNameWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDestinationNameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDestinationNameWidget::SetName(const FString& InName)
{
	Name->SetText(FText::FromString(InName));
}
