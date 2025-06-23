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

void AHitObject::Initialize(beatmap::HitObject* HitObjectArg, beatmap::Coord Loc, bool IsNormalHitCircle) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	if (!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. Null Mesh"));
		return;
	}


	UStaticMesh* LoadedMesh; 
	if (IsNormalHitCircle) {
		LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	}
	else {
		LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	}

	if (LoadedMesh) {
		Mesh->SetStaticMesh(LoadedMesh);
	}

	int _Time = HitObjectArg->getTime();
	int StartTime = _Time - OffsetTime;
	int EndTime = _Time + OffsetTime;
	//beatmap::Coord Loc = HitObjectArg->getCoords();

	beatComponent->Initialize(StartTime, EndTime, Loc.getX(), Loc.getY());

	//int Time = HitObjectArg->getTime();
	//beatmap::Coord BeatCoords = HitObjectArg->getCoords();
	//beatmap::HitObject::ObjectType BeatType = HitObjectArg->getType();

	//switch (BeatType) {
	//case beatmap::HitObject::ObjectType::HIT_CIRCLE:
	//		RenderHitCircle(HitObjectArg);
	//		break;
	//case beatmap::HitObject::ObjectType::SLIDER:
	//		RenderSlider(HitObjectArg);
	//		break;
	//case beatmap::HitObject::ObjectType::SPINNER:
	//		//RenderSpinner(HitObjectArg);
	//		break;
	//}
}

void AHitObject::RenderHitCircle(beatmap::HitObject* HitCircleObject) {

}
void AHitObject::RenderSlider(beatmap::HitObject* SliderHitObject) {
		beatmap::HitObject::CurveType CurveType = SliderHitObject->getCurveType();

		switch (CurveType) {
			case beatmap::HitObject::CurveType::LINEAR:
				UE_LOG(LogTemp, Warning, TEXT("Attempting to render LINEAR slider"));
				AHitObject::RenderSliderLinear(SliderHitObject);
				break;
			case beatmap::HitObject::CurveType::BEZIER:
				//AHitObject::RenderSliderBezier(SliderHitObject);
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

void AHitObject::RenderSliderLinear(beatmap::HitObject* SliderHitObject) {
	return;
	/*std::vector<beatmap::Coord> AnchorCoords = SliderHitObject->getAnchorPoints();
	if (AnchorCoords.size() < 2) return;

	TArray<FVector> Points;
	for (beatmap::Coord coord : AnchorCoords) {
		Points.Add(FVector(coord.getX(), 500.0f, coord.getY()));
	}

	USplineComponent* Spline = NewObject<USplineComponent>(this);
	Spline->RegisterComponent();
	Spline->SetMobility(EComponentMobility::Movable);
	Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	for (const FVector& Point : Points) {
		Spline->AddSplinePoint(Point, ESplineCoordinateSpace::Local);
	}

	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	for (int i = 0; i < Points.Num() - 1; i++) {
		USplineMeshComponent* Segment = NewObject<USplineMeshComponent>(this);
		Segment->SetStaticMesh(LoadedMesh);
		Segment->SetMobility(EComponentMobility::Movable);
		Segment->RegisterComponent();
		Segment->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);

		FVector Start = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector End = Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		FVector EndTangent = Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		Segment->SetStartAndEnd(Start, StartTangent, End, EndTangent);
	}

	int _Time = SliderHitObject->getTime();
	int StartTime = _Time - OffsetTime;
	int EndTime = _Time + OffsetTime;

	FVector Head = Points[0];
	beatComponent->Initialize(StartTime, EndTime, Head.X, Head.Y);

	UE_LOG(LogTemp, Warning, TEXT("HitObject Actor placed at: %s"), *GetActorLocation().ToString());*/
}

void AHitObject::RenderSliderBezier(beatmap::HitObject* SliderHitObject) {
	return;
	//std::vector<beatmap::Coord> AnchorPoints = SliderHitObject->getAnchorPoints();

	//if (!splineMeshComponent) {
	//	UE_LOG(LogTemp, Warning, TEXT("SplineMeshComponent is null in RenderSliderBezier."));
	//	return;
	//}

	//int Time = SliderHitObject->getTime();
	//for (int i = 1; i < AnchorPoints.size(); i++) {
	//	beatmap::Coord prev = AnchorPoints[i - 1];
	//	beatmap::Coord cur = AnchorPoints[i];

	//	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	//	USplineMeshComponent* Segment = NewObject<USplineMeshComponent>(this);
	//	Segment->RegisterComponent(); 
	//	Segment->SetMobility(EComponentMobility::Movable);
	//	Segment->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//	Segment->SetStaticMesh(LoadedMesh); // Set the cube mesh asset
	//	Segment->SetStartAndEnd(
	//		FVector(prev.getX(), 500.0f, prev.getY()),
	//		FVector(cur.getX(), 500.0f, cur.getY()),
	//		FVector::ZeroVector, FVector::ZeroVector
	//	);

	//	int _Time = SliderHitObject->getTime();
	//	int StartTime = _Time - OffsetTime;
	//	int EndTime = _Time + OffsetTime;
	//	beatComponent->Initialize(StartTime, EndTime, prev.getX(), prev.getY());
	//}
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
