// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PathFindingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API APathFindingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APathFindingGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	class UFloorGrid* GridRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<UFloorGrid> GridClass;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	//class UPathFinder* PathFinderRef;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PrintGrid();

protected:
	virtual void BeginPlay() override;
	
};
