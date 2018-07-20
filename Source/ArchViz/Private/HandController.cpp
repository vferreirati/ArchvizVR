// Fill out your copyright notice in the Description page of Project Settings.

#include "HandController.h"
#include "MotionControllerComponent.h"

AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->bDisplayDeviceModel = true;
	SetRootComponent(MotionController);
}

void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandController::SetHand(EControllerHand Hand) {
	MotionController->SetTrackingSource(Hand);
}

