// Robbie Grier

#include "CpuHealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Printer.h"

UCpuHealthBar::UCpuHealthBar(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UCpuHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCpuHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	InterpolateBackground(InDeltaTime);
}

void UCpuHealthBar::SetHealthValues(int32 Current, int32 Max)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(static_cast<float>(Current) / static_cast<float>(Max));
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Current, Max)));
	}
}

void UCpuHealthBar::SetScale(const FVector2D& InScale)
{
	HealthBarBorder->SetRenderScale(InScale);
}

void UCpuHealthBar::InterpolateBackground(float InDeltaTime)
{
	auto ReducedValue = HealthBarBackground->Percent - (InDeltaTime * BackgroundInterpolateSpeed);
	HealthBarBackground->SetPercent(FMath::Max(ReducedValue, HealthBar->Percent));
}