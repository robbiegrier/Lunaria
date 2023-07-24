// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Stat.generated.h"

UCLASS(BlueprintType)
class LUNARIA_API UAttribute : public UObject
{
	GENERATED_BODY()
public:
	virtual void OpenModification(class UModification* Mod) {}
	virtual void CloseModification(class UModification* Mod) {}
};

/**
 *
 */
UCLASS(BlueprintType)
class LUNARIA_API UStat : public UAttribute
{
	GENERATED_BODY()
public:
	void Set(float InBase, float InScalar = 1.f);
	FORCEINLINE float Render() { return Base * Scalar; }
	virtual void OpenModification(class UModification* Mod) override;
	virtual void CloseModification(class UModification* Mod) override;

	float GetBase() const { return Base; }
	float GetScalar() const { return Scalar; }

protected:
	float Base = 0.f;
	float Scalar = 0.f;
};

UCLASS(BlueprintType)
class LUNARIA_API UColorAttribute : public UAttribute
{
	GENERATED_BODY()
public:
	void Set(const FLinearColor& InBase);
	FORCEINLINE FLinearColor Render() { return Color; }
	virtual void OpenModification(class UModification* Mod) override;
	virtual void CloseModification(class UModification* Mod) override;

protected:
	FLinearColor Base = FLinearColor::White;
	FLinearColor Color = FLinearColor::White;
};

class LUNARIA_API FStatComposite
{
public:
	void Add(float InBase) { Base += InBase; }
	void AddScale(float InScalar) { Scalar *= InScalar; }
	void Add(UStat* SingleStat) { SubStats.Add(SingleStat); }

	float Render() {
		float TotalBase = Base;
		float TotalScalar = Scalar;

		for (auto Stat : SubStats)
		{
			TotalBase += Stat->GetBase();
			TotalScalar *= Stat->GetScalar();
		}

		return TotalBase * TotalScalar;
	}

protected:
	float Base = 0.f;
	float Scalar = 1.f;
	TArray<UStat*> SubStats;
};
