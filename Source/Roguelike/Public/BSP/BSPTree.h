// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSPNode.h"
#include "GameFramework/Actor.h"
#include "BSPTree.generated.h"

UENUM()
enum class EFarthest : uint8
{
	Top,
	Right,
	Bottom,
	Left
};

USTRUCT()
struct FStructNode
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		float TestFloat = 10.f;

		//DONT USE! Always use constructer with parameters.
	FStructNode()
	{
		//Will also be called when a FStructNode-MemberVariable is unassigned
		UE_LOG(LogTemp, Error, TEXT("FStructNode's constructer was called without Parameters. This is a potential source for errors. Use Custom Constructor instead!"));
	}

	FStructNode(FStructNode* Node, float TestFloat)
	{
		this->TestFloat = TestFloat;
		//FString Name = Tree->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("FStructNode's Custom Constructer was called."));
	}

	void PrintTest()
	{
		UE_LOG(LogTemp, Warning, TEXT("NodeTest"));
	}
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKE_API UBSPTree : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBSPTree();
	UPROPERTY(VisibleAnywhere)
		UBSPNode* Root;
	UPROPERTY(VisibleAnywhere)
	TArray<FStructNode> AllStructs;
	UPROPERTY(VisibleAnywhere)
	TArray<UBSPNode*> AllNodes;
	TArray<TArray<UBSPNode*>> NodeLists;
	int MinNodeWidth;
	int MinNodeHeigth;
	int NumberOfCorridors;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// TODO: Make sure UBSPNodes can only be used if properly initialized.
	void Split(UBSPNode* Node);
	void CreateNodeLists();
	void GenerateRooms();
	void GenerateCorridors();
	void GenerateCorridor();
	UBSPNode FindFarthestNode(EFarthest Farthest, TArray<UBSPNode*> candidates);
	TArray<UBSPNode> FindOverlapping(UBSPNode* Node, TArray<UBSPNode*> candidates, bool xDim);

	//TODO: Add Initialize function. 

	//TODO: Add float halfOverlap() which takes Unity specific Rects as arguments.

	/// For Testing
	FStructNode* StructNode = nullptr;
	FStructNode ConstStructNode = FStructNode(nullptr, 6.9f);
};
