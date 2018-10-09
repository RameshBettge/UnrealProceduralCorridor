// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetTest.generated.h"

// Displays a widget which can be used to set a FVector. Can Display a DebugLine in Editor.
UCLASS()
class ROGUELIKE_API AWidgetTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWidgetTest();

	// If this returns true Tick() will run in editor.
	virtual bool ShouldTickIfViewportsOnly() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
		FVector MyVector = FVector(50.0f, .0f, 0.f);

	// Only works if Realtime in ViewportOptions is enabled
	UPROPERTY(EditAnywhere)
	bool TickInEditor = true;
};


