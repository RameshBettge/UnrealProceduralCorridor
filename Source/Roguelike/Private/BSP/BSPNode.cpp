// Fill out your copyright notice in the Description page of Project Settings.

#include "BSPNode.h"
#include "Engine/World.h"
#include "TimerManager.h"

int UBSPNode::GetLevel()
{
	return Level;
}

UBSPNode* UBSPNode::CreateNode(UBSPNode * Parent, FIntRect Rect, bool bDoSplitVertically)
{
	UBSPNode* Node = NewObject<UBSPNode>();
	Node->Parent = Parent;
	Node->Rect = Rect;
	Node->bDoSplitVertically = bDoSplitVertically;
	Node->bIsResultOfVerticalSplit = !bDoSplitVertically;

	Node->bIsInitialized = true;

		/// TODO: Create rest of this 'constructor'

		return Node;
}




