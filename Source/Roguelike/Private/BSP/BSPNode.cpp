// Fill out your copyright notice in the Description page of Project Settings.

#include "BSPNode.h"

UBSPNode::UBSPNode()
{
	//UE_LOG(LogTemp, Warning, TEXT("BSPNode constructer called."));
}

void UBSPNode::PrintTest() 
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TestString);
}

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

	UE_LOG(LogTemp, Warning, TEXT("BSPNode properly created via UBSPNode::CreateNode"))
	/// TODO: Create rest of this 'constructor'

	return Node;
}




