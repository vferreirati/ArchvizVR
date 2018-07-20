// Fill out your copyright notice in the Description page of Project Settings.

#include "HandController.h"
#include "MotionControllerComponent.h"
#include "Gameframework/Character.h"
#include "Gameframework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	bCanClimb = false;
	bIsClimbing = false;
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

	if (bIsClimbing) {
		if (AActor* Owner = GetOwner()) {
			// Get the LocationOffset
			FVector OffsetLocation = GetActorLocation() - ClimbingStartLocation;

			// Move Player using the movement offset
			Owner->AddActorWorldOffset(-OffsetLocation);

			// Set the controller back to start location
			SetActorLocation(ClimbingStartLocation);
		}
	}
}

void AHandController::SetHand(EControllerHand Hand) {
	MotionController->SetTrackingSource(Hand);
}

void AHandController::PairControllers(AHandController* Controller) {
	OtherController = Controller;
	OtherController->OtherController = this;
}

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	if (bCanClimb) {
		return;
	}

	bool bNewCanClimb = CanClimb();
	if (bNewCanClimb) {
		bCanClimb = true;
		PlayHapticEffect();
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

void AHandController::PlayHapticEffect() {
	if (!HapticEffect) {
		return;
	}

	if (ACharacter* Owner = Cast<ACharacter>(GetOwner())) {
		if (APlayerController* PC = Cast<APlayerController>(Owner->GetController())) {
			PC->PlayHapticEffect(HapticEffect, MotionController->GetTrackingSource());
		}
	}
}

void AHandController::Grip() {
	if (!bCanClimb) {
		return;
	}
	
	// Store the start location
	ClimbingStartLocation = GetActorLocation();

	// Set that we're climbing
	bIsClimbing = true;

	OtherController->Release();

	// Update movement mode
	if (ACharacter* Owner = Cast<ACharacter>(GetOwner())) {
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}

void AHandController::Release() {
	if (bIsClimbing) {
		// Set that we're not climbing
		bIsClimbing = false;

		// Update movement mode
		if (ACharacter* Owner = Cast<ACharacter>(GetOwner())) {
			Owner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
	}
}