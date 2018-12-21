// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BaseProjectile.h"
#include "PickupItem.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MazeGenerator = new EllerMazeGenerator(5);

	MovementComponent = GetCharacterMovement();

	// Set how high the player can jump and whether they have control over their air speed
	if (MovementComponent != NULL)
	{
		MovementComponent->JumpZVelocity = 1600.0f;
		MovementComponent->AirControl = 1.0f;
	}

	// Hook up callback to overlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFirstPersonCharacter::OnCharacterOverlap);
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up axis bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFirstPersonCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFirstPersonCharacter::AddControllerPitchInput);

	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonCharacter::OnStopJump);
	PlayerInputComponent->BindAction("ShootProjectile", IE_Pressed, this, &AFirstPersonCharacter::OnShootProjectile);
	PlayerInputComponent->BindAction("TestButton", IE_Pressed, this, &AFirstPersonCharacter::OnTestFeature);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AFirstPersonCharacter::RestartLevel);
}

void AFirstPersonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value * MovementModifier);
	}
}

void AFirstPersonCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Value * MovementModifier);
	}
}

void AFirstPersonCharacter::OnStartJump()
{
	// Set movement speed to 0 and nullify axis inputs when the player is in the air
	if (MovementComponent != NULL)
	{
		MovementComponent->Velocity.X = 0.0f;
		MovementComponent->Velocity.Y = 0.0f;
	}

	bPressedJump = true;
	bIsAirborne = true;

	MovementModifier = 0.0f;
}

void AFirstPersonCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AFirstPersonCharacter::Landed(const FHitResult &Hit)
{
	// Reenable movement as soon as the player is back on the ground
	Super::Landed(Hit);

	bIsAirborne = false;

	MovementModifier = 1.0f;
}

void AFirstPersonCharacter::OnShootProjectile()
{
	// Shoot the projectile
	if (!bIsAirborne && AmmoQuantity > 0)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), GetActorLocation());
		FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor<ABaseProjectile>(GetActorLocation(), Rotation, SpawnInfo);

		AmmoQuantity--;
	}
}

void AFirstPersonCharacter::RestartLevel()
{
	// Restart the level using the 'R' key
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AFirstPersonCharacter::OnTestFeature()
{
	// This is for debugging purposes
	if (MazeGenerator != NULL)
	{
		for (int i = 0; i < 5; i++)
		{
			MazeGenerator->GenerateRow(false);
		}

		MazeGenerator->GenerateRow(true);

		FString mazeContents = FString(MazeGenerator->MazeToString());
	}
}

void AFirstPersonCharacter::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Pickup ammo and destroy the ammo actor
	if (Cast<APickupItem>(OtherActor))
	{
		AmmoQuantity++;
		OtherActor->Destroy();
	}
}

