// Fill out your copyright notice in the Description page of Project Settings.


#include "PathController.h"
#include "PathFinder.h"

APathController::APathController()
{
}

void APathController::BeginPlay()
{
	Super::BeginPlay();

	PathFinderRef = NewObject<UPathFinder>(this, UPathFinder::StaticClass());
}
