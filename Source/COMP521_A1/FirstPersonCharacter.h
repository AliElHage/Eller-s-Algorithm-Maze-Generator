// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EllerMazeGenerator.h"
#include "FirstPersonCharacter.generated.h"

UCLASS()
class COMP521_A1_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

private:
	// Pointer to the player's movement component
	UCharacterMovementComponent* MovementComponent;

	// The maze-generating component
	EllerMazeGenerator* MazeGenerator;

	// This modifier is used to ensure that the player cannot move laterally while in the air
	float MovementModifier = 1.0f;

	// Flag to check whether the player is currently mid-jump or not
	bool bIsAirborne = false;

	int AmmoQuantity = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Landed(const FHitResult &Hit) override;

	//handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Value);

	//handles strafing
	UFUNCTION()
	void MoveRight(float Value);

	// Sets jump flag when key is pressed
	UFUNCTION()
	void OnStartJump();

	// Clears jump flag when key is released
	UFUNCTION()
	void OnStopJump();

	// Shoots a projectile
	UFUNCTION()
	void OnShootProjectile();

	// Restarts the level
	UFUNCTION()
	void RestartLevel();

	// Displays a log message on click for testing purposes
	UFUNCTION()
	void OnTestFeature();

	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
