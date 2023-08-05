// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributesComponent.h"
#include "Stat.h"
#include "CommonActor.h"
#include "Modification.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class LUNARIA_API UModification : public UObject
{
	GENERATED_BODY()

public:
	void SetAttribute(class UAttribute* InAttribute);

	void Open();
	void Close();

protected:
	class UAttribute* Attribute;
	bool IsOpen = false;
};

//DECLARE_DYNAMIC_DELEGATE_OneParam(FLunariaModificationCallback, class UAction*, Action);

UCLASS(Blueprintable, BlueprintType)
class LUNARIA_API UModificationLunariaStat : public UModification
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		float GetScalar(class UAction const* const Action);
	float GetScalar_Implementation(class UAction const* const Action) { return 0.f; }
	UFUNCTION(BlueprintNativeEvent)
		float GetAdditive(class UAction const* const Action);
	float GetAdditive_Implementation(class UAction const* const Action) { return 0.f; }
};

UCLASS(Blueprintable, BlueprintType)
class LUNARIA_API UModificationLunariaColor : public UModification
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
		FLinearColor GetColor(class UAction* Action);
	FLinearColor GetColor_Implementation(class UAction* Action) { return FLinearColor::White; }
};