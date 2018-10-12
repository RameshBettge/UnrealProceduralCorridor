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

	//FloorScaler = NewObject<USceneComponent>(this, TEXT("FloorScaler"));
	//RoofScaler = NewObject<USceneComponent>(this, TEXT("RoofScaler"));
	//ZRotator = NewObject<USceneComponent>(this, TEXT("ZRotator"));
	//// Set Empty's location to local zero.
	//FloorScaler->SetRelativeLocation(GetActorLocation());
	//RoofScaler->SetRelativeLocation(GetActorLocation() + FVector(0.f, 0.f, RoofHeight));
	//ZRotator->SetRelativeLocation(GetActorLocation());

	//UStaticMeshComponent* FloorComponent = InstantiateMesh(Floor, FloorScaler, FName("Floor"));
	//UStaticMeshComponent* RoofComponent = InstantiateMesh(Roof, RoofScaler, FName("Roof"));
	////UE_LOG(LogTemp, Warning, TEXT("Setting Mesh!"));

	//CreateSupports();


	CreateModular();
}

float ACorridorGenerator::GetPlanarMagnitude(FVector V)
{
	V.Z = 0.f;
	return V.Size();
}



void ACorridorGenerator::CreateSupports()
{
	/*int NumberOfSupports = FGenericPlatformMath::FloorToInt(GetPlanarMagnitude(Controller) / RoofSupportSpacing);
	InstantiateSupportRow(NumberOfSupports, (Width * 0.5f) - RoofSupportEdgeOffset, "R_");
	InstantiateSupportRow(NumberOfSupports, (Width * -0.5f) + RoofSupportEdgeOffset, "L_");*/
}

void ACorridorGenerator::ClearSupports()
{
	//for (UMeshComponent* Component : Supports)
	//{
	//	Component->UnregisterComponent();
	//	Component->DestroyComponent();
	//}
	//Supports.Empty();
}

void ACorridorGenerator::InstantiateSupportRow(int NumberOfSupports, float YPos, FString SideName)
{
	//if (!RoofSupport)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("RoofSupport is unassigned!"));
	//	return;
	//}

	//for (int i = 0; i < NumberOfSupports; i++)
	//{

	//	FString SupportName = SideName;
	//	SupportName += "RoofSupport";
	//	SupportName += FString::FromInt(i);
	//	UStaticMeshComponent* Support = InstantiateMesh(RoofSupport, ZRotator, FName(*SupportName));


	//	float XPos = RoofSupportSpacing * (i + 0.5f);
	//	float XPercentage = XPos / GetPlanarMagnitude(Controller);

	//	float ZPos = XPercentage * Controller.Z;

	//	Support->SetRelativeLocation(FVector(XPos, YPos * 100.f, ZPos));
	//	Support->SetRelativeScale3D(FVector(1.f, 1.f, RoofHeight / 100.f));
	//	Supports.Add(Support);
	//}
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

	/// TODO: This function has to happen at least once but be only repeated if bGenerateInTick is true --> Extract function
	//FloorScaler->SetRelativeScale3D(FVector(Controller.Size() / 100.f, Width, 1.f));
	//RoofScaler->SetRelativeScale3D(FVector(Controller.Size() / 100.f, Width, 1.f));
	//FVector LookDirection = Controller.GetSafeNormal();
	//FVector PlanarLookDirection = LookDirection;
	//PlanarLookDirection.Z = 0.f;
	//PlanarLookDirection = PlanarLookDirection.GetSafeNormal();

	//FloorScaler->SetRelativeRotation(LookDirection.ToOrientationQuat());
	//RoofScaler->SetRelativeRotation(LookDirection.ToOrientationQuat());
	//ZRotator->SetRelativeRotation(PlanarLookDirection.ToOrientationQuat());

	//if(bGenerateInTick)
	//{
	//	RoofScaler->SetRelativeLocation(GetActorLocation() + FVector(0.f, 0.f, RoofHeight));
	//	ClearSupports();
	//	CreateSupports();
	//}
}


void ACorridorGenerator::CreateModular()
{
	if (RoofWidth == 0.f) { RoofWidth = FloorWidth; }

	FloorElement = FCorridorElement("Floor", Floor, 0.f, FloorWidth);
	RoofElement = FCorridorElement("Roof", Roof, RoofHeight, RoofWidth);
	CreateElement(FloorElement);
	CreateElement(RoofElement);

	for (FCorridorElement E : Elements)
	{
		CreateElement(E);
	}
}

