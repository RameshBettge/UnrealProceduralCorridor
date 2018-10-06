// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BSPNode.generated.h"

/**
 *
 */

UENUM()
enum class ETileType : uint8
{
	Invalid, 
	Floor, 
	Wall,
	Corridor
};

UCLASS()
class ROGUELIKE_API UBSPNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FString TestString;
	void PrintTest();

public:
	const int ChildCount = 2;

	UBSPNode* Parent = nullptr;
	UBSPNode* Root = nullptr;
	TArray<UBSPNode> Children;
	UBSPNode* Sibling = nullptr;

	bool bDoSplitVertically = false;
	bool bIsResultOfVerticalSplit = false;
	int MaxLevel = 0;

	//TODO: Add Unity Specific Rects: rect, room, corridor

	ETileType TileType;

	int GetLevel();

private:
	int Level;

};

