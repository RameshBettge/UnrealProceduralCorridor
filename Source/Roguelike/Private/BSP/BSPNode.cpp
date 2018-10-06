// Fill out your copyright notice in the Description page of Project Settings.

#include "BSPNode.h"

void UBSPNode::PrintTest() 
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TestString);
}

int UBSPNode::GetLevel()
{
	return Level;
}
