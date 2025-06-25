// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelDefaultPawn.h"

// Sets default values
ALevelDefaultPawn::ALevelDefaultPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALevelDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

