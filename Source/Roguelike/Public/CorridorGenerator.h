// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "CorridorGenerator.generated.h"

UCLASS()
class ROGUELIKE_API ACorridorGenerator : public AActor
{
	GENERATED_BODY()

public:	
	virtual void BeginPlay() override;
	ACorridorGenerator();
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category ="Meshes")
	UStaticMesh* Floor;
	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMesh* Roof;
	UPROPERTY(EditAnywhere, Category = "Meshes")
	UStaticMesh* RoofSupport;

	
	UPROPERTY(EditAnywhere, Category = "Size Parameters")
		float Width = 2.f;

	UPROPERTY(EditAnywhere, Category = "Size Parameters", meta = (MakeEditWidget))
	FVector Controller = FVector(50.0f, .0f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Size Parameters|Roof")
	float RoofHeight = 200.f;
	UPROPERTY(EditAnywhere, Category = "Size Parameters|Roof")
	float RoofSupportSpacing = 100.f;
	UPROPERTY(EditAnywhere, Category = "Size Parameters|Roof")
		float RoofSupportEdgeOffset = 30.f;

	UPROPERTY(EditAnywhere, Category = "Debugging")
		TArray<UStaticMeshComponent*> Supports;
	

private:
	UPROPERTY(meta = (MakeEditWidget))
	FVector Root = FVector::ZeroVector;

	/// UProperties are set so I can move the Containers.
	UPROPERTY(EditAnywhere, Category = "Debugging")
	USceneComponent* ZRotator;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	USceneComponent* FloorScaler;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	USceneComponent* RoofScaler;

private:
	float GetPlanarMagnitude(FVector);

	void CreateSupports(int NumberOfSupports, float YPos, FString SideName);
	UStaticMeshComponent* InstantiateMesh(UStaticMesh* Mesh, USceneComponent* Parent, FName MeshName = TEXT("Mesh"));
};
