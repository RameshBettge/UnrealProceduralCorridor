// Fill out your copyright notice in the Description page of Project Settings.

#include "CorridorGenerator.h"
#include "GenericPlatform/GenericPlatformMath.h"


// Sets default values
ACorridorGenerator::ACorridorGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACorridorGenerator::BeginPlay()
{
	Super::BeginPlay();

	CreateModular();
}

void ACorridorGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Check if values changed enough to justify instancing everything again.


	if (!bGenerateInTick) { return; }

	if (RowsContainer)
	{
		SetContainerRotation(RowsContainer, true);
		ClearRowContainer();
	}

	for (FCorridorElement E : Elements)
	{
		if (!E.Container) { continue; }

		if (E.bIsRow)
		{
			CreateElement(&E);
		}
		else
		{

			SetContainerRotation(E.Container);
			SetContainerScale(E);
		}
	}

	CreateFloorAndRoof();
}

float ACorridorGenerator::GetPlanarMagnitude(FVector V)
{
	V.Z = 0.f;
	return V.Size();
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

void ACorridorGenerator::CreateModular()
{
	if (RoofWidth == 0.f) { RoofWidth = FloorWidth; }

	CreateFloorAndRoof();

	for (int32 i = 0; i < Elements.Num(); i++)
	{
		CreateElement(&Elements[i]);
	}
}

void ACorridorGenerator::CreateFloorAndRoof() {
	FloorElement = FCorridorElement("Floor", Floor, 0.f, FloorWidth);
	RoofElement = FCorridorElement("Roof", Roof, RoofHeight, RoofWidth);
	CreateElement(&FloorElement);
	CreateElement(&RoofElement);
}

void ACorridorGenerator::CreateElement(FCorridorElement* E)
{
	if (!E->Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is unassigned!"), *E->ElementName);
		return;
	}

	USceneComponent* Container = nullptr;
	if (E->bIsRow)
	{
		if (E->Spacing == 0.f)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is a row and has to have a Spacing set."), *E->ElementName);
			return;
		}


		if (!RowsContainer)
		{
			RowsContainer = NewObject<USceneComponent>(this, TEXT("Rows Container"));
			Container = RowsContainer;
			E->Container = Container;
			RowsContainer->SetRelativeLocation(GetActorLocation());

			SetContainerRotation(E->Container, true);
		}
		else
		{
			Container = RowsContainer;
			E->Container = Container;
		}
		E->Container = Container;
		CreateModularRow(E);
	}
	else
	{
		FString Name = E->ElementName;
		auto automi = "String";
		Name += " Scaler";


		Container = NewObject<USceneComponent>(this, *Name);
		E->Container = Container;

		SetContainerScale(*E);

		SetContainerRotation(E->Container);

		InstantiateMesh(E->Mesh, Container, *E->ElementName);
		SetContainerPosition(*E);
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

void ACorridorGenerator::SetContainerRotation(USceneComponent* Container, bool bPlanar)
{
	FVector LookDirection = Controller.GetSafeNormal();
	FVector PlanarLookDirection = LookDirection;

	PlanarLookDirection.Z = 0.f;
	PlanarLookDirection = PlanarLookDirection.GetSafeNormal();

	if (bPlanar)
	{
		LookDirection.Z = 0.f;
		LookDirection = LookDirection.GetSafeNormal();
	}

	Container->SetRelativeRotation(LookDirection.ToOrientationQuat());
}

void ACorridorGenerator::CreateModularRow(FCorridorElement* Element)
{
	if (!Element->Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is unassigned!"), *Element->ElementName);
		return;
	}

	int NumberOfSupports = FGenericPlatformMath::FloorToInt(GetPlanarMagnitude(Controller) / Element->Spacing);
	InstantiateModularRow(Element, NumberOfSupports, 1, "R_");
	InstantiateModularRow(Element, NumberOfSupports, -1, "L_");
}

void ACorridorGenerator::InstantiateModularRow(FCorridorElement* E, int NumberOfSupports, int dir, FString SideName)
{
	/// TODO: calculate the YPos (/ OffsetMultiplicator) in a seperate function

	if (E->bUseAutoEdgeOffset)
	{
		E->SideOffset = AutoEdgeOffset;
		E->bEdgeOffset = true;
	}

	float YPos = E->bEdgeOffset ? FloorWidth * 0.5f - E->SideOffset : E->SideOffset;

	for (int i = 0; i < NumberOfSupports; i++)
	{

		FString SupportName = SideName;
		SupportName += E->ElementName;
		SupportName += FString::FromInt(i);
		UStaticMeshComponent* Support = InstantiateMesh(E->Mesh, E->Container, FName(*SupportName));


		float XPos = E->Spacing * (i + 0.5f);
		float XPercentage = XPos / GetPlanarMagnitude(Controller);

		float ZPos = XPercentage * Controller.Z;

		Support->SetRelativeLocation(FVector(XPos, YPos * dir, ZPos));

		if (E->YSize == 0.f)
		{
			Support->SetRelativeScale3D(FVector(1.f, 1.f, RoofHeight / 100.f));
		}
		else
		{
			Support->SetRelativeScale3D(FVector(1.f, 1.f, E->YSize / 100.f));
		}
	}
}

void ACorridorGenerator::UpdateSingleElement(FCorridorElement E, FVector LookDir)
{
	E.Container->SetRelativeRotation(LookDir.ToOrientationQuat());

}

void ACorridorGenerator::ClearRowContainer()
{
	TArray<USceneComponent*> Components;

	//RowContainer->GetParentComponents(Components);
	RowsContainer->GetChildrenComponents(true, Components);

	for (USceneComponent* Component : Components)
	{
		UMeshComponent* Mesh = Cast<UMeshComponent>(Component);
		if (Mesh)
		{
			Mesh->UnregisterComponent();
			Mesh->DestroyComponent();
		}
	}
}


