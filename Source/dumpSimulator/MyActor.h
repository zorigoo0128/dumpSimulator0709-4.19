// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"


UCLASS()
class DUMPSIMULATOR_API AMyActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent * mesh;	

	int n = 100; //grid number
	int gridSize = 50;

	FVector offset;

	TArray<FVector> vertices;
	TArray<FVector> verticesBefore;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void PostActorCreated();	
	void PostLoad();	
	void CreateTriangle();
	void updateMesh();
	FVector calculateNormal(int i);
	
	UFUNCTION(BlueprintCallable)
		void setOffset(FVector location);

	UFUNCTION(BlueprintCallable)
		bool recieveForkLocation(FVector forkLocation);

	UFUNCTION(BlueprintCallable)
		void changeMeshHeightByCube(TArray<FVector4> locations, float rotZ, FVector2D boxOffset);
	
};
