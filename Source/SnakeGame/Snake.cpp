// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"
#include "Fruit.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "SnakeGame/BodySegment.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"

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
		if(TailSegmentArray.Num() == 0)
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

		else if (TailSegmentArray.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Space Bar Pressed - SPAWNING REST OF SEGMENTS"));

			int32 i = TailSegmentArray.Num();

			const FVector SpawnLocation = TailSegmentArray[i - 1]->GetActorLocation();
			const FRotator SpawnRotation = TailSegmentArray[i - 1]->GetActorRotation();

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
}

void ASnake::SpawnFruit()
{
	if(!FruitClass)
	{
		UE_LOG(LogTemp, Error, TEXT("FoodClass NOT ASSIGNED"));
		return;
	}
	if(FruitClass)
	{
		const float RandX = FMath::RandRange(0.0f, 2000.0f);
		const float RandY = FMath::RandRange(0.0f, 2000.0f);
		
		const FVector SpawnLocation = FVector(RandX, RandY, 50.0f);
		const FRotator SpawnRotation = SnakeHead->GetComponentRotation();

		Fruit = GetWorld()->SpawnActor<AFruit>(FruitClass, SpawnLocation, SpawnRotation);
		Fruit->SetOwner(this);
	}

	// TODO: Delete and spawn new food when overlap with snake
	// TODO: Spawn snake segments when overlap with food
}

void ASnake::SnakeEatsFood()
{
	UE_LOG(LogTemp, Warning, TEXT("Snake has eaten fruit"));
	
	if(!Fruit)
	{
		UE_LOG(LogTemp, Error, TEXT("Food not assigned to an actor!"));
		return;
	}
	if (!Snake)
	{
		UE_LOG(LogTemp, Error, TEXT("Snake not assigned to an actor!"));
		return;
	}

	if(Snake->IsOverlappingActor(Fruit->GetOwner()))
	{
		UE_LOG(LogTemp, Error, TEXT("OVERLAP OCCURED"));
		Fruit->OnOverlap();
		SpawnFruit();
		SpawnSegment();
	}

	// TODO: When the Snake head and the food overlap this function is not firing - not sure why
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

void ASnake::MoveUp()
{
	MoveDir = FVector(0.0, -1.0, 0.0);
}

void ASnake::MoveDown()
{
	MoveDir = FVector(0.0, 1.0, 0.0);
}

void ASnake::MoveLeft()
{
	MoveDir = FVector(-1.0, 0.0, 0.0);
}

void ASnake::MoveRight()
{
	MoveDir = FVector(1.0, 0.0, 0.0);
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

	SnakeHead->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SnakeHead->SetCollisionObjectType(ECC_Pawn);
	SnakeHead->SetCollisionResponseToAllChannels(ECR_Overlap);

}

// Called when the game starts or when spawned

void ASnake::BeginPlay()
{
	Super::BeginPlay();

	Snake = FindObject<ASnake>(GetWorld()->GetCurrentLevel(), TEXT("BP_Snake_2"));
	Cube = FindObject<AActor>(GetWorld()->GetCurrentLevel(), TEXT("Cube_2"));

	TickSpeed = 0.5f;
	PrimaryActorTick.TickInterval = TickSpeed;

	SpawnFruit();
	SetSnakeHeadBounds();
	MoveStepSize = SnakeHead->GetStaticMesh()->GetBoundingBox().GetSize().X;

	
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Snake->IsOverlappingActor(Fruit))
	{
		UE_LOG(LogTemp, Error, TEXT("OVERLAP OCCURED"));
		Fruit->OnOverlap();
		SpawnFruit();
		SpawnSegment();
	}

	PreviousTailSegmentLoc = SnakeHead->GetComponentLocation();
	AddActorWorldOffset(MoveDir * MoveStepSize);
	UpdateTailSegmentLoc();
	
	/*if(!MovementDirection.IsZero())
	{
		const FVector NewLocation = SnakeHead->GetRelativeLocation() + (MovementDirection * DeltaTime * SnakeHeadSpeed);
		SnakeHead->SetRelativeLocation(NewLocation);

		UpdateTailSegmentLoc();
	}*/
}

// Called to bind functionality to input
void ASnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// PlayerInputComponent->BindAxis("MoveUp",this, &ASnake::MoveUp);
	// PlayerInputComponent->BindAxis("MoveAcross",this, &ASnake::MoveAcross);

	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ASnake::SpawnSegment);
	
	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &ASnake::MoveUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &ASnake::MoveDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &ASnake::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &ASnake::MoveRight);
}