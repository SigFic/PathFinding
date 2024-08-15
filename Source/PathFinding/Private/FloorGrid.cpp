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
				Temp->NorthEast = GetGridElement(i + 1, j + 1);
				Temp->NorthWest = GetGridElement(i + 1, j - 1);
				Temp->SouthEast = GetGridElement(i - 1, j + 1);
				Temp->SouthWest = GetGridElement(i - 1, j - 1);
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

	if (OutX < 0 || OutX >= Rows || OutY < 0 || OutY >= Columns)
	{
		OutX = -1;
		OutY = -1;
	}
	//OutX = FMath::Clamp<int32>(OutX, 0, Rows - 1);
	//OutY = FMath::Clamp<int32>(OutY, 0, Columns - 1);
}

void UFloorGrid::SetGridElement(int32 inX, int32 inY, bool bIsAvailable)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		if (Grid[inX][inY])
		{
			delete Grid[inX][inY];
		}
		Grid[inX][inY] = new GridCell(inX, inY, bIsAvailable);

		GridCell* Temp = Grid[inX][inY];

		if (Temp)
		{

			GridCell* EastNeighbor = GetGridElement(inX, inY + 1);
			GridCell* WestNeighbor = GetGridElement(inX, inY - 1);
			GridCell* NorthNeighbor = GetGridElement(inX + 1, inY);
			GridCell* SouthNeighbor = GetGridElement(inX - 1, inY);
			GridCell* SouthEastNeighbor = GetGridElement(inX - 1, inY + 1);
			GridCell* SouthWestNeighbor = GetGridElement(inX - 1, inY - 1);
			GridCell* NorthEastNeighbor = GetGridElement(inX + 1, inY + 1);
			GridCell* NorthWestNeighbor = GetGridElement(inX + 1, inY - 1);

			Temp->East = EastNeighbor;
			Temp->West = WestNeighbor;
			Temp->North = NorthNeighbor;
			Temp->South = SouthNeighbor;
			Temp->NorthEast = NorthEastNeighbor;
			Temp->NorthWest = NorthWestNeighbor;
			Temp->SouthWest = SouthWestNeighbor;
			Temp->SouthEast = SouthEastNeighbor;

			if (EastNeighbor) EastNeighbor->West = Temp;
			if (WestNeighbor) WestNeighbor->East = Temp;
			if (NorthNeighbor) NorthNeighbor->South = Temp;
			if (SouthNeighbor) SouthNeighbor->North = Temp;
			if (SouthEastNeighbor) SouthEastNeighbor->NorthWest = Temp;
			if (SouthWestNeighbor) SouthWestNeighbor->NorthEast = Temp;
			if (NorthEastNeighbor) NorthEastNeighbor->SouthWest = Temp;
			if (NorthWestNeighbor) NorthWestNeighbor->SouthEast = Temp;
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
			GridCell* SouthEastNeighbor = GetGridElement(inX - 1, inY + 1);
			GridCell* SouthWestNeighbor = GetGridElement(inX - 1, inY - 1);
			GridCell* NorthEastNeighbor = GetGridElement(inX + 1, inY + 1);
			GridCell* NorthWestNeighbor = GetGridElement(inX + 1, inY - 1);

			GridPoint->East = EastNeighbor;
			GridPoint->West = WestNeighbor;
			GridPoint->North = NorthNeighbor;
			GridPoint->South = SouthNeighbor;
			GridPoint->NorthEast = NorthEastNeighbor;
			GridPoint->NorthWest = NorthWestNeighbor;
			GridPoint->SouthWest = SouthWestNeighbor;
			GridPoint->SouthEast = SouthEastNeighbor;

			if (EastNeighbor) EastNeighbor->West = GridPoint;
			if (WestNeighbor) WestNeighbor->East = GridPoint;
			if (NorthNeighbor) NorthNeighbor->South = GridPoint;
			if (SouthNeighbor) SouthNeighbor->North = GridPoint;
			if (SouthEastNeighbor) SouthEastNeighbor->NorthWest = GridPoint;
			if (SouthWestNeighbor) SouthWestNeighbor->NorthEast = GridPoint;
			if (NorthEastNeighbor) NorthEastNeighbor->SouthWest = GridPoint;
			if (NorthWestNeighbor) NorthWestNeighbor->SouthEast = GridPoint;
		}
	}
}

void UFloorGrid::GetGridElement(int32 inX, int32 inY, int32& OutX, int32& OutY, bool& bIsAvailable)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		GridCell* Temp = Grid[inX][inY];
		OutX = Temp->X;
		OutY = Temp->Y;
		bIsAvailable = Temp->bIsWalkable;
	}

	else
	{
		OutX = -1;
		OutY = -1;
		bIsAvailable = false;
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

void UFloorGrid::SetCellbIsWalkable(GridCell* Cell)
{
	if (!Cell) return;
	if (!Cell->bIsWalkable)
	{
		return;
	}

	int32 X = Cell->X;
	int32 Y = Cell->Y;

	if (!GetGridElement(X + 1, Y + 1)->bIsWalkable && !GetGridElement(X - 1, Y - 1)->bIsWalkable)
	{
		return;
	}
	else if (!GetGridElement(X - 1, Y + 1)->bIsWalkable && !GetGridElement(X + 1, Y - 1)->bIsWalkable)
	{
		return;
	}

}
