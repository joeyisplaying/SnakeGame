// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* --- Mesh Component --- */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Food Mesh")
		UStaticMeshComponent* FoodMesh{nullptr};
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnOverlap();

};
