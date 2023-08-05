// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Stat.generated.h"

UCLASS(BlueprintType)
class LUNARIA_API UAttribute : public UObject
{
	GENERATED_BODY()
public:
	void OpenModification(class UModification* Mod) { Mods.Add(Mod); }
	void CloseModification(class UModification* Mod) { Mods.Remove(Mod); }
	void SetAttributeName(const FGameplayTag& InName) { AttributeName = InName; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag AttributeName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UModification*> Mods;
};

UCLASS(BlueprintType)
class LUNARIA_API ULunariaStat : public UAttribute
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void Set(float InBase) { Base = InBase; }

	UFUNCTION(BlueprintCallable)
		float Render(class UAction const* const Action);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Base = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Scalar = 1.f;
};

UCLASS(BlueprintType)
class LUNARIA_API ULunariaColor : public UAttribute
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void Set(FLinearColor InBase) { Base = InBase; }

	UFUNCTION(BlueprintCallable)
		FLinearColor Render(class UAction* Action);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor Base = FLinearColor::White;
};