// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LUNARIA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		int32 GetNodesHeld() const { return Nodes; }

	UFUNCTION(BlueprintCallable)
		int32 GetSpiceHeld() const { return Spice; }

	UFUNCTION(BlueprintCallable)
		void AddNodes(int32 Amount) { Nodes += Amount; }

	UFUNCTION(BlueprintCallable)
		void RemoveNodes(int32 Amount) { Nodes -= Amount; }

	UFUNCTION(BlueprintCallable)
		void AddSpice(int32 Amount) { Spice += Amount; }

	UFUNCTION(BlueprintCallable)
		void RemoveSpice(int32 Amount) { Spice -= Amount; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resources, meta = (AllowPrivateAccess = "true"))
		int32 Nodes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resources, meta = (AllowPrivateAccess = "true"))
		int32 Spice = 0;
};
