// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpriteWidget.generated.h"

/**
 *
 */
UCLASS()
class LUNARIA_API USpriteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USpriteWidget(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetSprite(const FSlateBrush& SpriteBrush);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
		class UImage* Sprite;
};
