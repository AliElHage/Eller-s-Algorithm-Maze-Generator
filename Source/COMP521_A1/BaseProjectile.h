// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class COMP521_A1_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

private:
	// The mesh of the projectile
	UStaticMeshComponent* Mesh;

	USphereComponent* SphereComponent;

	USceneComponent* SceneRoot = nullptr;

	// Axis along which the projectile moves
	FVector LocalMovementAxis;

	// The current distance the projectile has crossed since being shot
	float CurrentDistance = 0.0f;

public:
	// Speed at which the projectile moves
	UPROPERTY(EditAnywhere, CATEGORY = "Projectile Properties")
	float ProjectileSpeed = 1200.0f;

	// Max distance the projectile can cross before being destroyed
	UPROPERTY(EditAnywhere, CATEGORY = "Projectile Properties")
	float MaxDistance = 6400.0f;

public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
