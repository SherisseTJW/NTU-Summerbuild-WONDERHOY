// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Parser/headers/hit-object.h"
#include "LevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WONDERHOY_API ALevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALevelGameMode();
	
protected:
	virtual void BeginPlay() override;

public:
	void RenderHitCircle(beatmap::HitObject* HitCircleObject);
	void RenderSlider(beatmap::HitObject* SliderHitObject);
	void RenderSpinner(beatmap::HitObject* SpinnerHitObject);
};
