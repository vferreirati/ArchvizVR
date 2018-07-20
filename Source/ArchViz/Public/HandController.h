// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandController.generated.h"

UCLASS()
class ARCHVIZ_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHand(EControllerHand Hand);
	void PairControllers(AHandController* Controller);

	void Grip();
	void Release();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMotionControllerComponent* MotionController;

	UPROPERTY(EditDefaultsOnly, Category = "HandController")
	class UHapticFeedbackEffect_Base* HapticEffect;

	AHandController* OtherController;

	bool bCanClimb;

	bool bIsClimbing;

	FVector ClimbingStartLocation;

	FName ClimbableTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CanClimb() const;

	void PlayHapticEffect();

	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
};
