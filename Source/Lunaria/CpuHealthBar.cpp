// Robbie Grier

#include "CpuHealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Printer.h"
#include "HealthComponent.h"
#include "AttributesComponent.h"

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
	UpdateHealth();
	InterpolateBackground(InDeltaTime);
}

void UCpuHealthBar::UpdateHealth()
{
	if (MyOwnerHealth)
	{
		auto MaxHealth = MyOwnerHealth->GetMaxHealth();
		auto CurrentHealth = MyOwnerHealth->GetCurrentHealthFast(MaxHealth);
		SetHealthValues(CurrentHealth, MaxHealth);

		if (PreviousHealthValue >= 0.f && !FMath::IsNearlyEqual(CurrentHealth, PreviousHealthValue, 0.0001f))
		{
			OnHealthChanged(CurrentHealth - PreviousHealthValue);
		}

		PreviousHealthValue = CurrentHealth;
	}
}

void UCpuHealthBar::SetMyOwner(AActor* InOwner)
{
	MyOwner = InOwner;

	if (InOwner)
	{
		MyOwnerHealth = InOwner->FindComponentByClass<UHealthComponent>();
	}
}

void UCpuHealthBar::HideHealthBar()
{
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
	HealthBarBackground->SetVisibility(ESlateVisibility::Hidden);
	HealthText->SetVisibility(ESlateVisibility::Hidden);
	HealthBarBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UCpuHealthBar::SetHealthValues(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Current / Max);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), static_cast<int32>(Current), static_cast<int32>(Max))));
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