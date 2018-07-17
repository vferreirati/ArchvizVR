// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"


// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -1 + 2.f));
	VRRoot->SetupAttachment(GetRootComponent());	

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(VRRoot);

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
	DestinationMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestinationMarker->SetupAttachment(GetRootComponent());
	DestinationMarker->SetVisibility(false);

	TeleportRange = 1000.f;
	TeleportFadeDuration = 0.5f;
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewCameraOffset = CameraComp->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0.f;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);

	UpdateDestinationMarker();
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement Input Binding
	PlayerInputComponent->BindAxis("MoveForward", this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVRCharacter::MoveRight);

	// Teleport input binding
	PlayerInputComponent->BindAction("Teleport", IE_Released, this, &AVRCharacter::BeginTeleport);
}

void AVRCharacter::MoveForward(float Value) {
	AddMovementInput(CameraComp->GetForwardVector(), Value);
}

void AVRCharacter::MoveRight(float Value) {
	AddMovementInput(CameraComp->GetRightVector(), Value);
}

void AVRCharacter::BeginTeleport() {
	// Set timer to teleport
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportFade, this, &AVRCharacter::EndTeleport, TeleportFadeDuration);

	// Fade camera
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) {
		PC->PlayerCameraManager->StartCameraFade(0.f, 1.f, TeleportFadeDuration, FLinearColor::Black, false, true);
	}
}

void AVRCharacter::EndTeleport() {
	FVector NewLocation = DestinationMarker->GetComponentLocation();
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Update the Z value to pop the player off the ground
	NewLocation.Z += CapsuleHalfHeight + 2;
	
	// Teleport the Player
	SetActorLocation(NewLocation);

	// Fade the camera back
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) {
		PC->PlayerCameraManager->StopCameraFade();
	}
}

void AVRCharacter::UpdateDestinationMarker() {

	FVector TraceStart = CameraComp->GetComponentLocation();
	FRotator CameraRotation = CameraComp->GetComponentRotation();
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * TeleportRange);

	FHitResult HitResult;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);

	if (bSuccess) {
		DestinationMarker->SetWorldLocation(HitResult.Location);
	}

	DestinationMarker->SetVisibility(bSuccess);
}