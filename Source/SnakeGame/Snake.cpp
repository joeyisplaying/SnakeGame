// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

#define OUT

void ASnake::InitBoard()
{
	Board->SetWorldLocation(BoardWorldLoc);
	Board->SetWorldRotation(BoardWorldRot);
	Board->SetWorldScale3D(BoardWorldScale);
}

void ASnake::InitSnakeHead()
{
	SnakeHead->SetRelativeLocation(SnakeHeadStartingLoc);
	SnakeHead->SetRelativeRotation(SnakeHeadStartingRot);
	SnakeHead->SetWorldScale3D(SnakeHeadWorldScale);
}

void ASnake::InitCamera()
{
	Camera->SetRelativeLocation(CameraStartingLoc);
	Camera->SetRelativeRotation(CameraStartingRot);
	Camera->SetWorldScale3D(CameraWorldScale);
}

void ASnake::SetBoardBounds()
{
	Board->GetLocalBounds(OUT MinBoardBounds, OUT MaxBoardBounds);
	
	UE_LOG(LogTemp, Warning, TEXT("MIN = %s"), *(MinBoardBounds.ToString()));
	UE_LOG(LogTemp, Warning, TEXT("MAX = %s"), *(MaxBoardBounds.ToString()));
}

void ASnake::SetSnakeHeadBounds()
{
	SnakeHead->GetLocalBounds(OUT MinSnakeHeadBounds, OUT MaxSnakeHeadBounds);
	MinSnakeHeadBounds /= 20.0f;
	MaxSnakeHeadBounds /= 20.0f;

}

void ASnake::MoveUp(float Value)
{
	// If statements to stop SnakeHead going over edge of board
	if(SnakeHead->GetRelativeLocation().Y <= MinBoardBounds.Y - MinSnakeHeadBounds.Y)
	{
		MovementDirection.Y = FMath::Clamp(Value, 1.0f, 1.0f);	
	}
	else if( SnakeHead->GetRelativeLocation().Y + MaxSnakeHeadBounds.Y >= MaxBoardBounds.Y)
	{
		MovementDirection.Y = FMath::Clamp(Value, -1.0f, -1.0f);	
	}
	else
	{
		MovementDirection.Y = FMath::Clamp(Value, -1.0f, 1.0f);	
	}
	
}

void ASnake::MoveAcross(float Value)
{
	// If statements to stop SnakeHead going over edge of board
	if(SnakeHead->GetRelativeLocation().X <= MinBoardBounds.X - MinSnakeHeadBounds.X)
	{
		MovementDirection.X = FMath::Clamp(Value, 1.0f, 1.0f);
	}
	else if(SnakeHead->GetRelativeLocation().X + MaxSnakeHeadBounds.X >= MaxBoardBounds.X)
	{
		MovementDirection.X = FMath::Clamp(Value, -1.0f, -1.0f);
	}
	else
	{
		MovementDirection.X = FMath::Clamp(Value, -1.0f, 1.0f);	
	}
	
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
	SetBoardBounds();
	SetSnakeHeadBounds();
	UE_LOG(LogTemp, Warning, TEXT("MIN Snake Bounds: %s"), *(MinSnakeHeadBounds.ToString()));
	UE_LOG(LogTemp, Warning, TEXT("MAX Snake Bounds: %s"), *(MaxSnakeHeadBounds.ToString()));
	UE_LOG(LogTemp, Warning, TEXT("MIN Board Bounds: %s"), *(MinBoardBounds.ToString()));
	UE_LOG(LogTemp, Warning, TEXT("MAX Board Bounds: %s"), *(MaxBoardBounds.ToString()));
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
	UE_LOG(LogTemp, Warning, TEXT("MIN Snake bounds: %s"), *(SnakeHead->GetRelativeLocation().ToString()));
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);
}

