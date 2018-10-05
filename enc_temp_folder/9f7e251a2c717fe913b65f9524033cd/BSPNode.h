// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "TileType.h"
#include "BSPNode.generated.h"

/**
 *
 */
UCLASS()
class ROGUELIKE_API UBSPNode : public UObject
{
	GENERATED_BODY()

public:
	const int ChildCount = 2;

	UBSPNode* Parent = nullptr;
	UBSPNode* Root = nullptr;
	TArray<UBSPNode> Children;
	UBSPNode* Sibling = nullptr;

	bool bDoSplitVertically = false;
	bool bIsResultOfVerticalSplit = false;
	int MaxLevel = 0;

	

};

