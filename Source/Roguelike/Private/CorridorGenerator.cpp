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

	Scaler = NewObject<USceneComponent>(this, TEXT("Scaler"));
	// Set Scaler's location to local zero.
	Scaler->SetRelativeLocation(GetActorLocation());

	UStaticMeshComponent* FloorComponent = InstantiateMesh(Floor, Scaler, FName("Floor"));
	UStaticMeshComponent* RoofComponent = InstantiateMesh(Roof, Scaler, FName("Roof"));
	
	
	//UE_LOG(LogTemp, Warning, TEXT("Setting Mesh!"));
	//Floors.Add(FloorComponent);
}



// Called every frame
void ACorridorGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Scaler->SetRelativeScale3D(FVector(Controller.Size() / 100.f, Width, 1.f));
	FVector LookDirection = Controller.GetSafeNormal();
	Scaler->SetRelativeRotation(LookDirection.ToOrientationQuat());
	//Cube->SetActorRotation(LookDirection.ToOrientationQuat());

}

UStaticMeshComponent * ACorridorGenerator::InstantiateMesh(UStaticMesh * Mesh, USceneComponent* Parent, FName MeshName)
{
	if(!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshPointer is a nullptr!"));
		return nullptr;
	}

	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(Parent, MeshName);

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	MeshComponent->AttachToComponent(Parent, Rules);
	MeshComponent->RegisterComponentWithWorld(GetWorld());
	MeshComponent->SetStaticMesh(Mesh);
	return MeshComponent;
}


