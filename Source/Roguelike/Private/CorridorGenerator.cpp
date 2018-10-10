// Fill out your copyright notice in the Description page of Project Settings.

#include "CorridorGenerator.h"
#include "GenericPlatform/GenericPlatformMath.h"


// Sets default values
ACorridorGenerator::ACorridorGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = FloorComponent;
}

// Called when the game starts or when spawned
void ACorridorGenerator::BeginPlay()
{
	Super::BeginPlay();

	FloorScaler = NewObject<USceneComponent>(this, TEXT("FloorScaler"));
	RoofScaler = NewObject<USceneComponent>(this, TEXT("RoofScaler"));
	ZRotator = NewObject<USceneComponent>(this, TEXT("ZRotator"));
	// Set Empty's location to local zero.
	FloorScaler->SetRelativeLocation(GetActorLocation());
	RoofScaler->SetRelativeLocation(GetActorLocation() + FVector(0.f, 0.f, RoofHeight));
	ZRotator->SetRelativeLocation(GetActorLocation());

	UStaticMeshComponent* FloorComponent = InstantiateMesh(Floor, FloorScaler, FName("Floor"));
	UStaticMeshComponent* RoofComponent = InstantiateMesh(Roof, RoofScaler, FName("Roof"));

	int NumberOfSupports = FGenericPlatformMath::FloorToInt(GetPlanarMagnitude(Controller) / RoofSupportSpacing);
	CreateSupports(NumberOfSupports, (Width * 0.5f) - RoofSupportEdgeOffset, "R_");
	CreateSupports(NumberOfSupports, (Width * -0.5f) + RoofSupportEdgeOffset, "L_");
	//UE_LOG(LogTemp, Warning, TEXT("Setting Mesh!"));
}

float ACorridorGenerator::GetPlanarMagnitude(FVector V)
{
	V.Z = 0.f;
	return V.Size();
}

void ACorridorGenerator::CreateSupports(int NumberOfSupports, float YPos, FString SideName)
{
	if (!RoofSupport)
	{
		UE_LOG(LogTemp, Error, TEXT("RoofSupport is unassigned!"));
		return;
	}

	for (int i = 0; i < NumberOfSupports; i++)
	{

		FString SupportName = SideName;
		SupportName += "RoofSupport";
		SupportName += FString::FromInt(i);
		UStaticMeshComponent* Support = InstantiateMesh(RoofSupport, ZRotator, FName(*SupportName));


		float XPos = RoofSupportSpacing * (i + 0.5f);
		float XPercentage = XPos / GetPlanarMagnitude(Controller);

		float ZPos = XPercentage * abs(Controller.Z);

		/// Could still be used to debug MagicSupportOffset
		//UE_LOG(LogTemp, Warning, TEXT("XPos = %f, XPercentage = %f, ZMax = %f, ZPos = %f"), XPos, XPercentage, Controller.Z, ZPos);

		Support->SetRelativeLocation(FVector(XPos, YPos * 100.f, ZPos + MagicSupportOffset));
		Support->SetRelativeScale3D(FVector(1.f, 1.f, RoofHeight / 100.f));
		Supports.Add(Support);
	}
}

UStaticMeshComponent * ACorridorGenerator::InstantiateMesh(UStaticMesh * Mesh, USceneComponent* Parent, FName MeshName)
{
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshPointer to %s is a nullptr!"), *MeshName.ToString());
		return nullptr;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Instantiating %s"), *MeshName.ToString());


	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(Parent, MeshName);

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	MeshComponent->AttachToComponent(Parent, Rules);

	MeshComponent->RegisterComponentWithWorld(GetWorld());
	MeshComponent->SetStaticMesh(Mesh);


	return MeshComponent;
}



// Called every frame
void ACorridorGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FloorScaler->SetRelativeScale3D(FVector(Controller.Size() / 100.f, Width, 1.f));
	RoofScaler->SetRelativeScale3D(FVector(Controller.Size() / 100.f, Width, 1.f));
	FVector LookDirection = Controller.GetSafeNormal();
	FVector PlanarLookDirection = LookDirection;
	PlanarLookDirection.Z = 0.f;
	PlanarLookDirection = PlanarLookDirection.GetSafeNormal();

	FloorScaler->SetRelativeRotation(LookDirection.ToOrientationQuat());
	RoofScaler->SetRelativeRotation(LookDirection.ToOrientationQuat());
	ZRotator->SetRelativeRotation(PlanarLookDirection.ToOrientationQuat());
}



