// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeatComponent.h"
#include "Parser/headers/hit-object.h"
#include "Parser/headers/parser.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "HitObject.generated.h"

UCLASS()
class WONDERHOY_API AHitObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnMouseOverStart(UPrimitiveComponent* TouchedComp);

	UFUNCTION()
	virtual void OnMouseOverEnd(UPrimitiveComponent* TouchedComp);

	void Initialize(beatmap::HitObject* HitObjectArg, beatmap::Coord Loc, beatmap::Beatmap* BeatmapArg);

public:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* BaseColorMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	UBeatComponent* beatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USplineMeshComponent* splineMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// In ms
	int OffsetTime = 1000;
	int LoadTime = 500;

	float StartMouseOverTime = 0.0f;

	beatmap::Beatmap* Beatmap;
	beatmap::HitObject* HitObject;
	beatmap::HitObject::ObjectType HitObjectType;
};

