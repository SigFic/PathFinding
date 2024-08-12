// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObjects.h"
#include "Kismet/GameplayStatics.h"
#include "PathFindingGameMode.h"
#include "FloorGrid.h"


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
	
	SetObjectSizeXandY();
	SetLocationProperties(GetActorLocation());
}

// Called every frame
void ABaseObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseObjects::SetObjectLocation(const FVector& NewLocation, const float GridCellDistance)
{
	FVector SpawnLocation = FVector::ZeroVector;

	SpawnLocation.X = (((float)SizeX) / 2) * GridCellDistance;
	SpawnLocation.Y = (((float)SizeY) / 2) * GridCellDistance;

	SpawnLocation = SpawnLocation + NewLocation;

	SetActorLocation(SpawnLocation);
	SetLocationProperties(NewLocation);
}

void ABaseObjects::SetLocationProperties(const FVector& NewLocation)
{
	APathFindingGameMode* GM = Cast<APathFindingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM || !GM->GridRef) return;
	int32 XLocation, YLocation;
	GM->GridRef->GetCellCordinates(NewLocation, XLocation, YLocation);
	
	if (XLocation < 0 || XLocation + SizeX > GM->GridRef->GetRows()) return;
	if (YLocation < 0 || YLocation + SizeY > GM->GridRef->GetColumns()) return;

	for (int32 i = XLocation; i < (XLocation + SizeX); i++)
	{
		for (int32 j = YLocation; j < (YLocation + SizeY); j++)
		{
			GridCell* Ref = GM->GridRef->GetGridElement(i, j);
			Ref->bIsWalkable = false;
		}
	}

}

void ABaseObjects::SetObjectSizeXandY()
{
	FVector ScaleVector = ObjectMesh->GetComponentScale();
	SizeX = FMath::FloorToInt32(ScaleVector.X);
	SizeY = FMath::FloorToInt32(ScaleVector.Y);
}

