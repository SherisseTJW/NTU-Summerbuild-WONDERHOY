// Fill out your copyright notice in the Description page of Project Settings.  

#include "MenuGameMode.h"
#include "LevelController.h"
#include "LevelDefaultPawn.h"

#include "Kismet/GameplayStatics.h"

AMenuGameMode::AMenuGameMode() {
	PlayerControllerClass = ALevelController::StaticClass(); 
	DefaultPawnClass = ALevelDefaultPawn::StaticClass();
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString NumMiss = UGameplayStatics::ParseOption(OptionsString, TEXT("NumMiss"));
    FString NumBad = UGameplayStatics::ParseOption(OptionsString, TEXT("NumBad"));
    FString NumGood = UGameplayStatics::ParseOption(OptionsString, TEXT("NumGood"));
    FString NumGreat = UGameplayStatics::ParseOption(OptionsString, TEXT("NumGreat"));
    FString NumPerfect = UGameplayStatics::ParseOption(OptionsString, TEXT("NumPerfect"));

	UE_LOG(LogTemp, Warning, TEXT("MenuGameMode::BeginPlay - NumMiss: %s, NumBad: %s, NumGood: %s, NumGreat: %s, NumPerfect: %s"), 
		*NumMiss, *NumBad, *NumGood, *NumGreat, *NumPerfect);
}
