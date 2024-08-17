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
	if (MeshComponent) Meshes.Add(MeshComponent);

	TArray<USceneComponent*> ChildComponents = ParenComponent->GetAttachChildren();
	for (USceneComponent* ChildComp : ChildComponents)
	{
		UStaticMeshComponent* ChildMeshComponent = Cast<UStaticMeshComponent>(ChildComp);
		if(ChildMeshComponent) Meshes.AddUnique(ChildMeshComponent);
	}

	APathFindingGameMode* GM = Cast<APathFindingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->OnGridStartDelegate.AddDynamic(this, &UPlaceable::WhenGridCreated);
	}
}

void UPlaceable::WhenGridCreated(UFloorGrid* GridRef)
{
	SetSizeVectors(GridRef->GetDistanceBeetweenCells());
	for (UStaticMeshComponent* MeshRef : Meshes)
	{
		FVector Location = FVector::ZeroVector;
		Location.X = -(((float)SizeVectors[MeshRef].X) / 2) * GridRef->GetDistanceBeetweenCells();
		Location.Y = -(((float)SizeVectors[MeshRef].Y) / 2) * GridRef->GetDistanceBeetweenCells();

		Location = Location + GetComponentLocation();
		SetLocationProperties(Location, MeshRef);
	}
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

	int32 SizeX = (int32)SizeVectors[MeshRef].X;
	int32 SizeY = (int32)SizeVectors[MeshRef].Y;

	if (NewLocation.X != CheckVector.X) SizeX++;
	if (NewLocation.Y != CheckVector.Y) SizeY++;

	for (int32 i = XLocation; i < (XLocation + SizeX); i++)
	{
		for (int32 j = YLocation; j < (YLocation + SizeY); j++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("i and j : %d , %d"), i, j);	
			if (GM->GridRef->GetGridElement(i, j))GM->GridRef->GetGridElement(i, j)->bIsWalkable = false;
			if (GM->GridRef->GetGridElement(i, j))GM->GridRef->GetGridElement(i, j)->Z = SizeVectors[MeshRef].Z;
		}
	}
}

void UPlaceable::SetSizeVectors(float DistanceBetweenGrids)
{
	for (UStaticMeshComponent* MeshRef : Meshes)
	{
		if (!MeshRef) continue;
		FVector BoundsVector = MeshRef->GetStaticMesh()->GetBounds().GetBox().GetSize();
		BoundsVector = BoundsVector * MeshRef->GetComponentTransform().GetScale3D();

		int32 X = FMath::FloorToInt32(BoundsVector.X / DistanceBetweenGrids);
		int32 Y = FMath::FloorToInt32(BoundsVector.Y / DistanceBetweenGrids);
		float Z = FMath::CeilToInt32(BoundsVector.Z / DistanceBetweenGrids);

		FVector SizeVector = FVector((float)X, (float)Y, Z);

		SizeVectors.Add(MeshRef, SizeVector);
	}
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

}

