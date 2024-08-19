// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FloorGrid.generated.h"

/**
 * 
 */


struct GridCell
{

public:
	int32 X;
	int32 Y;
	int32 Z = 0;

	bool bIsWalkable = true;

	GridCell(int32 inX, int32 inY, int32 inZ) {
		X = inX;
		Y = inY;
		Z = inZ;
	}
	
	/*
	North = 0;
	NorthEast = 1;
	East = 2;
	SouthEast = 3;
	South = 4;
	SouthWest = 5;
	West = 6;
	NorthWest = 7;
	*/
	GridCell* Ways[8];

};


UCLASS(Blueprintable, BlueprintType)
class PATHFINDING_API UFloorGrid : public UObject
{
	GENERATED_BODY()
	
public:
	UFloorGrid();

	void InitialGrid();

	inline int32 GetRows() { return Rows; }
	inline int32 GetColumns() { return Columns; }
	inline int32 GetDistanceBeetweenCells() { return DiffBeetweenGridCells; }

public:
	UFUNCTION(BlueprintCallable)
	FVector GetGridWorldPosition(int32 inX, int32 inY);


	/*
	@param OutX - if function fail outx will return as -1
	@param Outy - if function fail outy will return as -1*/
	UFUNCTION(BlueprintCallable)
	void GetCellCordinates(FVector ActiveLocation, int32& OutX, int32& OutY);

	UFUNCTION(BlueprintCallable)
	void SetGridElement(int32 inX, int32 inY, int32 inZ);

	void SetGridElement(int32 inX, int32 inY, GridCell* GridPoint);
	
	UFUNCTION(BlueprintCallable)
	void GetGridElement(int32 inX, int32 inY, int32& OutX, int32& OutY, int32& OutZ);

	GridCell* GetGridElement(int32 inX, int32 inY);


	UFUNCTION(BlueprintCallable)
	bool IsWalkable(const int32& inStartX, const int32& inStartY, const int32& inEndX, const int32& inEndY);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	int32 Rows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	int32 Columns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	float DiffBeetweenGridCells;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	FVector InitialPoint;

	TArray<TArray<GridCell*>> Grid;

private:

	void CreateGrid();

	void setNeighbords();
};
