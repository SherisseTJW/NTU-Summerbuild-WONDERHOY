// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"
#include "Parser/headers/hit-object.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

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
void AHitObject::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AHitObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AHitObject::Initialize(beatmap::HitObject* HitObjectArg, beatmap::Coord Loc) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	if (!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. Null Mesh"));
		return;
	}


	UStaticMesh* LoadedMesh; 
	HitObjectType = HitObjectArg->getType();
	switch (HitObjectType) {
		case beatmap::HitObject::ObjectType::HIT_CIRCLE:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			break;
		case beatmap::HitObject::ObjectType::SLIDER:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
			break;
		case beatmap::HitObject::ObjectType::SPINNER:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			break;
		default:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			break;
	}

	if (LoadedMesh) {
		Mesh->SetStaticMesh(LoadedMesh);
	}

	int _Time = HitObjectArg->getTime();
	int StartTime = _Time - OffsetTime;
	int EndTime = _Time + OffsetTime;

	beatComponent->Initialize(StartTime, EndTime, Loc.getX(), Loc.getY());
}
