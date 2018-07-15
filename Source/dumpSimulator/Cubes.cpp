// Fill out your copyright notice in the Description page of Project Settings.

#include "Cubes.h"


// Sets default values
ACubes::ACubes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;		
	createCubes(Root);
}

// Called when the game starts or when spawned
void ACubes::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubes::createCubes(USceneComponent* scene) {
	float n = 4;
	FVector scaleMesh = FVector(1.0f/n, 1.0f / n, 1.0f / n);
	int i = 0;
	FName InitialName = FName("Mesh");
	for (int x = -4; x < 4; x++)
		for (int y = -4; y < 4; y++)
			for (int z = 0; z >-8; z--)
			{
				FString Str = "Mesh" + FString::FromInt(i++);
				InitialName = (*Str);
				UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this, InitialName);				
				Mesh->SetStaticMesh(staticMesh);
				Mesh->SetRelativeLocation(FVector(x*25.0f, y*25.0f, z*25.0f));				
				Mesh->SetRelativeScale3D(scaleMesh);

				/*FBodyInstance* BodyInst = Mesh->GetBodyInstance();
				BodyInst->MassScale = 0.0f;
				BodyInst->UpdateMassProperties();
				*/
				Mesh->SetSimulatePhysics(true);	

				Mesh->AttachTo(Root);
				cubes.Add(Mesh);
				inBucket.Add(false);
			}

	UE_LOG(LogTemp, Warning, TEXT("Created"));
}

void ACubes::updateCubes(FVector location) {
	int i = 0;	
	for (int x = -4; x < 4; x++)
		for (int y = -4; y < 4; y++)
			for (int z = 0; z >-8; z--)
			{
				if (inBucket[i] == false) {
					cubes[i]->SetRelativeLocation(FVector(x*25.0f, y*25.0f, z*25.0f - 25.0f) + location);
					cubes[i]->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
					//cubes[i]->AttachTo(scene);
				}
				i++;
			}

	UE_LOG(LogTemp, Warning, TEXT("Updated"));
}

TArray<FVector4> ACubes::getCubesPosition(FVector bucketLocation) {
	TArray <FVector4> temp;
	for (auto It = cubes.CreateIterator(); It; It++)
	{
		FVector cubeLocation=(*It)->GetComponentLocation();
		float d=FVector::Dist(bucketLocation, cubeLocation);
		int i = It.GetIndex();
	
		if (d < 134.0f) {
			inBucket[i] = true;
			temp.Add(FVector4(cubeLocation.X, cubeLocation.Y, cubeLocation.Z, 1.0f));
		}
		else {
			inBucket[i] = false;
			temp.Add(FVector4(cubeLocation.X, cubeLocation.Y, cubeLocation.Z, 0.0f));
		}		
	}		
	return temp;
}

