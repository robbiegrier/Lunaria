// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Boon.h"
#include "UserHud.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API UUserHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UUserHud(const FObjectInitializer& ObjectInitializer);
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetHealthValues(float Current, float Max);

	void SetBoonList(const TArray<ABoon*>& ActiveBoons);

	void SetInventoryDisplay(class UInventoryComponent* Inventory);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* BoonListText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UTextBlock* InventoryText;
};