void ACorridorGenerator::CreateElement(FCorridorElement E)
{
	USceneComponent* Container = nullptr;
	if (E.bIsRow)
	{

		/*if (!RowsContainer)
		{*/
			/// TODO: Each time an Element is a row there has to be a new rowsContainer created or the application will crash. Find out why!
			RowsContainer = NewObject<USceneComponent>(this, TEXT("Rows Container"));
			Container = RowsContainer;
			E.Container = Container;
			// TODO: Check if I shold use SetContainerLocation
			RowsContainer->SetRelativeLocation(GetActorLocation());

			SetContainerRotation(E, true);
		//}

		E.Container = Container;
		CreateModularRow(E);
	}
	else
	{
		FString Name = E.ElementName;
		auto automi = "String";
		Name += " Scaler";


		Container = NewObject<USceneComponent>(this, *Name);
		E.Container = Container;

		SetContainerScale(E);

		SetContainerRotation(E);
		InstantiateMesh(E.Mesh, Container, *E.ElementName);
		SetContainerPosition(E);
	}



}

void ACorridorGenerator::SetContainerPosition(FCorridorElement Element)
{
	if (Element.bUseAutoEdgeOffset)
	{
		Element.SideOffset = AutoEdgeOffset;
		Element.bEdgeOffset = true;
	}

	float OffsetMultiplicator = Element.bEdgeOffset ? FloorWidth * 0.5f - Element.SideOffset : Element.SideOffset;
	FVector Offset = OffsetMultiplicator * FVector::RightVector;

	if (!Element.bIsRow) { Offset += FVector::UpVector * Element.Height; }
	Element.Container->SetRelativeLocation(GetActorLocation() + Offset);


}

void ACorridorGenerator::SetContainerScale(FCorridorElement Element)
{
	Element.Container->SetRelativeScale3D(FVector(Controller.Size() * 0.01f, Element.width * 0.01f, 1.f));
}

void ACorridorGenerator::SetContainerRotation(FCorridorElement Element, bool bPlanar)
{
	FVector LookDirection = Controller.GetSafeNormal();
	FVector PlanarLookDirection = LookDirection;
	PlanarLookDirection.Z = 0.f;
	PlanarLookDirection = PlanarLookDirection.GetSafeNormal();

	if(bPlanar)
	{
		LookDirection.Z = 0.f;
		LookDirection = LookDirection.GetSafeNormal();
	}

	Element.Container->SetRelativeRotation(LookDirection.ToOrientationQuat());
}

void ACorridorGenerator::CreateModularRow(FCorridorElement Element)
{

	int NumberOfSupports = FGenericPlatformMath::FloorToInt(GetPlanarMagnitude(Controller) / Element.Spacing);
	InstantiateModularRow(Element, NumberOfSupports, 1, "R_");
	InstantiateModularRow(Element, NumberOfSupports, -1, "L_");
}

void ACorridorGenerator::InstantiateModularRow(FCorridorElement E, int NumberOfSupports, int dir, FString SideName)
{
	/// TODO: calculate the YPos (/ OffsetMultiplicator) in a seperate function



	if (E.bUseAutoEdgeOffset)
	{
		E.SideOffset = AutoEdgeOffset;
		E.bEdgeOffset = true;
	}

	float YPos = E.bEdgeOffset ? FloorWidth * 0.5f - E.SideOffset : E.SideOffset;

	if (!E.Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is unassigned!"), *E.ElementName);
		return;
	}

	for (int i = 0; i < NumberOfSupports; i++)
	{
		FString SupportName = SideName;
		SupportName += E.ElementName;
		SupportName += FString::FromInt(i);
		UStaticMeshComponent* Support = InstantiateMesh(E.Mesh, E.Container, FName(*SupportName));


		float XPos = E.Spacing * (i + 0.5f);
		float XPercentage = XPos / GetPlanarMagnitude(Controller);

		float ZPos = XPercentage * Controller.Z;

		Support->SetRelativeLocation(FVector(XPos, YPos * dir, ZPos));

	/*	if (E.YSize == 0.f)
		{
			Support->SetRelativeScale3D(FVector(1.f, 1.f, E.YSize / 100.f));
		}
		else
		{*/
			Support->SetRelativeScale3D(FVector(1.f, 1.f, RoofHeight / 100.f));
		//}
	}
}


