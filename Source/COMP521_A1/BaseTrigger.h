// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "BaseTrigger.generated.h"

UCLASS()
class COMP521_A1_API ABaseTrigger : public AActor
{
	GENERATED_BODY()
	
private:
	USceneComponent* SceneRoot = nullptr;

	UBoxComponent* BoxComponent;

	void SetUpBoxTrigger();

public:	
	// Sets default values for this actor's properties
	ABaseTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wEndGameScreen;

	UUserWidget* EndGameScreen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);
	
};
