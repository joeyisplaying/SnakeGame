// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodySegment.generated.h"

UCLASS()
class SNAKEGAME_API ABodySegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABodySegment();

	// Called every frame
	virtual void Tick(float DeltaTime) override;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Segment Mesh")
		UStaticMeshComponent* SegmentMesh;

};
