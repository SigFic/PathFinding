// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObjects.h"

// Sets default values
ABaseObjects::ABaseObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));

}

// Called when the game starts or when spawned
void ABaseObjects::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseObjects::SetObjectLocation(const FVector& NewLocation, const float GridCellDistance)
{
	FVector SpawnLocation = FVector::ZeroVector;

	SpawnLocation.X = (SizeX - 1) * GridCellDistance;
	SpawnLocation.Y = (SizeY - 1) * GridCellDistance;

	SpawnLocation = SpawnLocation + NewLocation;

	SetActorLocation(SpawnLocation);
}



void ABaseObjects::SetLocationProperties()
{
}

