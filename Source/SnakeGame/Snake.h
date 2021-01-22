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
class AFruit;

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
	void SpawnSegment();
	void UpdateTailSegmentLoc();
	TArray<ABodySegment*> TailSegmentArray;

	/* --- Segment Components --- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Segments")
		TSubclassOf<ABodySegment> SegmentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Segments")
		USceneComponent* SegmentSpawnPoint{nullptr};
	ABodySegment* TailSegment{nullptr};

	/* --- Storing the Board actor in BoardRef --- */
	UPROPERTY(EditAnywhere, Category = "Board Ref")
		ABoard* BoardRef{nullptr};

	/* --- Storing Segment Locations on Tick --- */
	FVector PreviousTailSegmentLoc;
	FVector PreviousTailSegmentForwardVec;
	FVector MoveAmount;
	
	UFUNCTION()
		void MoveUp();
	UFUNCTION()
		void MoveDown();
	UFUNCTION()
		void MoveLeft();
	UFUNCTION()
		void MoveRight();

	FVector MoveDir;
	float MoveStepSize;

	float TickSpeed;

	/* --- Food Code --- */
	UFUNCTION()
		void SpawnFruit();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fruit")
		TSubclassOf<AFruit> FruitClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fruit")
		AFruit* Fruit{nullptr};

	ASnake* Snake{nullptr};

	AActor* Cube{nullptr};
	
	UFUNCTION()
		void SnakeEatsFood();
};
