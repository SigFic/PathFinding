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

	/*Came Back here later and redesign to Walking system*/
	//if (!End->bIsWalkable)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("End Grid cell is not walkable"));
	//	return TArray<FVector>();
	//}

	TArray<GridCell* > PathInGridCells = FindPath(FloorGrid, Start, End);
	UE_LOG(LogTemp, Warning, TEXT("pathSize : %d"), PathInGridCells.Num());
	TArray<FVector> PathInVector;

	for (int32 i = 0; i < PathInGridCells.Num(); i++)
	{
		FVector CurrentVector = FloorGrid->GetGridWorldPosition(PathInGridCells[i]->X, PathInGridCells[i]->Y);
		CurrentVector.Z = (PathInGridCells[i]->Z * FloorGrid->GetDistanceBeetweenCells());
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


		for (GridCell* Neighbor : GetNeighbors(Current))
		{
			//if (!Neighbor || !Neighbor->bIsWalkable) continue;
			if (!Neighbor || FMath::Abs(Current->Z - Neighbor->Z) > 1) continue;

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

TArray<GridCell*> UPathFinder::GetNeighbors(GridCell* ActiveCell)
{
	/*TArray<GridCell*> Neighbors;

	for (int i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0:
			if (ActiveCell->Ways[0] && (FMath::Abs(ActiveCell->Z - ActiveCell->Ways[0]->Z) <= 1)) Neighbors.Add(ActiveCell->Ways[0]);
			break;

		case 1:
			if (!ActiveCell->Ways[1]) continue;
			if ((FMath::Abs(ActiveCell->Z - ActiveCell->Ways[1]->Z) > 1)) continue;

			if (ActiveCell->Ways[0] || ActiveCell->Ways[2]) 
			{
				if ((ActiveCell->Ways[0]->Z - ActiveCell->Z) > 1 && (ActiveCell->Ways[2]->Z - ActiveCell->Z) > 1) continue;
				Neighbors.Add(ActiveCell->Ways[1]);
				break;
			}

			Neighbors.Add(ActiveCell->Ways[1]);
			break;

		case 2:
			if (ActiveCell->Ways[2] && (FMath::Abs(ActiveCell->Z - ActiveCell->Ways[2]->Z) <= 1)) Neighbors.Add(ActiveCell->Ways[2]);
			break;

		case 3:
			if (!ActiveCell->Ways[3]) continue;
			if ((FMath::Abs(ActiveCell->Z - ActiveCell->Ways[3]->Z) > 1)) continue;

			if (ActiveCell->Ways[2] || ActiveCell->Ways[4])
			{
				if ((ActiveCell->Ways[2]->Z - ActiveCell->Z) > 1 && (ActiveCell->Ways[4]->Z - ActiveCell->Z) > 1) continue;
				Neighbors.Add(ActiveCell->Ways[1]);
				break;
			}

			Neighbors.Add(ActiveCell->Ways[1]);
			break;

		case 4:
			if (ActiveCell->Ways[4] && (FMath::Abs(ActiveCell->Z - ActiveCell->Ways[4]->Z) <= 1)) Neighbors.Add(ActiveCell->Ways[4]);
			break;

		case 5:
			if (!ActiveCell->Ways[5]) continue;
			if ((FMath::Abs(ActiveCell->Z - ActiveCell->Ways[5]->Z) > 1)) continue;

			if (ActiveCell->Ways[4] || ActiveCell->Ways[5])
			{
				if ((ActiveCell->Ways[4]->Z - ActiveCell->Z) > 1 && (ActiveCell->Ways[5]->Z - ActiveCell->Z) > 1) continue;
				Neighbors.Add(ActiveCell->Ways[1]);
				break;
			}

			Neighbors.Add(ActiveCell->Ways[1]);
			break;

		case 6:
			if (ActiveCell->Ways[6] && (FMath::Abs(ActiveCell->Z - ActiveCell->Ways[6]->Z) <= 1)) Neighbors.Add(ActiveCell->Ways[6]);
			break;

		case 7:
			if (!ActiveCell->Ways[7]) continue;
			if ((FMath::Abs(ActiveCell->Z - ActiveCell->Ways[7]->Z) > 1)) continue;

			if (ActiveCell->Ways[0] || ActiveCell->Ways[6])
			{
				if ((ActiveCell->Ways[0]->Z - ActiveCell->Z) > 1 && (ActiveCell->Ways[6]->Z - ActiveCell->Z) > 1) continue;
				Neighbors.Add(ActiveCell->Ways[1]);
				break;
			}

			Neighbors.Add(ActiveCell->Ways[1]);
			break;

		default:
			break;
		}
	}
	return Neighbors;*/
	TArray<GridCell*> Neighbors;

	for (int i = 0; i < 8; i++)
	{
		GridCell* Neighbor = ActiveCell->Ways[i];
		if (!Neighbor) continue; // Eðer komþu hücre yoksa atla

		// Z seviyesindeki farký kontrol et
		if (FMath::Abs(ActiveCell->Z - Neighbor->Z) > 1) continue;

		// Köþe komþular için ek kontrol
		if (i % 2 == 1) // Köþe komþularý 1, 3, 5, 7
		{
			GridCell* Adjacent1 = ActiveCell->Ways[(i - 1 + 8) % 8]; // Önceki komþu
			GridCell* Adjacent2 = ActiveCell->Ways[(i + 1) % 8];     // Sonraki komþu

			if (Adjacent1 && Adjacent2)
			{
				// Eðer her iki komþu da mevcut ve Z farký 1'den büyükse bu köþe komþusunu eklemeyin
				if (FMath::Abs(Adjacent1->Z - ActiveCell->Z) > 1 && FMath::Abs(Adjacent2->Z - ActiveCell->Z) > 1)
				{
					continue;
				}
			}
		}

		Neighbors.Add(Neighbor);
	}

	return Neighbors;
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




