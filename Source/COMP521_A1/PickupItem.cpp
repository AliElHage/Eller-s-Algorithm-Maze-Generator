// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"


// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Meshes/Projectile/Hatchet.Hatchet'"));
	UStaticMesh* Asset = MeshAsset.Object;

	// Set up the pickup item's mesh
	if (Mesh != NULL)
	{
		Mesh->SetStaticMesh(Asset);
		Mesh->SetRelativeScale3D(FVector(4.0f, 8.0f, 4.0f));
		Mesh->SetRelativeLocation(FVector(-20.0f, 0.0f, -10.0f));
		Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Give the pickup a spherical trigger
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereComponent->SetSphereRadius(25.0f);
	SphereComponent->bDynamicObstacle = true;
	SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->bShouldCollideWhenPlacing = false;
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Create a random number generator to offset the rotation of each pickup item. This is
	// only to make the game feel more organic.
	rotationRandomizer = FMath::FRandRange(0.8f, 1.2f);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Spin the pickup item to draw attention to it.
	AddActorLocalRotation(FRotator(0.0f, rotationRandomizer * DeltaTime * 30.0f, 0.0f));

}

