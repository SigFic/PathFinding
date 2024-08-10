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

	bool bIsWalkable;

	GridCell(int32 inX, int32 inY, bool binIsWalkable) {
		X = inX;
		Y = inY;
		bIsWalkable = binIsWalkable;
	}

	GridCell* North = nullptr;
	GridCell* South = nullptr;
	GridCell* East = nullptr;
	GridCell* West = nullptr;
};


UCLASS(Blueprintable, BlueprintType)
class PATHFINDING_API UFloorGrid : public UObject
{
	GENERATED_BODY()
	
public:
	UFloorGrid();

	void InitialGrid();

public:
	UFUNCTION(BlueprintCallable)
	FVector GetGridWorldPosition(int32 inX, int32 inY);

	UFUNCTION(BlueprintCallable)
	void GetCellCordinates(FVector ActiveLocation, int32& OutX, int32& OutY);

	UFUNCTION(BlueprintCallable)
	void SetGridElement(int32 inX, int32 inY, bool bIsWalkable);

	void SetGridElement(int32 inX, int32 inY, GridCell* GridPoint);
	
	UFUNCTION(BlueprintCallable)
	void GetGridElement(int32 inX, int32 inY, int32& OutX, int32& OutY, bool& bIsWalkable);

	GridCell* GetGridElement(int32 inX, int32 inY);

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
