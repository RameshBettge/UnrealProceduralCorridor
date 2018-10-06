// Fill out your copyright notice in the Description page of Project Settings.

#include "BSPTree.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UBSPTree::UBSPTree()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBSPTree::BeginPlay()
{
	Super::BeginPlay();


	// TEST
	Root = NewObject<UBSPNode>();
	Root->TestString = "Hello, I am the root.";
	AllNodes.Emplace(Root);

	for (int i = 0; i < 5; i++)
	{
		UBSPNode* Node = NewObject<UBSPNode>();
		Node->TestString = TEXT("I am Node number ") + FString::FromInt(i);
		Node->PrintTest();
		AllNodes.Emplace(Node);
	}

	for (UBSPNode* Node : AllNodes)
	{
		Node->PrintTest();
	}

	/*for (int i = 0; i < AllNodes.Num(); i++)
	{
		AllNodes[i].PrintTest();
	}*/
	// END TEST

	// ...

}


// Called every frame
void UBSPTree::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

