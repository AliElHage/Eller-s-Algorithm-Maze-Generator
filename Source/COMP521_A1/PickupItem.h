// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "PickupItem.generated.h"

UCLASS()
class COMP521_A1_API APickupItem : public AActor
{
	GENERATED_BODY()
	
private:
	UStaticMeshComponent* Mesh;

	USphereComponent* SphereComponent;

	USceneComponent* SceneRoot = nullptr;

	float rotationRandomizer = 0.0f;

public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
