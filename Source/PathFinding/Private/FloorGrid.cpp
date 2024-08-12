// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorGrid.h"

UFloorGrid::UFloorGrid()
{
	Columns = 15;
	Rows = 15;
	DiffBeetweenGridCells = 100.f;
	InitialPoint = FVector::ZeroVector;
}

void UFloorGrid::InitialGrid()
{
	Grid.SetNum(Rows);
	for (int32 i = 0; i < Rows; i++)
	{
		Grid[i].SetNum(Columns);
	}

	CreateGrid();
	setNeighbords();
}

void UFloorGrid::CreateGrid()
{
	for (int32 i = 0; i < Rows; i++)
	{
		for (int32 j = 0; j < Columns; j++)
		{
			GridCell* Temp = new GridCell(i, j, true);
			Grid[i][j] = Temp;
			Temp = nullptr;
			delete Temp;
		}
	}
}

void UFloorGrid::setNeighbords()
{
	for (int32 i = 0; i < Rows; i++)
	{
		for (int32 j = 0; j < Columns; j++)
		{
			GridCell* Temp = GetGridElement(i, j);
			
			if (Temp)
			{
				Temp->East = GetGridElement(i , j + 1);
				Temp->West = GetGridElement(i , j  -1);
				Temp->North = GetGridElement(i + 1 , j);
				Temp->South = GetGridElement(i - 1, j);
			}
		}
	}
}

FVector UFloorGrid::GetGridWorldPosition(int32 inX, int32 inY)
{
	FVector Location = InitialPoint;
	Location.X += inX * DiffBeetweenGridCells;
	Location.Y += inY * DiffBeetweenGridCells;

	return Location;
}

void UFloorGrid::GetCellCordinates(FVector ActiveLocation, int32& OutX, int32& OutY)
{
	FVector AdjustedVector = ActiveLocation - InitialPoint;

	OutX = FMath::FloorToInt32(AdjustedVector.X / DiffBeetweenGridCells);
	OutY = FMath::FloorToInt32(AdjustedVector.Y / DiffBeetweenGridCells);

	OutX = FMath::Clamp<int32>(OutX, 0, Rows - 1);
	OutY = FMath::Clamp<int32>(OutY, 0, Columns - 1);
}

void UFloorGrid::SetGridElement(int32 inX, int32 inY, bool bIsWalkable)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		/*GridCell* Temp = Grid[inX][inY];
		delete Temp;
		Temp = new GridCell(inX, inY, bIsWalkable);
		Grid[inX][inY] = nullptr;
		if (Temp)
		{
			Grid[inX][inY] = Temp;	
			Temp->East = GetGridElement(inX, inY + 1);
			if (GetGridElement(inX, inY + 1)) GetGridElement(inX, inY + 1)->West = Temp;

			Temp->West = GetGridElement(inX, inY - 1);
			if (GetGridElement(inX, inY - 1)) GetGridElement(inX, inY - 1)->East = Temp;

			Temp->North = GetGridElement(inX + 1, inY);
			if (GetGridElement(inX + 1, inY)) GetGridElement(inX + 1, inY)->North = Temp;

			Temp->South = GetGridElement(inX - 1, inY);
			if (GetGridElement(inX - 1, inY)) GetGridElement(inX - 1, inY)->South = Temp;
		}*/

		if (Grid[inX][inY])
		{
			delete Grid[inX][inY];
		}
		Grid[inX][inY] = new GridCell(inX, inY, bIsWalkable);

		GridCell* Temp = Grid[inX][inY];

		if (Temp)
		{
			// Komþu hücreleri al
			GridCell* EastNeighbor = GetGridElement(inX, inY + 1);
			GridCell* WestNeighbor = GetGridElement(inX, inY - 1);
			GridCell* NorthNeighbor = GetGridElement(inX + 1, inY);
			GridCell* SouthNeighbor = GetGridElement(inX - 1, inY);

			// Temp hücresinin komþularýný ata
			Temp->East = EastNeighbor;
			Temp->West = WestNeighbor;
			Temp->North = NorthNeighbor;
			Temp->South = SouthNeighbor;

			// Komþu hücrelere Temp hücresini ata
			if (EastNeighbor) EastNeighbor->West = Temp;
			if (WestNeighbor) WestNeighbor->East = Temp;
			if (NorthNeighbor) NorthNeighbor->South = Temp;
			if (SouthNeighbor) SouthNeighbor->North = Temp;
		}
	}
}

void UFloorGrid::SetGridElement(int32 inX, int32 inY, GridCell* GridPoint)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		if (Grid[inX][inY])
		{
			delete Grid[inX][inY];
		}
		Grid[inX][inY] = GridPoint;

		if (GridPoint)
		{
			GridCell* EastNeighbor = GetGridElement(inX, inY + 1);
			GridCell* WestNeighbor = GetGridElement(inX, inY - 1);
			GridCell* NorthNeighbor = GetGridElement(inX + 1, inY);
			GridCell* SouthNeighbor = GetGridElement(inX - 1, inY);

			GridPoint->East = EastNeighbor;
			GridPoint->West = WestNeighbor;
			GridPoint->North = NorthNeighbor;
			GridPoint->South = SouthNeighbor;

			if (EastNeighbor) EastNeighbor->West = GridPoint;
			if (WestNeighbor) WestNeighbor->East = GridPoint;
			if (NorthNeighbor) NorthNeighbor->South = GridPoint;
			if (SouthNeighbor) SouthNeighbor->North = GridPoint;
		}
	}
}

void UFloorGrid::GetGridElement(int32 inX, int32 inY, int32& OutX, int32& OutY, bool& bIsWalkable)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		GridCell* Temp = Grid[inX][inY];
		OutX = Temp->X;
		OutY = Temp->Y;
		bIsWalkable = Temp->bIsWalkable;
	}
}

GridCell* UFloorGrid::GetGridElement(int32 inX, int32 inY)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		return Grid[inX][inY];
	}
	return nullptr;
}
