// Fill out your copyright notice in the Description page of Project Settings.

#include "HandController.h"
#include "MotionControllerComponent.h"

AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	bCanClimb = false;
	ClimbableTag = "Climbable";
}

void AHandController::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverlap);
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandController::SetHand(EControllerHand Hand) {
	MotionController->SetTrackingSource(Hand);
}

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	if (bCanClimb) {
		return;
	}

	bool bNewCanClimb = CanClimb();
	if (bNewCanClimb) {
		bCanClimb = true;
		UE_LOG(LogTemp, Warning, TEXT("Can climb!"));
	}
}

void AHandController::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	bCanClimb = CanClimb();	
}

bool AHandController::CanClimb() const {
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors) {
		if (Actor->ActorHasTag(ClimbableTag)) {
			return true;
		}
	}

	return false;
}