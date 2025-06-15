// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"

// Sets default values
AHitObject::AHitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded()) {
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	beatComponent = CreateDefaultSubobject<UBeatComponent>(TEXT("beatComponent"));
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("BeatComponent not created successfully"));
	}
}

// Called when the game starts or when spawned
void AHitObject::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Spawned"));

	if (beatComponent) {
		beatComponent->Initialize(5.0f, 10.0f, 20.0f, 20.0f);
	}
}

// Called every frame
void AHitObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

