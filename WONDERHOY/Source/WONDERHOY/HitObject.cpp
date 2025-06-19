// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"
#include "Parser/headers/hit-object.h"

// Sets default values
AHitObject::AHitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	beatComponent = CreateDefaultSubobject<UBeatComponent>(TEXT("beatComponent"));
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("BeatComponent not created successfully"));
	}
}

// Called when the game starts or when spawned
void AHitObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHitObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitObject::Initialize(beatmap::HitObject HitObjectArg) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	if (!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. Null Mesh"));
		return;
	}

	int Time = HitObjectArg.getTime();
	beatmap::Coord BeatCoords = HitObjectArg.getCoords();
	beatmap::ObjectType BeatType = HitObjectArg.getType();

	switch (BeatType) {
		case beatmap::ObjectType::HIT_CIRCLE:
			RenderHitCircle();
			break;
		case beatmap::ObjectType::SLIDER:
			RenderSlider(HitObjectArg);
			break;
		case beatmap::ObjectType::SPINNER:
			RenderSpinner(HitObjectArg);
			break;
	}

	int StartTime = Time - OffsetTime;
	int EndTime = Time + OffsetTime;
	beatComponent->Initialize(StartTime, EndTime, BeatCoords.getX(), BeatCoords.getY());
}

void AHitObject::RenderHitCircle() {
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
}
void AHitObject::RenderSlider(beatmap::Slider SliderHitObject) {
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
}
void AHitObject::RenderSpinner(beatmap::Spinning SpinnerHitObject) {
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
}
