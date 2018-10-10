// Fill out your copyright notice in the Description page of Project Settings.

#include "CorridorGenerator.h"


// Sets default values
ACorridorGenerator::ACorridorGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = FloorComponent;
}

// Called when the game starts or when spawned
void ACorridorGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (!Floor) { return; }
	FloorComponent = NewObject<UStaticMeshComponent>(this);
	FloorComponent->RegisterComponentWithWorld(GetWorld());
	FloorComponent->SetRelativeLocation(GetActorLocation());
	FloorComponent->SetStaticMesh(Floor);
	//UE_LOG(LogTemp, Warning, TEXT("Setting Mesh!"));
	//Floors.Add(FloorComponent);
}

// Called every frame
void ACorridorGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

