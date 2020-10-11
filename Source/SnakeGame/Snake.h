// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Snake.generated.h"

class UStaticMeshComponent;
class UCameraComponent;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
		UStaticMeshComponent* Board{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake")
		UStaticMeshComponent* SnakeHead{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* Camera{nullptr};

	/* --- Initialisation --- */
	void InitBoard();
	void InitSnakeHead();
	void InitCamera();
	void SetBoardBounds();
	void SetSnakeHeadBounds();

	/* --- Board Variables --- */
	const FVector BoardWorldLoc{0.0f, 0.0f, 0.0f};
	const FRotator BoardWorldRot{0.0f, 0.0f, 0.0f,};
	const FVector BoardWorldScale{20.0f, 20.0f, 1.0f};
	FVector MinBoardBounds;
	FVector MaxBoardBounds;

	/* --- Snake Head Variables --- */
	const FVector SnakeHeadStartingLoc{20.0f, 20.0f, 50.0f};
	const FRotator SnakeHeadStartingRot{0.0f, 0.0f, 0.0f};
	const FVector SnakeHeadWorldScale{1.0f, 1.0f, 1.0f};

	/* --- Camera Variables --- */
	const FVector CameraStartingLoc{50.0f, 50.0f, 2000.0f};
	const FRotator CameraStartingRot{-90.0f, -90.0f, 0.0f};
	const FVector CameraWorldScale{1.0f, 1.0f, 1.0f};

	/* --- Snake Head Movement Functions --- */
	void MoveUp(float Value);
	void MoveAcross(float Value);

	/* --- Snake Head Variables --- */
	FVector MovementDirection{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SnakeHeadSpeed{100.0f};
	FVector MinSnakeHeadBounds;
	FVector MaxSnakeHeadBounds;

	// TODO :: May need to change board bounds - Currently the board bounds are not in keeping with its scale. I.e. it's max bounds are still 100, not 2000
};
