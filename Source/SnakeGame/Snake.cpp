// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

void ASnake::InitBoard() const
{
	Board->SetWorldLocation(BoardWorldLoc);
	Board->SetWorldRotation(BoardWorldRot);
	Board->SetWorldScale3D(BoardWorldScale);
}

void ASnake::InitSnakeHead() const
{
	SnakeHead->SetRelativeLocation(SnakeHeadStartingLoc);
	SnakeHead->SetRelativeRotation(SnakeHeadStartingRot);
	SnakeHead->SetWorldScale3D(SnakeHeadWorldScale);
}

void ASnake::InitCamera() const
{
	Camera->SetRelativeLocation(CameraStartingLoc);
	Camera->SetRelativeRotation(CameraStartingRot);
	Camera->SetWorldScale3D(CameraWorldScale);
}

void ASnake::MoveUp(float Value)
{
	MovementDirection.Y = FMath::Clamp(Value, -1.0f, 1.0f);	
}

void ASnake::MoveAcross(float Value)
{
	MovementDirection.X = FMath::Clamp(Value, -1.0f, 1.0f);	
}

// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Board = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));
	RootComponent = Board;

	SnakeHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snake Head"));
	SnakeHead->SetupAttachment(Board);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Board);

	InitBoard();
	InitSnakeHead();
	InitCamera();
	
}

// Called when the game starts or when spawned

void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!MovementDirection.IsZero())
	{
		const FVector NewLocation = SnakeHead->GetRelativeLocation() + (MovementDirection * DeltaTime * SnakeHeadSpeed);
		SnakeHead->SetRelativeLocation(NewLocation);
	}
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);
}

