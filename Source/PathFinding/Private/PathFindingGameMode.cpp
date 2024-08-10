// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingGameMode.h"
#include "FloorGrid.h"

APathFindingGameMode::APathFindingGameMode()
{
}

void APathFindingGameMode::BeginPlay()
{
	GridRef = NewObject<UFloorGrid>(this, UFloorGrid::StaticClass());
	if (GridRef)
	{
		GridRef->InitialGrid();
		PrintGrid();
	}
}
