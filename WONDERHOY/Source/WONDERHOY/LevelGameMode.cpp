// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"
#include "LevelController.h"
#include "LevelDefaultPawn.h"
#include "Parser/headers/hit-object.h"
#include "Parser/headers/parser.h"
#include "Parser/headers/util.h"
#include "HitObject.h"

#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#include <string>

ALevelGameMode::ALevelGameMode() {
	PlayerControllerClass = ALevelController::StaticClass(); 
	DefaultPawnClass = ALevelDefaultPawn::StaticClass();
}

void ALevelGameMode::BeginPlay()
{
    Super::BeginPlay();

	FString FullLevelName = GetWorld()->GetMapName();
	std::vector<std::string> LevelNameTokens = beatmap::split(TCHAR_TO_UTF8(*FullLevelName), "Lvl_");

	std::string SongName = "Unknown Song";
	if (LevelNameTokens.size() > 1) {
		SongName = LevelNameTokens[1];
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to parse level name: %s.."), *FullLevelName);
	}

	FString ProjectDir = FPaths::ProjectContentDir();
	std::string BeatmapPath = std::string(TCHAR_TO_UTF8(*ProjectDir)) + "Beatmaps/" + SongName + ".osu";
	UE_LOG(LogTemp, Warning, TEXT("Beatmap Path: %s"), *FString(BeatmapPath.c_str()));

	BeatMap = new beatmap::Beatmap(beatmap::parseBeatmap(BeatmapPath));
	std::vector<beatmap::HitObject*> HitObjects = BeatMap->getHitObjects();

	UE_LOG(LogTemp, Warning, TEXT("BeatMap has %d HitObjects"), HitObjects.size());

	std::string AudioFileName = BeatMap->getAudioFileName();
	UE_LOG(LogTemp, Warning, TEXT("Audio file name: "), *FString(AudioFileName.c_str()));


	FString AudioFilePath = ("/Game/Beatmaps/" + AudioFileName + "." + AudioFileName).c_str();
	
	UE_LOG(LogTemp, Warning, TEXT("Attempting to load audio: %s"), *AudioFilePath);
	USoundWave * Sound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, *AudioFilePath));

	if (Sound) {
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), Sound);
		if (AudioComponent) {
			UGameplayStatics::PlaySound2D(GetWorld(), Sound);
			AudioComponent->Play();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn audio component."));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load audio: %s"), *AudioFilePath);
	}

	for (beatmap::HitObject* curHitObject : HitObjects) {
		beatmap::HitObject::ObjectType BeatType = curHitObject->getType();

		switch (BeatType) {
		case beatmap::HitObject::ObjectType::HIT_CIRCLE:
				RenderHitCircle(curHitObject);
				break;
		case beatmap::HitObject::ObjectType::SLIDER:
				RenderSlider(curHitObject);
				break;
		case beatmap::HitObject::ObjectType::SPINNER:
				RenderSpinner(curHitObject);
				break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Loaded %d HitObjects"), loadedHitObjectsCount);
}

void ALevelGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (BeatMap) {
		// Log final judgement first

		delete BeatMap;
		BeatMap = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void ALevelGameMode::RenderHitCircle(beatmap::HitObject* HitCircleObject) {
	AHitObject* HitObjectActor = GetWorld()->SpawnActor<AHitObject>(AHitObject::StaticClass());
	HitObjectActor->Initialize(HitCircleObject, HitCircleObject->getCoords(), BeatMap);

	loadedHitObjectsCount++;
};

void ALevelGameMode::RenderSlider(beatmap::HitObject* SliderHitObject) {
	std::vector<beatmap::Coord> AnchorCoords = SliderHitObject->getAnchorPoints();

	for (beatmap::Coord AnchorCoord : AnchorCoords) {
		AHitObject* HitObjectActor = GetWorld()->SpawnActor<AHitObject>(AHitObject::StaticClass());
		HitObjectActor->Initialize(SliderHitObject, AnchorCoord, BeatMap);

		loadedHitObjectsCount++;
	}
};
void ALevelGameMode::RenderSpinner(beatmap::HitObject* SpinnerHitObject) {
	AHitObject* HitObjectActor = GetWorld()->SpawnActor<AHitObject>(AHitObject::StaticClass());
	HitObjectActor->Initialize(SpinnerHitObject, SpinnerHitObject->getCoords(), BeatMap);

	loadedHitObjectsCount++;
};
