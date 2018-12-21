// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTrigger.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"


// Sets default values
ABaseTrigger::ABaseTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetUpBoxTrigger();
}

// Called when the game starts or when spawned
void ABaseTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseTrigger::SetUpBoxTrigger()
{
	// This actor is basically a collision box that can be placed in the world. This initializes it.
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	BoxComponent->SetBoxExtent(FVector(50.0f, 600.0f, 400.0f));
	BoxComponent->bDynamicObstacle = true;
	BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxComponent->bGenerateOverlapEvents = true;
	BoxComponent->bShouldCollideWhenPlacing = false;
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Hook up event callback
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseTrigger::OnTriggerBeginOverlap);
}

void ABaseTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// If the colliding actor is the player, disable input and show the end game screen
	APawn* player = Cast<APawn>(OtherActor);
	if (player != NULL)
	{
		APlayerController* controller = Cast<APlayerController>(player->GetController());
		if (controller != NULL)
		{
			if (wEndGameScreen != NULL)
			{
				EndGameScreen = CreateWidget<UUserWidget>(GetGameInstance(), wEndGameScreen);
				EndGameScreen->AddToViewport();
			}

			player->DisableInput(controller);
		}
	}
}


