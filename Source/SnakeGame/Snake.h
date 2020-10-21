// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Snake.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USceneComponent;
class ABodySegment;
class UArrowComponent;
class ABoard;
class UPhysicsConstraintComponent;

/* --- A LOT OF THIS CODE NEEDS REFACTORING --- */

UCLASS()
class SNAKEGAME_API ASnake : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnake();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/* --- Components --- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake")
		UStaticMeshComponent* SnakeHead{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* Camera{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation")
		UArrowComponent* ArrowComponent{nullptr};

	/* --- Initialisation --- */
	void InitSnakeHead();
	void InitCamera();
	void SetSnakeHeadBounds();

	/* --- Snake Head Variables --- */
	const FVector SnakeHeadStartingLoc{1000.0f, 1000.0f, 50.0f};
	const FRotator SnakeHeadStartingRot{0.0f, 0.0f, 0.0f};
	const FVector SnakeHeadWorldScale{1.0f, 1.0f, 1.0f};

	/* --- Camera Variables --- */
	const FVector CameraStartingLoc{1000.0f, 1000.0f, 2000.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Location")
		float CameraStartingZLoc{2000.0f};
	const FRotator CameraStartingRot{-90.0f, -90.0f, 0.0f};
	const FVector CameraWorldScale{1.0f, 1.0f, 1.0f};

	/* --- Snake Head Movement Functions --- */
	void MoveUp(float Value);
	void MoveAcross(float Value);
	FVector GetCurrentSnakeHeadLoc() const;

	/* --- Snake Head Variables --- */
	FVector MovementDirection{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SnakeHeadSpeed{100.0f};
	FVector MinSnakeHeadBounds;
	FVector MaxSnakeHeadBounds;

public:
	/* --- Snake Segments --- */

	/* --- Segment Components --- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Segments")
		TSubclassOf<ABodySegment> SegmentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Segments")
		USceneComponent* SegmentSpawnPoint{nullptr};
	ABodySegment* FirstSegment{nullptr};
	ABodySegment* NextSegment{nullptr};
	int32 SegmentIndex{0};

	void SpawnSegment();
	void UpdateNextSegmentLoc();
	void UpdateFirstBodySegmentLoc();
	void UpdateFirstBodyRotation();
	void UpdateNextSegmentRotation();
	FVector GetPreviousSegmentLoc();
	TArray<ABodySegment*> SegmentArray;
	UPROPERTY(EditAnywhere, Category = "Board Ref")
		ABoard* BoardRef{nullptr};
	FVector ForwardVec{};

	/* --- Storing Locations on Tick --- */
	FVector SnakeHeadTickLoc{};
	FVector FirstSegmentTickLoc{};
	FVector NextSegmentTickLoc{};
	TArray <FVector> NextSegmentTickArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Constraint")
		UPhysicsConstraintComponent* PhysicsConstraint;
};
