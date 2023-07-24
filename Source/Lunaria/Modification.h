// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributesComponent.h"
#include "Modification.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class LUNARIA_API UModification : public UObject
{
	GENERATED_BODY()

public:
	void Open();
	void Close();

protected:
	class UAttribute* Attribute;
	bool IsOpen = false;
};

UCLASS(BlueprintType)
class LUNARIA_API UModificationStat : public UModification
{
	GENERATED_BODY()

public:
	void Set(class UStat* InAttribute, float InBase, float InScalar);

	float GetBase() const { return Base; }
	float GetScalar() const { return Scalar; }

protected:
	float Base = 0.f;
	float Scalar = 0.f;
	bool IsOpen = false;
};

UCLASS(BlueprintType)
class LUNARIA_API UModificationColor : public UModification
{
	GENERATED_BODY()

public:
	void Set(class UColorAttribute* InAttribute, const FLinearColor& InColor);

	FLinearColor GetColor() const { return Color; }

protected:
	FLinearColor Color;
};