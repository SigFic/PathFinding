// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObjects.generated.h"

UCLASS()
class PATHFINDING_API ABaseObjects : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseObjects();

protected:

	virtual void BeginPlay() override;



public:	

	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObjectMesh;

};
