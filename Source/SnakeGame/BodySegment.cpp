// Fill out your copyright notice in the Description page of Project Settings.


#include "BodySegment.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ABodySegment::ABodySegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SegmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Segment Mesh"));
	RootComponent = SegmentMesh;

	SegmentPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint"));
}

// Called when the game starts or when spawned
void ABodySegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABodySegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

