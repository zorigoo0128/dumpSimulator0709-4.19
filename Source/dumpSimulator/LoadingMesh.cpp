// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadingMesh.h"


// Sets default values
ALoadingMesh::ALoadingMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void ALoadingMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoadingMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void ALoadingMesh::PostActorCreated()
{
	Super::PostActorCreated();
	CreateTriangle();
}

// This is called when actor is already in level and map is opened
void ALoadingMesh::PostLoad()
{
	Super::PostLoad();
	CreateTriangle();
}

FVector ALoadingMesh::calculateNormal(int i) {

	float heightL = 0, heightR = 0, heightU = 0, heightD = 0;
	if (i - 1 >= 0)
		heightL = vertices[i - 1].Z;
	if (i + 1 < vertices.Num())
		heightR = vertices[i + 1].Z;
	if (i - length >= 0)
		heightU = vertices[i - length].Z;
	if (i + length <vertices.Num())
		heightD = vertices[i + length].Z;

	FVector  normal = FVector(heightL - heightR, 2.0f, heightD - heightU);
	float magnitude = FMath::Sqrt(normal.X*normal.X + normal.Y*normal.Y + normal.Z*normal.Z);
	return normal / magnitude;

}

void ALoadingMesh::CreateTriangle()
{

	for (int i = 0; i<width; i++)
		for (int j = 0; j < length; j++) {
			vertices.Add(FVector(j*gridSize, i*gridSize, 0));

			UV0.Add(FVector2D((float)j / (length - 1), (float)i / (width - 1)));

		}

	for (int i = 0; i<width; i++)
		for (int j = 0; j < length; j++) {
			normals.Add(calculateNormal(i*length + j));
		}


	for (int i = 0; i<width - 1; i++)
		for (int j = 0; j < length - 1; j++) {
			int v0 = i*length + j;
			int v1 = v0 + 1;
			int v2 = v0 + length;
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
}

void ALoadingMesh::updateMesh() {
	mesh->UpdateMeshSection_LinearColor(0, vertices, normals, UV0, vertexColors, tangents);
}

void ALoadingMesh::setOffset(FVector location) {
	this->offset = location;
}

void ALoadingMesh::changeMeshHeightByCube(TArray<FVector4> locations, float rotZ) {

	//rotZ : loadingMesh iin world rotation Z iin utga

	bool smoothing = false;
	for (int k = 0; k < locations.Num(); k++) {

		FVector2D tempLocation;
		float x = locations[k].X - offset.X;
		float y = locations[k].Y -offset.Y;		

		//cube iin location iig loading mesh iin coordinatiin system ruu hurvuul bn.
		tempLocation.X = x*FMath::Cos(FMath::DegreesToRadians(-rotZ)) - y*FMath::Sin(FMath::DegreesToRadians(-rotZ));
		tempLocation.Y = y*FMath::Cos(FMath::DegreesToRadians(-rotZ)) + x*FMath::Sin(FMath::DegreesToRadians(-rotZ));		

		//loading mesh-d orj bga esehiig tegsh ontsogtd helbereer shalgaj bn
		if (tempLocation.X > 0 && tempLocation.X<700 && tempLocation.Y>0 && tempLocation.Y<500) {			
			if (locations[k].W == 0) {
				int j = tempLocation.X / gridSize;
				int i = tempLocation.Y / gridSize;
				float z = locations[k].Z - offset.Z;
				int t = i*length + j;

				if (t >= 0) {
					if (t < vertices.Num()) {
						if ((z > vertices[t].Z)) {
							vertices[t].Z += 50.0f;

							//ergen toirnii 4n tsegiin meshiig bas ondorsgoj bga
							if (t - 1 >= 0)
								vertices[t - 1].Z = vertices[t].Z;
							if (t + 1 < vertices.Num())
								vertices[t + 1].Z = vertices[t].Z;
							if (t - length >= 0)
								vertices[t - length].Z = vertices[t].Z;
							if (t + length < vertices.Num())
								vertices[t + length].Z = vertices[t].Z;

							smoothing = true;

						}
					}
				}	

			}
		}
	}

	if (smoothing == true) {
		smoothVertices();
		updateMesh();
	}
}

void ALoadingMesh::smoothVertices() {
	//ergen toirnii 8 tsegtei dundjilj smooth hj bna

	for (int t = 0; t < vertices.Num(); t++) {
		float z = vertices[t].Z;
		float LB = 0, B = 0, RB = 0, L = 0, R = 0, LF = 0, F = 0, RF = 0;
		float s = 0;

		if (t - length - 1 >= 0 && t - length - 1 < vertices.Num()) {
			LB = vertices[t - length - 1].Z;
			s++;
		}

		if (t - length >= 0 && t - length < vertices.Num()) {
			B = vertices[t - length].Z;
			s++;
		}

		if (t - length + 1 >= 0 && t - length + 1 < vertices.Num()) {
			RB = vertices[t - length + 1].Z;
			s++;
		}

		if (t - 1 >= 0 && t - 1 < vertices.Num()) {
			L = vertices[t - 1].Z;
			s++;
		}

		if (t + 1 >= 0 && t + 1 < vertices.Num()) {
			R = vertices[t + 1].Z;
			s++;
		}

		if (t + length - 1 >= 0 && t + length - 1 < vertices.Num()) {
			LF = vertices[t + length - 1].Z;
			s++;
		}

		if (t + length >= 0 && t + length < vertices.Num()) {
			F = vertices[t + length].Z;
			s++;
		}

		if (t + length + 1 >= 0 && t + length + 1 < vertices.Num()) {
			RF = vertices[t + length + 1].Z;
			s++;
		}

		s++;
		vertices[t].Z = (z + LB + B + RB + L + R + LF + F + RF) / s;

	}
}

