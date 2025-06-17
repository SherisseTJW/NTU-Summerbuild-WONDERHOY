// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"
#include "Parser/headers/parser.h"
#include "Parser/headers/util.h"
#include "HitObject.h"

#include <string>

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

    beatmap::Beatmap BeatMap = beatmap::parseBeatmap(BeatmapPath);
	std::vector<beatmap::HitObject> HitObjects = BeatMap.getHitObjects();

	UE_LOG(LogTemp, Warning, TEXT("BeatMap has %d HitObjects"), HitObjects.size());

	for (beatmap::HitObject curHitObject : HitObjects) {
		int Time = curHitObject.getTime();
		beatmap::Coord BeatMapCoords = curHitObject.getCoords();

		AHitObject* HitObjectActor = GetWorld()->SpawnActor<AHitObject>(AHitObject::StaticClass());
		HitObjectActor->Initialize(Time, BeatMapCoords.getX(), BeatMapCoords.getY());
	}
}
