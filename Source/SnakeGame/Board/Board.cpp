// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

void ABoard::InitBoard()
{
	BoardMesh->SetWorldLocation(BoardWorldLoc);
	BoardMesh->SetWorldRotation(BoardWorldRot);
	BoardMesh->SetWorldScale3D(BoardWorldScale);
}

void ABoard::SetBoardBounds()
{
	BoardMesh->GetLocalBounds(OUT MinBoardBounds, OUT MaxBoardBounds);
}

FVector ABoard::GetCentreOfBoard()
{
	float BoardCentreX = ((MaxBoardBounds.X - MinBoardBounds.X) / 2) * BoardWorldScale.X;
	float BoardCentreY = ((MaxBoardBounds.Y - MinBoardBounds.Y) / 2) * BoardWorldScale.Y;

	return FVector (BoardCentreX, BoardCentreY, 0.0f);
}

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board Mesh"));
	RootComponent = BoardMesh;

	InitBoard();
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	SetBoardBounds();
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

