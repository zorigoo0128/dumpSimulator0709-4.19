// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cubes.generated.h"

UCLASS()
class DUMPSIMULATOR_API ACubes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubes();


	UPROPERTY()
	USceneComponent* Root;

	TArray<UStaticMeshComponent*> cubes;
	UStaticMesh* staticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/StarterContent/Shapes/Shape_Cube")));
	TArray<bool> inBucket;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void createCubes(USceneComponent* scene);

	UFUNCTION(BlueprintCallable)
	void updateCubes(FVector location);

	UFUNCTION(BlueprintCallable)
	TArray<FVector4> getCubesPosition(FVector bucketLocation);

	
	
};
