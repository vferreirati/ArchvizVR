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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPostProcessComponent* PostProcessComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMotionControllerComponent* MotionControllerLeft;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMotionControllerComponent* MotionControllerRight;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USplineComponent* SplineComp;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	float TeleportProjectileRadius;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	float TeleportProjectileSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	float TeleportSimulationTime;

	FTimerHandle TimerHandle_TeleportFade;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	float TeleportFadeDuration;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	class UMaterialInterface* BlinkerMaterialParent;

	class UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	class UCurveFloat* RadiusVsVelocityCurve;

	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	class UStaticMesh* TeleportArcMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "VRCharacter")
	class UMaterialInterface* TeleportArcMaterial;

	TArray<class USplineMeshComponent*> TeleportPathMeshPool;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement bind functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	// Teleport bind function
	void BeginTeleport();
	void EndTeleport();

	// Find the teleport destination based on the MotionController
	bool FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation);

	// Updates the destination marker position
	void UpdateDestinationMarker();

	// Updates the radius of the blinkers
	void UpdateBlinkers();

	// Updates the Arc of the Spline Component
	void UpdateSpline(const TArray<FVector> &Path);

	// Draws the teleport arc using the ObjectPool
	void DrawTeleportPath(const TArray<FVector> &Path);

	// Calculates the center of motion
	FVector2D GetBlinkerCenter();

	void HideSplineMeshes();
};
