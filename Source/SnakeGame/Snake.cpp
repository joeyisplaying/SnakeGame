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
	// SegmentArray.Num() == 0 means that it can only spawn once
	if(SegmentClass && SegmentArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed - SPAWNING FIRST SEGMENT"));

		FVector SpawnLocation = SnakeHead->GetComponentLocation() + ((SnakeHead->GetForwardVector() * -1) * 100.0f);
		FRotator SpawnRotation = SnakeHead->GetComponentRotation();

		FirstSegment = GetWorld()->SpawnActor<ABodySegment>(SegmentClass, SpawnLocation, SpawnRotation);
		FirstSegment->SetOwner(this);
		
		SegmentArray.Push(FirstSegment);
	}
	else
	{
		FVector SpawnLocation;
		FRotator SpawnRotation;
		UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed - SPAWNING NEXT SEGMENT"));

		if(SegmentArray.Num() == 1)
		{
			SpawnLocation = GetPreviousSegmentLoc() + ((SegmentArray[0]->GetActorForwardVector() * -1) * 100.0f);
			SpawnRotation = SegmentArray[0]->GetActorRotation();
			SegmentIndex++;
		}
		else
		{
			SpawnLocation = GetPreviousSegmentLoc() + ((SegmentArray[SegmentIndex]->GetActorForwardVector() * -1) * 100.0f);
			SpawnRotation = SegmentArray[SegmentIndex]->GetActorRotation();
			SegmentIndex++;
		}

		NextSegment = GetWorld()->SpawnActor<ABodySegment>(SegmentClass, SpawnLocation, SpawnRotation);
		NextSegment->SetOwner(this);

		SegmentArray.Push(NextSegment);
	}
}

// Updates the location of the first segment relative to the head of the snake
void ASnake::UpdateFirstBodySegmentLoc()
{
	if(!FirstSegment)
	{
		return;
	}
	if(FirstSegment)
	{
		// This updates the first segment's loc to always be 100 units behind the head
		FirstSegment->SetActorLocation(SnakeHeadTickLoc);
		UE_LOG(LogTemp, Warning, TEXT("First Segment loc: %s"), *(FirstSegment->GetActorLocation().ToString()));
	}
}

// Updates the rotation of the first segment relative to the head of the snake
void ASnake::UpdateFirstBodyRotation()
{
	if(!FirstSegment)
	{
		return;
	}
	if(FirstSegment)
	{
		FirstSegment->SetActorRotation(SnakeHead->GetRelativeRotation());
	}
}

// Updates the rotation of each segment relative to the previous segment in the snake
void ASnake::UpdateNextSegmentRotation()
{
	if(!NextSegment)
	{
		return;
	}
	if(NextSegment && SegmentArray.Num() >= 2)
	{
		for(int32 i = 1; i <= (SegmentArray.Num() - 1); i++)
		{
			FRotator PreviousSegmentRotation = SegmentArray[i - 1]->GetActorRotation();
			
			SegmentArray[i]->SetActorRotation(PreviousSegmentRotation);
		}		
	}
}

// Updates the location of each segment relative to the previous segment in the snake
void ASnake::UpdateNextSegmentLoc()
{
	if(!NextSegment)
	{
		return;
	}
	if(NextSegment && SegmentArray.Num() > 1)
	{
		for(int32 i = 1; i <= (SegmentArray.Num() - 1); i++)
		{
			FVector PreviousSegmentLoc = SegmentArray[i - 1]->GetActorLocation();

			/* 100.0f is the width/height of each actor, so currently the new segments are being spawned in the location
			of the previous segment + 100 units */
			
			SegmentArray[i]->SetActorLocation(NextSegmentTickLoc);
		}	
	}	
}

FVector ASnake::GetPreviousSegmentLoc()
{
	FVector PreviousSegmentLoc;
	// If SegmentArray.Num() == 1, then the FirstSegment has been added to the SegmentArray
	if(SegmentArray.Num() == 1)
	{
		PreviousSegmentLoc = SegmentArray[SegmentArray.Num() - 1]->GetActorLocation();
	}
	
	// If SegmentArray.Num() >= 2, then at least 1 NextSegment spawned actor has been added to the SegmentArray
	if(SegmentArray.Num() >= 2)
	{
		PreviousSegmentLoc = SegmentArray[SegmentArray.Num() - 2]->GetActorLocation();
	}
	
	return PreviousSegmentLoc;
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

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint"));
		
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
	
	if(!MovementDirection.IsZero())
	{
		OldHeadLoc = SnakeHead->GetRelativeLocation();

		const FVector NewLocation = SnakeHead->GetRelativeLocation() + (MovementDirection * DeltaTime * SnakeHeadSpeed);
		SnakeHead->SetRelativeLocation(NewLocation);
		
		/*UpdateFirstBodySegmentLoc();
		UpdateFirstBodyRotation();*/

		// FirstSegmentPos = NewHeadPos - Difference between OldHeadPos and NewHeadPos(NewHeadPos - OldHeadPos)
		// NextSegmentPos = Seg[i - 1].NewPos - Difference between (Seg[i - 1].NewPos - Seg[i - 1].OldPos)
		// Variables: NewHeadPos; OldHeadPos;

		if(FirstSegment)
		{
			NewHeadLoc = SnakeHead->GetComponentLocation();

			// NewFirstSegmentLoc = NewSnakeHeadLoc - the difference between the NewSnakeHeadLoc and the OldSnakeHeadLoc
			FVector NewFirstSegmentLoc = NewHeadLoc - (NewHeadLoc - OldHeadLoc);
		
			SegmentArray[0]->SetActorLocation(NewFirstSegmentLoc);

			for (int32 i = 1; i < SegmentArray.Num(); i++)
			{
				OldFirstSegmentPos = SegmentArray[i - 1]->GetActorLocation();
				// The element number of the position array correlates with the element number of the segment array.
				// I.e. The old position of SegmentArray[0] is the FVector OldFirstSegmentPosArray[0].
				OldFirstSegmentPosArray.Push(OldFirstSegmentPos);
			}
			for (int32 i = 1; i < SegmentArray.Num(); i++)
			{				
				SegmentArray[i]->SetActorLocation(OldFirstSegmentPosArray[i]);
			}
		}
		/*
		UpdateNextSegmentLoc();
		UpdateNextSegmentRotation();*/
	}

	// TODO - Maybe use UPhysicsConstraintComponent to chain actors together? GAME CRASHING IN CURRENT STATE!
	/*if(FirstSegment)
	{
		PhysicsConstraint->SetConstrainedComponents(SnakeHead, "Snake Head", FirstSegment->SegmentMesh, "First Segment");
	}
	if(NextSegment)
	{
		FirstSegment->SegmentPhysicsConstraint->SetConstrainedComponents(FirstSegment->SegmentMesh, "First Segment", NextSegment->SegmentMesh, "NextSegment");
	}*/
	
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);

	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ASnake::SpawnSegment);
	
}

