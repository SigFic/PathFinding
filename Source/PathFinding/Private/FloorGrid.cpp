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
			GridCell* Temp = new GridCell(i, j, 0);
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
				Temp->Ways[2] = GetGridElement(i , j + 1);
				Temp->Ways[6] = GetGridElement(i , j  -1);
				Temp->Ways[0] = GetGridElement(i + 1, j);
				Temp->Ways[4] = GetGridElement(i - 1, j);
				Temp->Ways[1] = GetGridElement(i + 1, j + 1);
				Temp->Ways[7] = GetGridElement(i + 1, j - 1);
				Temp->Ways[3] = GetGridElement(i - 1, j + 1);
				Temp->Ways[5] = GetGridElement(i - 1, j - 1);
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

void UFloorGrid::SetGridElement(int32 inX, int32 inY, int32 inZ)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		if (Grid[inX][inY])
		{
			delete Grid[inX][inY];
		}
		Grid[inX][inY] = new GridCell(inX, inY, inZ);

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

			Temp->Ways[2] = EastNeighbor;
			Temp->Ways[6] = WestNeighbor;
			Temp->Ways[0] = NorthNeighbor;
			Temp->Ways[4] = SouthNeighbor;
			Temp->Ways[1] = NorthEastNeighbor;
			Temp->Ways[7] = NorthWestNeighbor;
			Temp->Ways[5] = SouthWestNeighbor;
			Temp->Ways[3] = SouthEastNeighbor;

			if (EastNeighbor) EastNeighbor->Ways[6] = Temp;
			if (WestNeighbor) WestNeighbor->Ways[2] = Temp;
			if (NorthNeighbor) NorthNeighbor->Ways[4] = Temp;
			if (SouthNeighbor) SouthNeighbor->Ways[0] = Temp;
			if (SouthEastNeighbor) SouthEastNeighbor->Ways[7] = Temp;
			if (SouthWestNeighbor) SouthWestNeighbor->Ways[1] = Temp;
			if (NorthEastNeighbor) NorthEastNeighbor->Ways[5] = Temp;
			if (NorthWestNeighbor) NorthWestNeighbor->Ways[3] = Temp;
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

			GridPoint->Ways[2] = EastNeighbor;
			GridPoint->Ways[6] = WestNeighbor;
			GridPoint->Ways[0] = NorthNeighbor;
			GridPoint->Ways[4] = SouthNeighbor;
			GridPoint->Ways[1] = NorthEastNeighbor;
			GridPoint->Ways[7] = NorthWestNeighbor;
			GridPoint->Ways[5] = SouthWestNeighbor;
			GridPoint->Ways[3] = SouthEastNeighbor;

			if (EastNeighbor) EastNeighbor->Ways[6] = GridPoint;
			if (WestNeighbor) WestNeighbor->Ways[2] = GridPoint;
			if (NorthNeighbor) NorthNeighbor->Ways[4] = GridPoint;
			if (SouthNeighbor) SouthNeighbor->Ways[0] = GridPoint;
			if (SouthEastNeighbor) SouthEastNeighbor->Ways[7] = GridPoint;
			if (SouthWestNeighbor) SouthWestNeighbor->Ways[1] = GridPoint;
			if (NorthEastNeighbor) NorthEastNeighbor->Ways[5] = GridPoint;
			if (NorthWestNeighbor) NorthWestNeighbor->Ways[3] = GridPoint;
		}
	}
}

void UFloorGrid::GetGridElement(int32 inX, int32 inY, int32& OutX, int32& OutY, int32& OutZ)
{
	if (inX >= 0 && inX < Rows && inY >= 0 && inY < Columns)
	{
		GridCell* Temp = Grid[inX][inY];
		OutX = Temp->X;
		OutY = Temp->Y;
		OutZ = Temp->Z;
	}

	else
	{
		OutX = -1;
		OutY = -1;
		OutZ = -1;
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


bool UFloorGrid::IsWalkable(const int32& inStartX, const int32& inStartY, const int32& inEndX, const int32& inEndY)
{
	GridCell* Start = GetGridElement(inStartX, inStartY);
	GridCell* Target = GetGridElement(inEndX, inEndY);
	if (!Start || !Target) return false;
	if (FMath::Abs(Start->Z - Target->Z) > 1) return false;

	for (int32 i = 1; i < 8; i += 2)
	{
		if (Start->Ways[i] == Target)
		{
			GridCell* Adjacent1 = Start->Ways[(i - 1 + 8) % 8]; 
			GridCell* Adjacent2 = Start->Ways[(i + 1) % 8];     

			if (Adjacent1 && Adjacent2)
			{
				if ((Adjacent1->Z - Start->Z) > 1 && (Adjacent2->Z - Start->Z) > 1)
				{
					return false;
				}
			}
		}
	}

	return true;
}


