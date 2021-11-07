// Robbie Grier

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DetailTogglable.h"
#include "MapManager.generated.h"

UCLASS()
class LUNARIA_API AMapManager : public AActor, public IDetailTogglable
{
	GENERATED_BODY()

public:
	AMapManager();
	virtual void Tick(float DeltaTime) override;

	virtual void ToggleDetailOn();
	virtual void ToggleDetailOff();
	void SetWallsDetailFlag(float Value);

	// Manually load a new map
	void LoadNewMap(float Scale, const FVector& ExitDirection, int32 NumberOfDoors);

	// Get the center of the circular map
	UFUNCTION(BlueprintCallable)
		const FVector& GetCenter() const { return Center; }

	// Get the radius of the circular map
	UFUNCTION(BlueprintCallable)
		float GetRadius() const { return Radius; }

	// Get the actor representing the entrance
	UFUNCTION(BlueprintCallable)
		class AActor* GetEntrance() const { return Entrance; }

	// Get the direction from which the map was entered
	UFUNCTION(BlueprintCallable)
		const FVector& GetEntryDirection() const { return EntryDirection; }

	// Sets the backgorund color
	UFUNCTION(BlueprintCallable)
		void SetCosmicSoup(const FVector& Color);

	// Sets the backgorund color to a nice random color
	UFUNCTION(BlueprintCallable)
		void SetRandomCosmicSoup() { SetCosmicSoup(FVector(FMath::RandRange(0.05f, 0.15f), FMath::RandRange(0.05f, 0.15f), FMath::RandRange(0.05f, 0.15f))); }

	UFUNCTION(BlueprintCallable)
		FVector GetNearestPointInsideMap(const FVector& Point, float Feathering = 0.f);

	UFUNCTION(BlueprintCallable)
		void OpenCurrentDoors();

	UFUNCTION(BlueprintCallable)
		void CloseCurrentDoors();

	UFUNCTION(BlueprintCallable)
		FVector GetRandomPointInsideMap();

protected:
	virtual void BeginPlay() override;

private:
	// Set up visual components based on map properties such as radius/center
	void InitializeCircleVisualization();

	// Get a random point along the map circle's edge
	FVector GetRandomPointOnEdge(float Feathering = 0.f);

	// Setup the entrance and entry direction based on the exit of a previous map
	void SetupNewEntry(const FVector& ExitDirection);

	// Destroy and forget the current doors
	void ClearCurrentDoors();

	// Spawn NumberOfDoors doors at random points along the map circle
	void SpawnRandomDoors(int32 NumberOfDoors);

	// Set the radius
	void SetRadius(float R) { Radius = R; }

	void ClearCurrentEnvironmentActors();

	void CleanupPreviousMap();

	UClass* GetRandomEnvironmentActorClass();

	AActor* SpawnEnvironmentActor(const FVector& Location);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
		FVector Center;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
		float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
		float DoorFeathering{ 50.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
		float CharacterHeight{ 100.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dimensions, meta = (AllowPrivateAccess = "true"))
		float MapElementHeight{ 90.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CircleVisualization;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CenterVisualization;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CosmicBackgroundVisualization;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visualization, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ArenaWalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> EntranceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ADoor> DoorClass;

	UPROPERTY()
		AActor* Entrance;

	UPROPERTY()
		TArray<class ADoor*> CurrentDoors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Actors", meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<class AActor>> EnvironmentActorClasses;

	UPROPERTY()
		TArray<class AActor*> CurrentEnvironmentActors;

	FVector EntryDirection;
};
