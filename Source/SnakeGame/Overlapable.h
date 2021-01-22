// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Overlapable.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ASnake;

UCLASS()
class SNAKEGAME_API AOverlapable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverlapable();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RootComp")
		USceneComponent* RootComp{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* Mesh{nullptr};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void OverlappedWithSnakeHead(ASnake* SnakeHeadPtr);

};
