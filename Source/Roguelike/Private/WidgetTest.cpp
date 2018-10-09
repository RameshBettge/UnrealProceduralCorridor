// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetTest.h"
#include "DrawDebugHelpers.h"


// Sets default values
AWidgetTest::AWidgetTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AWidgetTest::ShouldTickIfViewportsOnly() const
{
	return TickInEditor;
}

// Called when the game starts or when spawned
void AWidgetTest::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWidgetTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetTransform().GetLocation();
	DrawDebugLine(GetWorld(), Location, Location + MyVector, FColor::Magenta, false, .0f, (uint8)'\000', 10.f);
}

