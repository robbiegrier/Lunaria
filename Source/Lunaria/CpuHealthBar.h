// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CpuHealthBar.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UCpuHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UCpuHealthBar(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetHealthValues(float Current, float Max);

	UFUNCTION(BlueprintCallable)
		void SetScale(const FVector2D& InScale);

	UFUNCTION(BlueprintCallable)
		void SetMyOwner(class AActor* InOwner);

private:
	void InterpolateBackground(float InDeltaTime);
	void UpdateHealth();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UProgressBar* HealthBarBackground;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UBorder* HealthBarBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
		float BackgroundInterpolateSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Owner Actor", meta = (AllowPrivateAccess = "true"))
		AActor* MyOwner;

	class UHealthComponent* MyOwnerHealth;
};
