// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Food"));
	RootComponent = FoodMesh;

	FoodMesh->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFood::OnOverlap()
{
	Destroy();
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
