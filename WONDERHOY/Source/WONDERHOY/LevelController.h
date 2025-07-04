// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LevelController.generated.h"

/**
 * 
 */
UCLASS()
class WONDERHOY_API ALevelController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

	void Turn(float Rate);
	void LookUp(float Rate);
};
