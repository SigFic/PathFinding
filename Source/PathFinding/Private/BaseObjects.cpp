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
	//SetLocationProperties(GetActorLocation());

}

// Called when the game starts or when spawned
void ABaseObjects::BeginPlay()
{
	Super::BeginPlay();
	
	SetObjectSizeXandY();
	APathFindingGameMode* GM = Cast<APathFindingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->OnGridStartDelegate.AddDynamic(this, &ABaseObjects::WhenGridCreated);
	}
}

void ABaseObjects::WhenGridCreated(UFloorGrid* GridRef)
{
	FVector Location = FVector::ZeroVector;
	Location.X = -(((float)SizeX) / 2) * GridRef->GetDistanceBeetweenCells();
	Location.Y = -(((float)SizeY) / 2) * GridRef->GetDistanceBeetweenCells();

	Location = Location + GetActorLocation();
	SetLocationProperties(Location);
}

// Called every frame
void ABaseObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseObjects::SetObjectLocation(const FVector& NewLocation, const float GridCellDistance)
{
	FVector SpawnLocation = FVector::ZeroVector;

	SpawnLocation.X = ((ObjectMesh->GetComponentScale().X / 2) * GridCellDistance);
	SpawnLocation.Y = ((ObjectMesh->GetComponentScale().Y / 2) * GridCellDistance);
	SpawnLocation.Z = ((ObjectMesh->GetComponentScale().Z / 2) * GridCellDistance);

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
	
	if (XLocation == -1 || YLocation == -1) return;
	//UE_LOG(LogTemp, Warning, TEXT("X and Y : %d , %d"), XLocation, YLocation);
	FVector CheckVector = GM->GridRef->GetGridWorldPosition(XLocation, YLocation);
	if (NewLocation.X != CheckVector.X) SizeX++;
	if (NewLocation.Y != CheckVector.Y) SizeY++;

	for (int32 i = XLocation; i < (XLocation + SizeX); i++)
	{
		for (int32 j = YLocation; j < (YLocation + SizeY); j++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("i and j : %d , %d"), i, j);	
			if(GM->GridRef->GetGridElement(i,j))GM->GridRef->GetGridElement(i, j)->bIsWalkable = false;
			if(GM->GridRef->GetGridElement(i,j))GM->GridRef->GetGridElement(i, j)->Z = Hight;
		}
	}

}

void ABaseObjects::SetObjectSizeXandY()
{
	FVector ScaleVector = ObjectMesh->GetComponentScale();
	SizeX = FMath::FloorToInt32(ScaleVector.X);
	SizeY = FMath::FloorToInt32(ScaleVector.Y);
	Hight = FMath::CeilToInt32(ScaleVector.Z);
	//FMath::Toint32
}

