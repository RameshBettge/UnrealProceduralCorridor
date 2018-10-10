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
	// Sets default values for this actor's properties
	ACorridorGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Floor;
	UPROPERTY(EditAnywhere)
	UStaticMesh* Roof;

	//UPROPERTY(EditAnywhere)
	USceneComponent* Scaler;
	
	UPROPERTY(EditAnywhere)
		float Width = 2.f;

	/*UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> Floors;*/
	
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
	FVector Controller = FVector(50.0f, .0f, 0.f);
	UPROPERTY(meta = (MakeEditWidget))
	FVector Root = FVector::ZeroVector;

	UStaticMeshComponent* InstantiateMesh(UStaticMesh* Mesh, USceneComponent* Parent, FName MeshName = TEXT("Mesh"));
};
