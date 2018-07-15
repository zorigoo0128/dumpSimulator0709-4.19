// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Creating a standard root object.
AMyActor::AMyActor()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	bReplicates = true;
	bNetLoadOnClient = true;
	bReplicateMovement = true;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
}


// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void AMyActor::PostActorCreated()
{
	Super::PostActorCreated();
	CreateTriangle();
}

// This is called when actor is already in level and map is opened
void AMyActor::PostLoad()
{
	Super::PostLoad();
	CreateTriangle();
}

FVector AMyActor::calculateNormal(int i) {
	
	float heightL = 0, heightR = 0, heightU = 0, heightD = 0;
	if (i - 1 >= 0) 
		heightL = vertices[i - 1].Z;
	if (i + 1 < vertices.Num()) 
		heightR = vertices[i + 1].Z;
	if (i - n >= 0) 
		heightU = vertices[i - n].Z;
	if (i + n <vertices.Num()) 
		heightD = vertices[i +n].Z;

	FVector  normal = FVector(heightL - heightR, 2.0f, heightD - heightU);	
	float magnitude = FMath::Sqrt(normal.X*normal.X+ normal.Y*normal.Y+ normal.Z*normal.Z);
	return normal/magnitude;

}

void AMyActor::CreateTriangle()
{

	for (int i=0; i<n; i++)
		for (int j = 0; j < n; j++) {
			vertices.Add(FVector(j*gridSize, i*gridSize, 0));

			UV0.Add(FVector2D((float) j/(n-1), (float)i / (n - 1)));
			
		}

	for (int i = 0; i<n; i++)
		for (int j = 0; j < n; j++) {			
			normals.Add(calculateNormal(i*n+j));
		}


	for (int i = 0; i<n-1; i++)
		for (int j = 0; j < n-1; j++) {
			int v0 = i*n + j;
			int v1 = v0 + 1;
			int v2 = v0 + n;
			int v3 = v2 + 1;

			Triangles.Add(v2);
			Triangles.Add(v1);
			Triangles.Add(v0);

			Triangles.Add(v1);
			Triangles.Add(v2);
			Triangles.Add(v3);
		}	

	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
	mesh->ContainsPhysicsTriMeshData(true);
	mesh->SetIsReplicated(true);
}

void AMyActor::updateMesh() {
	mesh->UpdateMeshSection_LinearColor(0, vertices,normals, UV0, vertexColors, tangents);

}

void AMyActor::setOffset(FVector location) {
	this->offset = location;
}

bool AMyActor::recieveForkLocation(FVector forkLocation){
	int j = (forkLocation.X - offset.X)/ gridSize;
	int i = (forkLocation.Y - offset.Y) / gridSize;
	float z = forkLocation.Z - offset.Z;
	int k = i*n + j;
	if(k>=0&&k<vertices.Num())
	if (z < vertices[i*n + j].Z) {
		
		for (int x = -1; x < 3; x++)
			for (int y = -1; y < 3; y++) {
				int t = (i + y)*n + (j + x);
				if (t >= 0)
					vertices[t].Z -= 250.0f;
			}
	
		updateMesh();
		return true;
	
	}
	return false;	
}

void AMyActor::changeMeshHeightByCube(TArray<FVector4> locations, float rotZ, FVector2D boxOffset) {
	UE_LOG(LogTemp, Warning, TEXT("rotY %f"), FMath::Sin(FMath::DegreesToRadians(-rotZ)));

	for (int k = 0; k < locations.Num(); k++) {
		FVector2D tempLocation;
		float x = locations[k].X-boxOffset.X;
		float y = locations[k].Y-boxOffset.Y;

		tempLocation.X = x*FMath::Cos(FMath::DegreesToRadians(-rotZ)) - y*FMath::Sin(FMath::DegreesToRadians(-rotZ));
		tempLocation.Y = y*FMath::Cos(FMath::DegreesToRadians(-rotZ)) + x*FMath::Sin(FMath::DegreesToRadians(-rotZ));

		if (tempLocation.X < 0 || tempLocation.X>700 || tempLocation.Y<0 || tempLocation.Y>500) {
			if (locations[k].W == 0) {
				int j = (locations[k].X - offset.X) / gridSize;
				int i = (locations[k].Y - offset.Y) / gridSize;
				float z = locations[k].Z - offset.Z;
				int t = i*n + j;

				if (t >= 0)
					if (t < vertices.Num())
						if ((z > vertices[t].Z))
							vertices[t].Z = z + 12.0f;


			}
		}
	}

	updateMesh();
}