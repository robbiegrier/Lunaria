// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonActor.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FLunariaModificationCallback, class UAction*, Action, class UModificationLunariaStat*, Mod);

UCLASS()
class LUNARIA_API ACommonActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACommonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
