// Robbie Grier


#include "CommonActor.h"

// Sets default values
ACommonActor::ACommonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACommonActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

