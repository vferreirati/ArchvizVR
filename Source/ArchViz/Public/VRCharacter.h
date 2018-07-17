// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class ARCHVIZ_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* DestinationMarker;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	float TeleportRange;

	FTimerHandle TimerHandle_TeleportFade;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	float TeleportFadeDuration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement bind functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	// Teleport bind function
	void BeginTeleport();
	void EndTeleport();

	// Find the teleport destination based on where the player is looking at
	bool FindTeleportDestination(FVector& OutLocation);

	void UpdateDestinationMarker();
};
