// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "CorridorGenerator.generated.h"

USTRUCT()
struct FCorridorElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ElementName;

	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;

	// Z-Scale for Rows.
	UPROPERTY(EditAnywhere)
	float Height;

	// If the side offset should be calculated from the edge instead of from the middle.
	UPROPERTY(EditAnywhere)
	bool bUseAutoEdgeOffset = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseAutoEdgeOffset"))
	bool bEdgeOffset = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseAutoEdgeOffset"))
	float SideOffset = 0.f;

	UPROPERTY(EditAnywhere)
	bool bIsRow = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bIsRow"))
	float width;

	// TODO: Make sure this can never be 0.
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsRow"))
	float Spacing;

	//If this is set to 0 the RoofHeight will be used.
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsRow"))
		float YSize = 0.f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Container = nullptr;

	FCorridorElement(FString Name, UStaticMesh* Mesh, float Height, float Width)
	{
		this->ElementName = Name, this->Mesh = Mesh, this->Height = Height, this->width = Width;
	}
	FCorridorElement() {};
};


UCLASS()
class ROGUELIKE_API ACorridorGenerator : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	ACorridorGenerator();
	virtual void Tick(float DeltaTime) override;
	//virtual void EndPlay() override;
	virtual void BeginDestroy() override;



public:
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMesh* Floor;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMesh* Roof;

	UPROPERTY(EditAnywhere, Category = "Size Parameters", meta = (MakeEditWidget))
		FVector Controller = FVector(50.0f, .0f, 0.f);

	// Deletes every object from the scene but does not clear any settings.
	UPROPERTY(EditAnywhere, Category = "Debugging|Events")
	bool UpdateSettings;
	UPROPERTY(EditAnywhere, Category = "Debugging|Events")
	bool ClearAll;
	UPROPERTY(EditAnywhere, Category = "Debugging|Ticking")
	bool bGenerateInTick;
	UPROPERTY(EditAnywhere, Category = "Debugging|Ticking", meta = (EditCondition = "bGenerateInTick"))
	bool bTickInEditor;

	virtual bool ShouldTickIfViewportsOnly() const override;



private:
	UPROPERTY(meta = (MakeEditWidget))
		FVector Root = FVector::ZeroVector;

private:
	float GetPlanarMagnitude(FVector);

	UStaticMeshComponent* InstantiateMesh(UStaticMesh* Mesh, USceneComponent* Parent, FName MeshName = TEXT("Mesh"));

	UPROPERTY(EditAnywhere, Category = "Modular")
	float FloorWidth;
	UPROPERTY(EditAnywhere, Category = "Modular")
	float RoofHeight;
	// If this is set to 0 the floor width will be used.
	UPROPERTY(EditAnywhere, Category = "Modular")
	float RoofWidth;

	UPROPERTY(EditAnywhere, Category = "Modular")
		float AutoEdgeOffset= 50.f;

	UPROPERTY(EditAnywhere, Category = "Modular")
	TArray<FCorridorElement> Elements;

	FCorridorElement FloorElement;
	FCorridorElement RoofElement;

	TArray<USceneComponent*> Containers;
	USceneComponent* RowsContainer = nullptr;

	void CreateModular(bool bDisplayErrors = false);
	void CreateElement(FCorridorElement* E, bool bDisplayErrors = false);
	void SetContainerPosition(FCorridorElement Element);
	void SetContainerScale(FCorridorElement Element);
	void SetContainerRotation(USceneComponent* Container, bool planar = false);

	void CreateModularRow(FCorridorElement* Element);
	void InstantiateModularRow(FCorridorElement* E, int NumberOfSupports, int dir, FString SideName);

	void ClearContainer(USceneComponent* Container, bool bDelete = false);

	void CreateFloorAndRoof();
	void UpdateCorridor();

	void ClearEverything();

};
