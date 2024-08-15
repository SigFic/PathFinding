// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingGameMode.h"
#include "FloorGrid.h"
//#include "PathFinder.h"

APathFindingGameMode::APathFindingGameMode()
{
}

void APathFindingGameMode::BeginPlay()
{
	Super::BeginPlay();
	//PathFinderRef = NewObject<UPathFinder>(this, UPathFinder::StaticClass());
	GridRef = NewObject<UFloorGrid>(this, GridClass);
	if (GridRef)
	{
		GridRef->InitialGrid();
		OnGridStartDelegate.Broadcast(GridRef);
		PrintGrid();
	}


}
