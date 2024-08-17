// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObjects.h"
#include "Kismet/GameplayStatics.h"
#include "PathFindingGameMode.h"
#include "FloorGrid.h"


ABaseObjects::ABaseObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	//SetLocationProperties(GetActorLocation());

}

void ABaseObjects::BeginPlay()
{
	Super::BeginPlay();
	
	//SetObjectSizeXandY();
	//APathFindingGameMode* GM = Cast<APathFindingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (GM)
	//{
	//	GM->OnGridStartDelegate.AddDynamic(this, &ABaseObjects::WhenGridCreated);
	//}
}


void ABaseObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

