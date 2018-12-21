// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "EllerMazeGenerator.h"
#include "LevelGenerator.generated.h"

UCLASS()
class COMP521_A1_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

private:
	EllerMazeGenerator* MazeGenerator;

	USceneComponent* SceneRoot = nullptr;

	UBoxComponent* BoxComponent;

	UStaticMeshComponent* MazeLedge;

	int globalCounter = 0;

	int floorCounter = 0;

	void GenerateMazeRow(bool bIsLastRow);

public:

	UPROPERTY(EditAnywhere, CATEGORY = "Maze Dimensions")
	float LeftmostWallXCoordinate = -1200.0f;

	UPROPERTY(EditAnywhere, CATEGORY = "Maze Dimensions")
	float CellSize = 300.0f;

	UPROPERTY(EditAnywhere, CATEGORY = "Maze Dimensions")
	int MazeCellWidth = 8;

	UPROPERTY(EditAnywhere, CATEGORY = "Trigger Dimensions")
	float BoxTriggerLength = 50.0f;

	UPROPERTY(EditAnywhere, CATEGORY = "Trigger Dimensions")
	float BoxTriggerWidth = 2400.0f;

	UPROPERTY(EditAnywhere, CATEGORY = "Trigger Dimensions")
	float BoxTriggerDepth = 200.0f;

	UPROPERTY(EditAnywhere, CATEGORY = "HISMC Properties")
	UInstancedStaticMeshComponent* HismcBaseFloor;

	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult &SweepResult);
	
};
