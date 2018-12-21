// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MazeWall.h"
#include "BaseProjectile.h"
#include "PickupItem.h"
#include <vector>
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/World.h"


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	// Set up the box trigger that will be pushed back everytime a new row is generated
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	BoxComponent->SetBoxExtent(FVector(BoxTriggerLength, BoxTriggerWidth, BoxTriggerDepth));
	BoxComponent->bDynamicObstacle = true;
	BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->bGenerateOverlapEvents = true;
	BoxComponent->bShouldCollideWhenPlacing = false;
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	FVector initialPosition = FVector(50.0f, 0, 225.0f);

	BoxComponent->SetRelativeLocation(initialPosition);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelGenerator::OnTriggerBeginOverlap);

	// Create and place a ledge after each row that will allow the player to step on it to generate
	// the new row.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/Maze/MazeLedge.MazeLedge'"));
	UStaticMesh* MazeLedgeAsset = MeshAsset.Object;

	MazeLedge = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Maze Ledge"));
	MazeLedge->SetupAttachment(RootComponent);
	MazeLedge->SetStaticMesh(MazeLedgeAsset);

	initialPosition = FVector(50.0f, 0, 0.0f);
	MazeLedge->SetRelativeLocation(initialPosition);
	MazeLedge->bGenerateOverlapEvents = false;

	// Create a maze generator instance
	MazeGenerator = new EllerMazeGenerator(MazeCellWidth);
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BoxComponent->bGenerateOverlapEvents = true;
}

void ALevelGenerator::GenerateMazeRow(bool bIsLastRow)
{
	// Move trigger forward
	FVector updatedTranslation = BoxComponent->GetRelativeTransform().GetTranslation();
	updatedTranslation += FVector(CellSize, 0, 0);
	BoxComponent->SetRelativeLocation(updatedTranslation);

	// Move ledge forward
	if (MazeLedge != NULL)
	{
		FVector updatedLedgeTranslation = MazeLedge->GetRelativeTransform().GetLocation();
		updatedLedgeTranslation += FVector(CellSize, 0.0f, 0.0f);
		MazeLedge->SetRelativeLocation(updatedLedgeTranslation);
	}

	// Create new ground
	if (HismcBaseFloor != NULL)
	{
		HismcBaseFloor->AddInstance(FTransform(FVector(globalCounter * CellSize + (0.5f * CellSize), 0, 0)));
	}

	BoxComponent->bGenerateOverlapEvents = false;

	// Generate the row to spawn
	std::vector<MazeCell*> mazeRow = MazeGenerator->GenerateRow(bIsLastRow);

	// Spawn leftmost wall and set HP to 0 to ensure it's indestructible
	FActorSpawnParameters SpawnInfo;
	AMazeWall* leftmostVerticalWall = GetWorld()->SpawnActor<AMazeWall>(FVector(globalCounter * CellSize + (0.5f * CellSize), LeftmostWallXCoordinate, 100.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	leftmostVerticalWall->HealthPoints = 0;

	for (int i = 0; i < mazeRow.size(); i++)
	{
		MazeCell* currentCell = mazeRow[i];

		// Spawn vertical walls
		if (currentCell != NULL && currentCell->bHasWallRight)
		{
			FActorSpawnParameters SpawnInfo;
			AMazeWall* verticalWall = GetWorld()->SpawnActor<AMazeWall>(FVector(globalCounter * CellSize + (0.5f * CellSize), LeftmostWallXCoordinate + (i + 1) * CellSize, 100.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);

			// ensure the rightmost wall also cannot be destroyed
			if (verticalWall != NULL && i == mazeRow.size() - 1)
			{
				verticalWall->HealthPoints = 0;
			}
		}

		// Spawn horizontal walls
		if (currentCell != NULL && currentCell->bHasWallBelow)
		{
			FActorSpawnParameters SpawnInfo;
			AMazeWall* horizontalWall = GetWorld()->SpawnActor<AMazeWall>(FVector((globalCounter + 1) * CellSize, LeftmostWallXCoordinate + i * CellSize + (0.5f * CellSize), 100.0f), FRotator(0.0f, 90.0f, 0.0f), SpawnInfo);
			if (bIsLastRow)
			{
				horizontalWall->HealthPoints = 0;
			}
		}

		// Spawn pickup item (ammo)
		FActorSpawnParameters SpawnInfo;
		APickupItem* verticalWall = GetWorld()->SpawnActor<APickupItem>(FVector(globalCounter * CellSize + (0.5f * CellSize), LeftmostWallXCoordinate + i * CellSize + (0.5f * CellSize), 50.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
	}
}

void ALevelGenerator::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Generate a new row depending on whether a projectile or the player hit the trigger.
	if (Cast<ACharacter>(OtherActor))
	{
		GenerateMazeRow(false);
	}
	else if (Cast<ABaseProjectile>(OtherActor))
	{
		GenerateMazeRow(true);
	}

	globalCounter++;
}

