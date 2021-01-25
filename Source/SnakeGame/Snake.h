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
class UBoxComponent;
class AFood;

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

private:
	/* --- Components --- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* SnakeHead{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Location", meta = (AllowPrivateAccess = "true"))
		float CameraStartingZLoc{2000.0f};
	const FRotator CameraStartingRot{-90.0f, -90.0f, 0.0f};
	const FVector CameraWorldScale{1.0f, 1.0f, 1.0f};

	/* --- Snake Head Movement Functions --- */
	FVector GetCurrentSnakeHeadLoc() const;
	UFUNCTION()
		void MoveUp();
	UFUNCTION()
        void MoveDown();
	UFUNCTION()
        void MoveLeft();
	UFUNCTION()
        void MoveRight();

	/* --- Snake Head Variables --- */
	FVector MovementDirection{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float SnakeHeadSpeed{100.0f};
	FVector MinSnakeHeadBounds;
	FVector MaxSnakeHeadBounds;
	FVector MoveAmount;
	FVector MoveDir;
	float MoveStepSize;

	/* --- Snake Segments --- */
	void SpawnSegment();
	void UpdateTailSegmentLoc();
	TArray<ABodySegment*> TailSegmentArray;

	/* --- Segment Components --- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Segments", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABodySegment> SegmentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Segments", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SegmentSpawnPoint{nullptr};
	ABodySegment* TailSegment{nullptr};

	/* --- Storing the Board actor in BoardRef --- */
	UPROPERTY(EditAnywhere, Category = "Board Ref")
		ABoard* BoardRef{nullptr};

	/* --- Storing Segment Locations on Tick --- */
	FVector PreviousTailSegmentLoc;
	FVector PreviousTailSegmentForwardVec;



	/* --- Food Code --- */
	UFUNCTION()
		void SpawnFood();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AFood> FoodClass;

	ASnake* Snake{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* SnakeCollisionBox;

	AFood* Food{ nullptr };

	UFUNCTION()
		virtual void PostInitializeComponents() override;
	UFUNCTION()
		void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere)
		float TickChangeAmount = 0.9f;
	UPROPERTY(EditAnywhere)
		float TickStartSpeed = 0.5f;
	
	float TickSpeedCheck;
	
	TArray<AActor*> OverlappingActors;
};

