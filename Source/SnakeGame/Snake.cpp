// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "SnakeGame/BodySegment.h"
#include "Components/ArrowComponent.h"
#include "SnakeGame/Board/Board.h"

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
	/*MinSnakeHeadBounds /= 20.0f;
	MaxSnakeHeadBounds /= 20.0f;*/

}

void ASnake::MoveUp(float Value)
{
	if(!BoardRef)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot access Min/MaxBoardBounds due to BoardRef not being assigned"));
		return;
	}

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

void ASnake::SpawnFirstSegment() 
{
	// TODO - Maybe use GetForwardVector() for this function?
	
	if(!SegmentClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SEGMENT CLASS NOT ASSIGNED"));
		return;
	}
	// !TempSegment means that it can only spawn once
	if(SegmentClass && SegmentArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed - SPAWNING FIRST SEGMENT"));

		FVector SpawnLocation;
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
			// TODO - Fix bug where when SnakeHead vector Y == -1, the SpawnLocation appears to be random, all others work fine
			SpawnLocation = GetCurrentSnakeHeadLoc();
			SpawnLocation.Y += 100.0f;
			/* Whenever I move the default pawn in the rotation vector of Y = -1 the TempSegment spawn object spawns at relative loc:
			 X = 0.0, Y = 0.0, Z = -9223372036854775808.0. All conditions work as expected (which is directly behind the SnakeHead.
			 Also, if I spawn the actor when moving in the other 3 directions, the actor spawns where I want it to. However, if I do this
			 and then move in the Y = -1 direction, the TempSegment remains static and no longer updates its location to be directly
			 behind the SnakeHead. */
		}
		else if(SnakeHead->GetComponentRotation().Vector().Y == 1)
		{
			SpawnLocation = GetCurrentSnakeHeadLoc();
			SpawnLocation.Y -= 100.0f;
		}
		const FRotator SpawnRotation = SnakeHead->GetComponentRotation();

		FirstSegment = GetWorld()->SpawnActor<ABodySegment>(SegmentClass, SpawnLocation, SpawnRotation);
		FirstSegment->SetOwner(this);
		
		SegmentArray.Push(FirstSegment);
	}
	/*if (SegmentClass && SegmentArray.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed - SPAWNING REST OF SNAKE"));

	}*/
}

FVector ASnake::GetNextSegmentLoc()
{
	FVector SegmentLoc;
	
	for(int32 i = 0; i <= (SegmentArray.Num() - 1); i++)
	{
		SegmentLoc = SegmentArray[i - 1]->GetActorLocation();		
	}

	return SegmentLoc;
}

void ASnake::SetNextSegmentLoc()
{
	if(SegmentArray.Num() == 0)
	{
		return;
	}
	if(SegmentArray.Num() > 0)
	{
		for(int32 i = 0; i <= (SegmentArray.Num() - 1); i++)
		{
			// TODO - Trying to set the most recent segment spawn to be located behind the previously spawned segment
			SegmentArray[i]->SetActorLocation(GetNextSegmentLoc() + ((SegmentArray[i - 1]->GetActorForwardVector() * -1) * 100.0f));
		}		
	}
}

void ASnake::UpdateFirstBodySegmentLoc()
{
	if(!FirstSegment)
	{
		UE_LOG(LogTemp, Error, TEXT("TEMP SEGMENT NOT ASSIGNED"));
		return;
	}
	if(FirstSegment)
	{
		FirstSegment->SetActorLocation(SnakeHead->GetComponentLocation() + (SnakeHead->GetForwardVector() * -1) * 100.0f);
	}
}

void ASnake::UpdateArraySegmentLoc()
{
	if(SegmentArray.Num() > 1)
	{
		// SegmentArray[0]->SetActorLocation()
		for (int32 i = 1; i < SegmentArray.Num(); i++)
		{
			SegmentArray[i]->SetActorLocation(SegmentArray[i - 1]->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Segment loc: %s"), *(SegmentArray[i]->GetActorLocation().ToString()));
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
	//SnakeHead->SetRelativeLocation(SnakeHeadStartingLoc);
	UE_LOG(LogTemp, Warning, TEXT("Min Snake Head Bounds: %s"), *(MinSnakeHeadBounds.ToString()));
	UE_LOG(LogTemp, Warning, TEXT("Max Snake Head Bounds: %s"), *(MaxSnakeHeadBounds.ToString()));
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Snake relative loc: %f"), SnakeHead->GetRelativeLocation().Y);
	
	if(!MovementDirection.IsZero())
	{
		const FVector NewLocation = SnakeHead->GetRelativeLocation() + (MovementDirection * DeltaTime * SnakeHeadSpeed);
		SnakeHead->SetRelativeLocation(NewLocation);
		UpdateFirstBodySegmentLoc();
		/*UpdateArraySegmentLoc();*/
	}
	/*UE_LOG(LogTemp, Warning, TEXT("SnakeHead Loc: %s"), *(SnakeHead->GetRelativeLocation().ToString()));
	UE_LOG(LogTemp, Warning, TEXT("SnakeHead RLoc: %s"), *(SnakeHead->GetComponentRotation().Vector().ToString()));*/
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);

	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ASnake::SpawnFirstSegment);
	
}

