// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Overlapable.h"
#include "Fruit.generated.h"

class StaticMeshComponent;

UCLASS()
class SNAKEGAME_API AFruit : public AOverlapable
{
	GENERATED_BODY()
public:
	AFruit();


	UFUNCTION()
        void OnOverlap();
	
	void OverlappedWithSnakeHead(ASnake* SnakeHeadPtr) override;

	void SpawnFood();
	
};
