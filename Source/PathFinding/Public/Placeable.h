// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Placeable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHFINDING_API UPlaceable : public USceneComponent
{
	GENERATED_BODY()

public:	

	UPlaceable();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetObjectLocation(const FVector& NewLocation, float DistanceBeetweenGrids);

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void WhenGridCreated(class UFloorGrid* GridRef);


private:

	TMap<class UStaticMeshComponent*, FVector> SizeVectors;

	TArray<UStaticMeshComponent*> Meshes;

	void SetLocationProperties(const FVector& NewLocation, UStaticMeshComponent* MeshRef);

	void SetSizeVectors(float DistanceBetweenGrids);

		
};
