// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PathController.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API APathController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APathController();


protected: 

	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	class UPathFinder* PathFinderRef;

};
