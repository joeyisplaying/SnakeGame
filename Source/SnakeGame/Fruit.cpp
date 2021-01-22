// Fill out your copyright notice in the Description page of Project Settings.


#include "Fruit.h"

#include "Snake.h"

AFruit::AFruit()
{
	
}

void AFruit::OverlappedWithSnakeHead(ASnake* SnakeHeadPtr)
{
	SnakeHeadPtr->SnakeEatsFood();
}

void AFruit::OnOverlap()
{
	AActor* MyOwner = GetOwner();
	
	if(!MyOwner) {return;}

	// Destroys this actor if OnOverlap() is called
	Destroy();

}


