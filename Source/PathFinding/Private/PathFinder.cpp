// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"
#include "FloorGrid.h"

UPathFinder::UPathFinder()
{
}

TArray<FVector> UPathFinder::GetPath(int32 StartX, int32 StartY, int32 EndX, int32 EndY, UFloorGrid* FloorGrid)
{
	if (!FloorGrid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Floor grid is not available"));
		return TArray<FVector>();
	}

	UE_LOG(LogTemp, Warning, TEXT("Floor grid is available"));
	GridCell* Start = FloorGrid->GetGridElement(StartX, StartY);
	GridCell* End = FloorGrid->GetGridElement(EndX, EndY);

	if (!End->bIsWalkable)
	{
		UE_LOG(LogTemp, Warning, TEXT("End Grid cell is not walkable"));
		return TArray<FVector>();
	}
	TArray<GridCell* > PathInGridCells = FindPath(FloorGrid, Start, End);
	UE_LOG(LogTemp, Warning, TEXT("pathSize : %d"), PathInGridCells.Num());
	TArray<FVector> PathInVector;

	for (int32 i = 0; i < PathInGridCells.Num(); i++)
	{
		FVector CurrentVector = FloorGrid->GetGridWorldPosition(PathInGridCells[i]->X, PathInGridCells[i]->Y);
		PathInVector.Add(CurrentVector);
	}
	return PathInVector;
}

TArray<GridCell*> UPathFinder::FindPath(UFloorGrid* Grid, GridCell* StartCell, GridCell* EndCell)
{
	TArray<GridCell*> OpenSet;
	OpenSet.Add(StartCell);

	TMap<GridCell*, GridCell*> CameFrom;

	TMap<GridCell*, float> GScores;
	TMap<GridCell*, float> FScores;

	GScores.Add(StartCell, 0.0f);
	FScores.Add(StartCell, CalculateHeuristic(StartCell, EndCell));

	while (OpenSet.Num() > 0)
	{
		// get the node in openSet have the lowest FScore value
		GridCell* Current = OpenSet[0];
		for (GridCell* Cell : OpenSet)
		{
			if (FScores.Contains(Cell) && FScores[Cell] < FScores[Current])
			{
				Current = Cell;
			}
		}

		if (Current == EndCell)
		{
			return ReconstructPath(CameFrom, Current);
		}

		OpenSet.Remove(Current);

		TArray<GridCell*> Neighbors;
		if (Current->East) Neighbors.Add(Current->East);
		if (Current->West) Neighbors.Add(Current->West);
		if (Current->North) Neighbors.Add(Current->North);
		if (Current->South) Neighbors.Add(Current->South);

		for (GridCell* Neighbor : Neighbors)
		{
			if (!Neighbor || !Neighbor->bIsWalkable) continue;

			float TentativeGScore = GScores[Current] + CalculateG();

			if (!GScores.Contains(Neighbor) || TentativeGScore < GScores[Neighbor])
			{
				CameFrom.Add(Neighbor, Current);
				GScores.Add(Neighbor, TentativeGScore);
				FScores.Add(Neighbor, TentativeGScore + CalculateHeuristic(Neighbor, EndCell));

				if (!OpenSet.Contains(Neighbor))
				{
					OpenSet.Add(Neighbor);
				}
			}
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("No path found"));
	return TArray<GridCell*>();
}

float UPathFinder::CalculateHeuristic(GridCell* CurrentCell, GridCell* TargetCell)
{
	//manhattan Function(Manhattan Distance)
	return FMath::Abs(CurrentCell->X - TargetCell->X) + FMath::Abs(CurrentCell->Y - CurrentCell->Y);
}

float UPathFinder::CalculateG()
{
	// Assuming a uniform cost for moving from one cell to another
	return 1.0f;
}

float UPathFinder::CalculateF(GridCell* CurrenCell, GridCell* EndCell)
{
	// F = G + H
	return CalculateG() + CalculateHeuristic(CurrenCell, EndCell);
}

TArray<GridCell*> UPathFinder::ReconstructPath(TMap<GridCell*, GridCell*> CameFrom, GridCell* CurrentCell)
{
	TArray<GridCell*> Path;
	Path.Add(CurrentCell);

	while (CameFrom.Contains(CurrentCell))
	{
		CurrentCell = CameFrom[CurrentCell];
		Path.Insert(CurrentCell, 0);
	}

	return Path;
}




