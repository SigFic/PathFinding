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
			if (i == 0 || i == Rows - 1)
			{
				if (i == 0)
				{
					Temp->South = nullptr;
					Temp->North = GetGridElement(i + 1, j);
				}
				else
				{
					Temp->South = GetGridElement(i - 1, j);
					Temp->North = nullptr;
				}
			}
			else
			{
				Temp->North = GetGridElement(i + 1, j);
				Temp->South = GetGridElement(i - 1, j);
			}

			if (j == 0 || j == Columns - 1)
			{
				if (j == 0)
				{
					Temp->East = GetGridElement(i, j + 1);
					Temp->West = nullptr;
				}

				else
				{
					Temp->East = nullptr;
					Temp->West = GetGridElement(i, j - 1);
				}

			}
			else
			{
				Temp->East = GetGridElement(i , j + 1);
				Temp->West = GetGridElement(i , j - 1);
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
		GridCell* Temp = Grid[inX][inY];
		delete Temp;
		Temp = new GridCell(inX, inY, bIsWalkable);
		Grid[inX][inY] = Temp;
	}
}

void UFloorGrid::SetGridElement(int32 inX, int32 inY, GridCell* GridPoint)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		GridCell* Temp = Grid[inX][inY];
		delete Temp;
		Grid[inX][inY] = GridPoint;
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
