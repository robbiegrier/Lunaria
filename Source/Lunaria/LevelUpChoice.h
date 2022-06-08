// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Choosable.h"
#include "LevelUpChoice.generated.h"

UCLASS()
class LUNARIA_API ALevelUpChoice : public AActor, public IChoosable
{
	GENERATED_BODY()

public:
	ALevelUpChoice();
	virtual void Tick(float DeltaTime) override;

	virtual FString GetChoiceName() override;
	virtual FString GetChoiceDescription() override;
	void Choose(class AActor* Chooser) override;

	void SetBoon(class ABoon* InBoon) { Boon = InBoon; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ABoon* Boon;
};
