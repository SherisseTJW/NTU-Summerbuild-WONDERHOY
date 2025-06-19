// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"
#include "Parser/headers/hit-object.h"

// Sets default values
AHitObject::AHitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded()) {
		Mesh->SetStaticMesh(CubeMesh.Object);
	}*/

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

void AHitObject::Initialize(int TimeArg, int CoordXArg, int CoordYArg, beatmap::ObjectType ObjectTypeArg) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(RootComponent);
	if (!Mesh) return;

	FString Path;
	switch (ObjectTypeArg) {
		case beatmap::ObjectType::HIT_CIRCLE:
			Path = TEXT("/Engine/BasicShapes/Cube.Cube");
			break;
		case beatmap::ObjectType::SLIDER:
			Path = TEXT("/Engine/BasicShapes/Sphere.Sphere");
			break;
		case beatmap::ObjectType::SPINNER:
			Path = TEXT("/Engine/BasicShapes/Cylinder.Cylinder");
			break;
	}

	if (!Path.IsEmpty()) {
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, *Path);
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
	}

	int StartTime = TimeArg - OffsetTime;
	int EndTime = TimeArg + OffsetTime;
	beatComponent->Initialize(StartTime, EndTime, CoordXArg, CoordYArg);
}
