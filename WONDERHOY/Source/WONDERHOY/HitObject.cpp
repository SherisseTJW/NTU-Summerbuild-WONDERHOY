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
	Mesh->SetMobility(EComponentMobility::Movable);
	RootComponent = Mesh;

	splineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
    splineMeshComponent->SetupAttachment(RootComponent);
	splineMeshComponent->SetMobility(EComponentMobility::Movable);

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

void AHitObject::Initialize(beatmap::HitObject* HitObjectArg) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	if (!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. Null Mesh"));
		return;
	}

	int Time = HitObjectArg->getTime();
	beatmap::Coord BeatCoords = HitObjectArg->getCoords();
	beatmap::HitObject::ObjectType BeatType = HitObjectArg->getType();

	switch (BeatType) {
	case beatmap::HitObject::ObjectType::HIT_CIRCLE:
			//RenderHitCircle();
			break;
	case beatmap::HitObject::ObjectType::SLIDER:
			RenderSlider(HitObjectArg);
			break;
	case beatmap::HitObject::ObjectType::SPINNER:
			//RenderSpinner(HitObjectArg);
			break;
	}
}

void AHitObject::RenderHitCircle() {
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
}
void AHitObject::RenderSlider(beatmap::HitObject* SliderHitObject) {
		beatmap::HitObject::CurveType CurveType = SliderHitObject->getCurveType();

		switch (CurveType) {
			case beatmap::HitObject::CurveType::LINEAR:
				//AHitObject::RenderSliderHitCircle(SliderHitObject);
				break;
			case beatmap::HitObject::CurveType::BEZIER:
				AHitObject::RenderSliderBezier(SliderHitObject);
				break;
			case beatmap::HitObject::CurveType::PERFECT_CIRCLE:
				//AHitObject::RenderSliderPerfectCircle(SliderHitObject);
				break;
			case beatmap::HitObject::CurveType::CENTRIPETAL:
				//AHitObject::RenderSliderCentripetal(SliderHitObject);
				break;
		}
}

void AHitObject::RenderSpinner(beatmap::HitObject* SpinnerHitObject) {
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
}


void AHitObject::RenderSliderHitCircle(beatmap::HitObject* SliderHitObject) {
	Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.1f)); // Flat cylinder for linear slider
	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (LoadedMesh) {
		Mesh->SetStaticMesh(LoadedMesh);
	}
};

void AHitObject::RenderSliderBezier(beatmap::HitObject* SliderHitObject) {
	std::vector<beatmap::Coord> AnchorPoints = SliderHitObject->getAnchorPoints();

	if (!splineMeshComponent) {
		UE_LOG(LogTemp, Warning, TEXT("SplineMeshComponent is null in RenderSliderBezier."));
		return;
	}

	int Time = SliderHitObject->getTime();
	for (int i = 1; i < AnchorPoints.size(); i++) {
		beatmap::Coord prev = AnchorPoints[i - 1];
		beatmap::Coord cur = AnchorPoints[i];

		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		USplineMeshComponent* Segment = NewObject<USplineMeshComponent>(this);
		Segment->RegisterComponent(); 
		Segment->SetMobility(EComponentMobility::Movable);
		Segment->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		Segment->SetStaticMesh(LoadedMesh); // Set the cube mesh asset
		Segment->SetStartAndEnd(
			FVector(prev.getX(), 500.0f, prev.getY()),
			FVector(cur.getX(), 500.0f, cur.getY()),
			FVector::ZeroVector, FVector::ZeroVector
		);

		int _Time = SliderHitObject->getTime();
		int StartTime = _Time - OffsetTime;
		int EndTime = _Time + OffsetTime;
		beatComponent->Initialize(StartTime, EndTime, prev.getX(), prev.getY());
	}


	/*
	int Time = SliderHitObject->getTime();
	for (int i = 0; i < AnchorPoints.size(); i++) {
		beatmap::Coord point = AnchorPoints[i];
		float x = point.getX();
		float y = point.getY();

		FVector CurPoint(point.getX(), 500.0f, point.getY());
		splineComponent->AddSplinePoint(CurPoint, ESplineCoordinateSpace::World);
		splineComponent->SetTangentAtSplinePoint(i, FVector(50, 50, 0), ESplineCoordinateSpace::World);

		int StartTime = Time - OffsetTime;
		int EndTime = Time + OffsetTime;
		beatComponent->Initialize(StartTime, EndTime, x, y);
	}
	splineComponent->SetClosedLoop(true); // Close the loop for bezier curve;

	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (LoadedMesh) {
		Mesh->SetStaticMesh(LoadedMesh);
	}
	*/
};

void AHitObject::RenderSliderPerfectCircle(beatmap::HitObject* SliderHitObject) {
	return;
	/*
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
*/
};

void AHitObject::RenderSliderCentripetal(beatmap::HitObject* SliderHitObject) {
	return;
	/*
		UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		if (LoadedMesh) {
			Mesh->SetStaticMesh(LoadedMesh);
		}
*/
};
