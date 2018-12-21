// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "MazeWall.generated.h"

UCLASS()
class COMP521_A1_API AMazeWall : public AActor
{
	GENERATED_BODY()
	
private:
	UStaticMeshComponent* Wall;

	UBoxComponent* BoxComponent;

	USceneComponent* SceneRoot = nullptr;

public:
	UPROPERTY(EditAnywhere, CATEGORY = "Wall Properties")
	bool bIsTicking = true;

	UPROPERTY(EditAnywhere, CATEGORY = "Wall Properties")
	int HealthPoints = 3;

	// Sets default values for this actor's properties
	AMazeWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnWallBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
