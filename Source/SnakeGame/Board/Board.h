// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"

class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Mesh")
		UStaticMeshComponent* BoardMesh;
	
	float BoardHeight{};

	/* --- Board Variables --- */
	const FVector BoardWorldLoc{0.0f, 0.0f, 0.0f};
	const FRotator BoardWorldRot{0.0f, 0.0f, 0.0f,};
	const FVector BoardWorldScale{20.0f, 20.0f, 1.0f};
	FVector MinBoardBounds;
	FVector MaxBoardBounds;

	void InitBoard();
	void SetBoardBounds();

	FVector GetCentreOfBoard();
};
