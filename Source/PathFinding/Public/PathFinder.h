// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathFinder.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API UPathFinder : public UObject
{
	GENERATED_BODY()
	
public:

	UPathFinder();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPath(int32 StartX, int32 StartY, int32 EndX, int32 EndY, class UFloorGrid* FloorGrid);

private:

	float CalculateHeuristic(struct GridCell* CurrentCell, GridCell* TargetCell);
	float CalculateG();
	float CalculateF(GridCell* CurrenCell, GridCell* EndCell);
	TArray<GridCell*> ReconstructPath(TMap<GridCell*, GridCell*> CameFrom, GridCell* CurrentCell);
	TArray< GridCell*> FindPath(UFloorGrid* Grid, GridCell* StartCell, GridCell* EndCell);
	TArray< GridCell*> GetNeighbors(GridCell* ActiveCell);

};
