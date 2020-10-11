// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "SnakeGame/BodySegment.h"
#include "Components/ArrowComponent.h"

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

	Orientation = CreateDefaultSubobject<UArrowComponent>(TEXT("Orientation"));
	Orientation->SetupAttachment(SnakeHead);
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

void ASnake::SpawnFirstSegment() 
{
	if(!SegmentClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SEGMENT CLASS NOT ASSIGNED"));
		return;
	}
	// !TempSegment means that it can only spawn once
	if(SegmentClass && !TempSegment)
	{
		UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed"));

		// TODO - Fix bug where when SnakeHead vector Y == -1, the SpawnLocation appears to be random, all others work fine
		
		FVector SpawnLocation{};
		if(SnakeHead->GetComponentRotation().Vector().X == -1)
		{
			SpawnLocation = GetCurrentSnakeHeadLoc();
			SpawnLocation.X += 100.0f;
		}
		else if(SnakeHead->GetComponentRotation().Vector().X == 1)
		{
			SpawnLocation = GetCurrentSnakeHeadLoc();
			SpawnLocation.X -= 100.0f;
		}
		else if(SnakeHead->GetComponentRotation().Vector().Y == -1)
		{
			SpawnLocation = GetCurrentSnakeHeadLoc();
			SpawnLocation.Y += 100.0f;
		}
		else if(SnakeHead->GetComponentRotation().Vector().Y == 1)
		{
			SpawnLocation = GetCurrentSnakeHeadLoc();
			SpawnLocation.Y -= 100.0f;
		}
		const FRotator SpawnRotation = SnakeHead->GetComponentRotation();

		TempSegment = GetWorld()->SpawnActor<ABodySegment>(SegmentClass, SpawnLocation, SpawnRotation);
		TempSegment->SetOwner(this);
	}	
}

void ASnake::UpdateFirstBodySegmentLoc()
{
	if(!TempSegment)
	{
		UE_LOG(LogTemp, Error, TEXT("SEGMENT CLASS NOT ASSIGNED"));
		return;
	}
	if(TempSegment)
	{
		TempSegment->SetActorRotation(SnakeHead->GetComponentRotation());
		const FVector LeftSnakeHeadLoc{GetCurrentSnakeHeadLoc().X + 100.0f, GetCurrentSnakeHeadLoc().Y, GetCurrentSnakeHeadLoc().Z};
		const FVector RightSnakeHeadLoc{GetCurrentSnakeHeadLoc().X - 100.0f, GetCurrentSnakeHeadLoc().Y, GetCurrentSnakeHeadLoc().Z};
		// TODO - Same bug as spawn loc - Fix bug where when SnakeHead vector Y == -1, the SpawnLocation appears to be random, all others work fine
		const FVector UpSnakeHeadLoc{GetCurrentSnakeHeadLoc().X, GetCurrentSnakeHeadLoc().Y + 100.0f, GetCurrentSnakeHeadLoc().Z};
		const FVector DownSnakeHeadLoc{GetCurrentSnakeHeadLoc().X, GetCurrentSnakeHeadLoc().Y - 100.0f, GetCurrentSnakeHeadLoc().Z};
	
		if(SnakeHead->GetComponentRotation().Vector().X == -1)
		{
			TempSegment->SetActorLocation(LeftSnakeHeadLoc);
		}
		else if(SnakeHead->GetComponentRotation().Vector().X == 1)
		{
			TempSegment->SetActorLocation(RightSnakeHeadLoc);
		}
		else if(SnakeHead->GetComponentRotation().Vector().Y == -1)
		{
			TempSegment->SetActorLocation(UpSnakeHeadLoc);
		}
		else if(SnakeHead->GetComponentRotation().Vector().Y == 1)
		{
			TempSegment->SetActorLocation(DownSnakeHeadLoc);
		}
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

}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFirstBodySegmentLoc();
	if(!MovementDirection.IsZero())
	{
		const FVector NewLocation = SnakeHead->GetRelativeLocation() + (MovementDirection * DeltaTime * SnakeHeadSpeed);
		SnakeHead->SetRelativeLocation(NewLocation);
	}
	UE_LOG(LogTemp, Warning, TEXT("SnakeHead Loc: %s"), *(SnakeHead->GetRelativeLocation().ToString()));
	UE_LOG(LogTemp, Warning, TEXT("SnakeHead RLoc: %s"), *(SnakeHead->GetComponentRotation().Vector().ToString()));
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);

	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ASnake::SpawnFirstSegment);
	
}

