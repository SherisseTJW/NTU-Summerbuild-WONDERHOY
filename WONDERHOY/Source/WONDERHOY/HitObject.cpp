// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"

// Sets default values
AHitObject::AHitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AHitObject::BeginPlay()
{
	Super::BeginPlay();

	beatComponent = CreateDefaultSubobject<UBeatComponent>(TEXT("BeatComponent"));
	beatComponent->Initialize(2.0f, 4.0f, 2.0f, 2.0f);
	AddOwnedComponent(beatComponent);
}

// Called every frame
void AHitObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

