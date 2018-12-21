// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeWall.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BaseProjectile.h"
#include "Engine.h"


// Sets default values
AMazeWall::AMazeWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/Maze/MazeWall.MazeWall'"));
	UStaticMesh* WallAsset = MeshAsset.Object;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	// Create mesh for the wall
	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	Wall->SetupAttachment(RootComponent);
	Wall->SetStaticMesh(WallAsset);

	// Create collision box for the wall
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	BoxComponent->SetBoxExtent(FVector(150.0f, 25.0f, 75.0f));
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BoxComponent->bDynamicObstacle = true;
	BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->bGenerateOverlapEvents = true;
	BoxComponent->bShouldCollideWhenPlacing = false;
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);

	// Hook up callback to overlap event. This is for when a projectile hits the wall
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMazeWall::OnWallBeginOverlap);
}

// Called when the game starts or when spawned
void AMazeWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMazeWall::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PrimaryActorTick.bCanEverTick = bIsTicking;
}

// Called every frame
void AMazeWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeWall::OnWallBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// When hit by a projectile, decrement the wall's HP. If HP reaches 0, the wall is destroyed.
	// Edge walls start with a value of 0 HP so they are never destroyed.
	if (Cast<ABaseProjectile>(OtherActor) != NULL)
	{
		OtherActor->Destroy();
		if (--HealthPoints == 0)
		{
			Destroy();
		}
	}
}

