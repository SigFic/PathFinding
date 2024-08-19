// Fill out your copyright notice in the Description page of Project Settings.


#include "Placeable.h"
#include "FloorGrid.h"
#include "Kismet/GameplayStatics.h"
#include "PathFindingGameMode.h"

// Sets default values for this component's properties
UPlaceable::UPlaceable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlaceable::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* ParenComponent = GetAttachParent();
	if (!ParenComponent) return;

	UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ParenComponent);
	if (MeshComponent) ParentStaticMeshRef = MeshComponent;

	APathFindingGameMode* GM = Cast<APathFindingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->OnGridStartDelegate.AddDynamic(this, &UPlaceable::WhenGridCreated);
	}
}

void UPlaceable::WhenGridCreated(UFloorGrid* GridRef)
{
	SetSizeVectors(GridRef->GetDistanceBeetweenCells());

	FVector Location = FVector::ZeroVector;
	Location.X = -(((float)SizeVector.X) / 2) * GridRef->GetDistanceBeetweenCells();
	Location.Y = -(((float)SizeVector.Y) / 2) * GridRef->GetDistanceBeetweenCells();

	Location = Location + GetComponentLocation();
	SetLocationProperties(Location, ParentStaticMeshRef);
}

void UPlaceable::SetLocationProperties(const FVector& NewLocation, UStaticMeshComponent* MeshRef)
{
	APathFindingGameMode* GM = Cast<APathFindingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM || !GM->GridRef) return;
	int32 XLocation, YLocation;
	GM->GridRef->GetCellCordinates(NewLocation, XLocation, YLocation);

	if (XLocation == -1 || YLocation == -1) return;
	//UE_LOG(LogTemp, Warning, TEXT("X and Y : %d , %d"), XLocation, YLocation);
	FVector CheckVector = GM->GridRef->GetGridWorldPosition(XLocation, YLocation);

	int32 SizeX = SizeVector.X;
	int32 SizeY = SizeVector.Y;

	if (FMath::Abs(NewLocation.X - CheckVector.X) > (float)GM->GridRef->GetDistanceBeetweenCells() / 2.f ) XLocation++;
	if (FMath::Abs(NewLocation.Y - CheckVector.Y) > (float)GM->GridRef->GetDistanceBeetweenCells() / 2.f ) YLocation++;
	//if (NewLocation.Y != CheckVector.Y) SizeY++;

	for (int32 i = XLocation; i < (XLocation + SizeX); i++)
	{
		for (int32 j = YLocation; j < (YLocation + SizeY); j++)
		{	
			//if (GM->GridRef->GetGridElement(i, j))GM->GridRef->GetGridElement(i, j)->bIsWalkable = false;
			if (GM->GridRef->GetGridElement(i, j))GM->GridRef->GetGridElement(i, j)->Z = SizeVector.Z;
		}
	}
}

void UPlaceable::SetSizeVectors(float DistanceBetweenGrids)
{

	if (!ParentStaticMeshRef) return;
	FVector BoundsVector = ParentStaticMeshRef->GetStaticMesh()->GetBounds().GetBox().GetSize();
	BoundsVector = BoundsVector * ParentStaticMeshRef->GetComponentTransform().GetScale3D();

	int32 X = FMath::FloorToInt32(BoundsVector.X / DistanceBetweenGrids);
	int32 Y = FMath::FloorToInt32(BoundsVector.Y / DistanceBetweenGrids);
	float Z = FMath::CeilToInt32(BoundsVector.Z / DistanceBetweenGrids);

	SizeVector = FVector((float)X, (float)Y, Z);

}

// Called every frame
void UPlaceable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("length : %d"), Meshes.Num());
	if (Meshes.Num() > 0)
	{
		if (Meshes[0])
		{
			FVector BoundsVector = Meshes[0]->GetStaticMesh()->GetBounds().GetBox().GetSize();
			BoundsVector = BoundsVector * Meshes[0]->GetComponentTransform().GetScale3D();
			UE_LOG(LogTemp, Warning, TEXT("length : %f"), SizeVectors[Meshes[0]].Z);
		}
	}
	// ...
}

void UPlaceable::SetObjectLocation(const FVector& NewLocation, float DistanceBeetweenGrids)
{
	FVector SpawnLocation = FVector::ZeroVector;

	SpawnLocation.X = ((ParentStaticMeshRef->GetComponentScale().X / 2) * DistanceBeetweenGrids);
	SpawnLocation.Y = ((ParentStaticMeshRef->GetComponentScale().Y / 2) * DistanceBeetweenGrids);
	SpawnLocation.Z = ((ParentStaticMeshRef->GetComponentScale().Z / 2) * DistanceBeetweenGrids);

	SpawnLocation = SpawnLocation + NewLocation;

	AActor* ParentOwner = ParentStaticMeshRef->GetOwner();
	ParentOwner->SetActorLocation(SpawnLocation);

	//SetComponentLocation(SpawnLocation);
	SetLocationProperties(NewLocation, ParentStaticMeshRef);
}

