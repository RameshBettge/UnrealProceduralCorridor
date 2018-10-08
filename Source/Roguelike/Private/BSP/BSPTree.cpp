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


	/// TEST


	/*FStructNode TempNode = FStructNode(FStructNode(nullptr, 98.f));
	StructNode = &TempNode;*/
	StructNode = &ConstStructNode;
	//AllStructs.Emplace(FStructNode(FStructNode(nullptr,60.f)));

	//for (int i = 0; i < 5; i++)
	//{
	//	//UBSPNode* Node = NewObject<UBSPNode>();
	//	FIntRect Rect;
	//	UBSPNode* Node = UBSPNode::CreateNode(nullptr, Rect, false);
	//	AllNodes.Emplace(Node);
	//}

	//for (UBSPNode* Node : AllNodes)
	//{
	//	Node->DO STH;
	//}

	/// END TEST

}


// Called every frame
void UBSPTree::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//for (FStructNode Node : AllStructs)
	//{
	//	//Node.PrintTest();
	//}
	ConstStructNode.TestFloat += 10.f;
	UE_LOG(LogTemp, Warning, TEXT("Struct's float = %f"), StructNode->TestFloat)
	UE_LOG(LogTemp, Warning, TEXT("ConstStruct's float = %f"), ConstStructNode.TestFloat)


		//UE_LOG(LogTemp, Warning, TEXT("Structs' float = %f"), ConstStructNode.TestFloat)

	// ...
}

