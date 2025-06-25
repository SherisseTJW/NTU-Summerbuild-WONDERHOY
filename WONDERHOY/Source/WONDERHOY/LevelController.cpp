// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"

void ALevelController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAxis("Turn", this, &ALevelController::Turn);
    InputComponent->BindAxis("LookUp", this, &ALevelController::LookUp);
}

void ALevelController::Turn(float Rate) { }

void ALevelController::LookUp(float Rate) {  }
