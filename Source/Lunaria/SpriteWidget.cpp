// Robbie Grier

#include "SpriteWidget.h"
#include "Components/Image.h"

USpriteWidget::USpriteWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void USpriteWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USpriteWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USpriteWidget::SetSprite(const FSlateBrush& SpriteBrush)
{
	auto Size = Sprite->Brush.ImageSize;
	Sprite->SetBrush(SpriteBrush);
	Sprite->Brush.ImageSize = Size;
}