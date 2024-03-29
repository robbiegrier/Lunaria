// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatusEffect.generated.h"

UCLASS()
class LUNARIA_API AStatusEffect : public AActor
{
	GENERATED_BODY()

public:
	AStatusEffect();
	virtual void Tick(float DeltaTime) override;
	void NativeOnAdded(class UAttributesComponent* Attributes);

	class ABoon* AddStack(AActor* Creator = nullptr);

	UClass* GetBoonClass() const { return BoonClass; }
	int32 GetCurrentStacks() const { return Statuses.Num(); }
	TArray<class ABoon*>& GetStatuses() { return Statuses; }

	void SetBoonClass(UClass* InClass) { BoonClass = InClass; }
	void SetDuration(float InDuration) { Duration = InDuration; }
	void SetMaxStacks(float InMax) { MaxStacksSeed = InMax; }

	UFUNCTION(BlueprintCallable)
		void Remove();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
	void ClearStatuses();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComponent;

	UClass* BoonClass;
	TArray<class ABoon*> Statuses;
	class UAttributesComponent* MyOwnerAttributes;
	float LastStackTime = 0.f;
	float Duration = 3.f;
	int32 MaxStacksSeed = 3;
};
