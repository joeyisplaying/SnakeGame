// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "SnakeGame/BodySegment.h"
#include "Components/ArrowComponent.h"
#include "SnakeGame/Board/Board.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#define OUT

void ASnake::InitSnakeHead()
{
	SnakeHead->SetRelativeRotation(SnakeHeadStartingRot);
	SnakeHead->SetWorldScale3D(SnakeHeadWorldScale);

	ArrowComponent->SetRelativeRotation(SnakeHead->GetRelativeRotation());
}

void ASnake::InitCamera()
{
	Camera->SetWorldLocation(CameraStartingLoc);
	Camera->SetWorldRotation(CameraStartingRot);
	Camera->SetWorldScale3D(CameraWorldScale);
}

void ASnake::SetSnakeHeadBounds()
{
	SnakeHead->GetLocalBounds(OUT MinSnakeHeadBounds, OUT MaxSnakeHeadBounds);
}

void ASnake::MoveUp(float Value)
{
	if(SnakeHead->GetRelativeLocation().Y >= 50.0f)
	{
		MovementDirection.Y = 1.0f;
	}

	MovementDirection.Y = FMath::Clamp(Value, -1.0f, 1.0f);	
		
	if(Value == -1.0f)
	{
		SnakeHead->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	if(Value == 1.0f)
	{
		SnakeHead->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
}

void ASnake::MoveAcross(float Value)
{
	if(!BoardRef)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot access Min/MaxBoardBounds due to BoardRef not being assigned"));
		return;
	}

	MovementDirection.X = FMath::Clamp(Value, -1.0f, 1.0f);

	if(Value == -1.0f)
	{
		SnakeHead->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	if(Value == 1.0f)
	{
		SnakeHead->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

FVector ASnake::GetCurrentSnakeHeadLoc() const
{
	return SnakeHead->GetComponentLocation();
}

// Spawns snake segment (currently when Space Bar is pressed)
void ASnake::SpawnSegment() 
{	
	if(!SegmentClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SEGMENT CLASS NOT ASSIGNED"));
		return;
	}
	
	if(SegmentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed - SPAWNING FIRST SEGMENT"));

		const FVector SpawnLocation = SnakeHead->GetComponentLocation();
		const FRotator SpawnRotation = SnakeHead->GetComponentRotation();

		TailSegment = GetWorld()->SpawnActor<ABodySegment>(SegmentClass, SpawnLocation, SpawnRotation);
		TailSegment->SetOwner(this);

		if (TailSegment)
		{
			TailSegmentArray.Add(TailSegment);
		}
		if(!TailSegment)
		{
			UE_LOG(LogTemp, Error, TEXT("TailSegment not being spawned!"));
			return;
		}
	}
}

// Updates the location of the segment relative to the head of the snake
void ASnake::UpdateTailSegmentLoc()
{
	if(!TailSegment)
	{
		return;
	}
	if(TailSegment)
	{		
		for (int32 i = 0; i < TailSegmentArray.Num(); i++)
		{
			const FVector CurrentSectionLoc = TailSegmentArray[i]->GetActorLocation();
			TailSegmentArray[i]->SetActorLocation(PreviousTailSegmentLoc);
			PreviousTailSegmentLoc = CurrentSectionLoc;
		}
	}
}

// Sets default values
ASnake::ASnake()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SnakeHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snake Head"));
	RootComponent = SnakeHead;
	
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SnakeHead);
		
	InitSnakeHead();
	InitCamera();	
}

// Called when the game starts or when spawned

void ASnake::BeginPlay()
{
	Super::BeginPlay();
	SetSnakeHeadBounds();
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PreviousTailSegmentLoc = SnakeHead->GetComponentLocation();
	
	if(!MovementDirection.IsZero())
	{
		const FVector NewLocation = SnakeHead->GetRelativeLocation() + (MovementDirection * DeltaTime * SnakeHeadSpeed);
		SnakeHead->SetRelativeLocation(NewLocation);

		UpdateTailSegmentLoc();
	}
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);

	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ASnake::SpawnSegment);	
}