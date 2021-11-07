// Robbie Grier

#pragma once

#include "CoreMinimal.h"

/**
 * Put Helpers Here
 */
class LUNARIA_API Helpers
{
public:
	// Get the component/mesh of an actor that should be used for flair movement, can be skeletal or static.
	static class USceneComponent* GetMeshForFlairMovement(class AActor* Actor);

	// Scale a value based on the tick rate of the user's game.
	static float Dilate(float Val, class UWorld* WorldRelativeTo);

	// Get the closest object in the Array to the Location. Optionally Ignore one of the objects (usually the observer/caller)
	template <typename T>
	static T* GetClosestToLocation(const FVector& Location, const TArray<T*>& Array, T* Ignore = nullptr)
	{
		if (Array.Num() == 0) return nullptr;
		if (Array.Num() == 1 && Array[0] == Ignore) return nullptr;
		auto CurrentClosestElement = Array[0];
		auto CurrentMinDistance = FVector::DistSquared(Location, Array[0]->GetActorLocation());

		for (auto Element : Array)
		{
			auto Distance = FVector::DistSquared(Location, Element->GetActorLocation());

			if (Distance < CurrentMinDistance && Element != Ignore)
			{
				CurrentClosestElement = Element;
				CurrentMinDistance = Distance;
			}
		}

		return CurrentClosestElement;
	}

	// Get the Sister component to a given component. Helpful for getting other component types without interacting with the owner type.
	template <typename SisterType, typename InType>
	static SisterType* GetSister(const InType* Context)
	{
		auto Sister = Context->GetOwner()->FindComponentByClass<SisterType>();
		checkf(Sister, TEXT("%s does not have a Sister of type SisterType"), *(Context->GetFullName()));
		return Sister;
	}

	static bool IsPointInsideSphere(const FVector& InPoint, const FVector& Center, float Radius);

	static FVector NearestPointInsideSphere(const FVector& InPoint, const FVector& Center, float Radius);

	template <typename ElementType>
	static ElementType GetRandomArrayElement(const TArray<ElementType>& Collection)
	{
		return Collection[FMath::RandRange(0, Collection.Num() - 1)];
	}

	static FVector GetRandomDirection2D()
	{
		return FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f).GetSafeNormal();
	}

	template <typename T, typename I>
	static bool IsValidIndex(const TArray<T>& Arr, I Index)
	{
		return Index >= 0 && Index < Arr.Num();
	}
};
