// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EventLibraryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UEventLibraryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEventLibraryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AnnounceEnemyHitWithAttack(class AActor* HitActor);

protected:
	virtual void BeginPlay() override;
};
