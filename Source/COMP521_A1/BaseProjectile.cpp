// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the mesh and its collision
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Meshes/Projectile/Hatchet.Hatchet'"));
	UStaticMesh* Asset = MeshAsset.Object;

	if (Mesh != NULL)
	{
		Mesh->SetStaticMesh(Asset);
		Mesh->SetRelativeScale3D(FVector(6.0f, 12.0f, 6.0f));
		Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereComponent->SetSphereRadius(25.0f);
	SphereComponent->bDynamicObstacle = true;
	SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->bShouldCollideWhenPlacing = false;
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	if (GetWorld() != NULL)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController != NULL)
		{
			const FRotator Rotation = PlayerController->GetControlRotation();
			const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

			LocalMovementAxis = Direction.GetSafeNormal();
			LocalMovementAxis.Z = 0.0f;

			if (Mesh != NULL)
			{
				FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
				Mesh->SetRelativeRotation(LookAtRotation);
			}
		}
	}
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the projectile once it is spawned
	float displacement = DeltaTime * ProjectileSpeed;
	SetActorLocation(GetActorLocation() + displacement * LocalMovementAxis);

	// If the projectile gets too far, it is destroyed
	CurrentDistance += displacement;
	if (CurrentDistance > MaxDistance)
	{
		Destroy();
	}

	// Rotate the mesh for style points
	if (Mesh != NULL)
	{
		Mesh->AddLocalRotation(FRotator( displacement * (-1.5f), 0.0f, 0.0f));
	}
}

