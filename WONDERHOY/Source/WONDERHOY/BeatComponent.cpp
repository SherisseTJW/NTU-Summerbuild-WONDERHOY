// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatComponent.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include <cmath>

// Sets default values for this component's properties
UBeatComponent::UBeatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBeatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBeatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("BeatComponent does not have an Owner"));
		return;
	}

	float CurrentRunTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	float StartTimeInS = this->StartTime / 1000;
	float EndTimeInS = this->EndTime / 1000;
	float BaseTimeInS = this->BaseTime / 1000;

	bool bPastPeriod = CurrentRunTime > EndTimeInS;

	// WHY NO TRIGGER BRO
	if (bPastPeriod) {
		Owner->Destroy();
		return;
	}

	// If already spawned, no need to handle any further logic until time to destroy
	if (Spawned) {
		return;
	}

	bool bShouldBeVisible = CurrentRunTime >= StartTimeInS && CurrentRunTime <= EndTimeInS;
	if (bShouldBeVisible) {
		Owner->SetActorHiddenInGame(false);
		Owner->SetActorEnableCollision(true);

		UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp) {
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

		Spawned = true;
	}
	else {
		Owner->SetActorHiddenInGame(true);
		Owner->SetActorEnableCollision(false);

		UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComp) {
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	// NOTE: Interaction mode is now Hover so we will only make it appear slightly before the perfect time, no need to change color
	/*FLinearColor IndicatorColor;
	if (CurrentRunTime >= StartTimeInS) {
		if (CurrentRunTime <= BaseTimeInS) {
			IndicatorColor = FLinearColor(1.0f, 0.5f, 0.0f);
		}
		else {
			IndicatorColor = FLinearColor::Green;
		}
	}

	UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(0));
	if (DynMaterial) {
		DynMaterial->SetVectorParameterValue("Color", IndicatorColor);
	}*/
}

void UBeatComponent::Initialize(int StartTimeArg, int EndTimeArg, float CoordXArg, float CoordYArg, int TimeArg) {
	this->StartTime = StartTimeArg;
	this->EndTime = EndTimeArg;
	this->BaseTime = TimeArg;
	this->CoordX = CoordXArg;
	this->CoordY = CoordYArg;

	UE_LOG(LogTemp, Warning, TEXT("BeatComponent initialized with StartTime: %d ms, EndTime: %d ms, CoordX: %f, CoordY: %f"), StartTimeArg, EndTimeArg, CoordXArg, CoordYArg);

	SpawnHitObject();

	AActor* Owner = GetOwner();
	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("BeatComponent does not have an Owner"));
		return;
	}

}

void UBeatComponent::SpawnHitObject() {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) {
		UE_LOG(LogTemp, Error, TEXT("No PlayerController found in the world"));
		return;
	}

	if (!PlayerController->PlayerCameraManager) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController does not have a PlayerCameraManager"));
		return;
	}

	AActor* Owner = GetOwner();

	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("BeatComponent does not have an Owner"));
		return;
	}

	FVector2D ViewportSize;
	UGameViewportClient* ViewportClient = GEngine->GameViewport;
	if (ViewportClient && ViewportClient->Viewport) {
		ViewportSize = ViewportClient->Viewport->GetSizeXY();
	}

	// NOTE: OSU Maps - 512 x 384
	// NOTE: Viewport - 1526 x 650
	float OsuWidth = 512.0f; 
	float OsuHeight = 384.0f;
	float UnitScale = 1.5f;

	float NormalizedCoordX = CoordX - (OsuWidth / 2.0f);
	float NormalizedCoordY = (OsuHeight / 2.0f) - CoordY;

	int32 Margin = 50;

	float OffsetCoordX = (NormalizedCoordX * ((ViewportSize.X - Margin) / OsuWidth)) / UnitScale;
	float OffsetCoordY = (NormalizedCoordY * ((ViewportSize.Y - Margin) / OsuHeight)) / UnitScale;

	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

	FVector RightVector = CameraRotation.Quaternion().GetRightVector();
	FVector UpVector = CameraRotation.Quaternion().GetUpVector();

	FVector OffsetVector = (RightVector * OffsetCoordX) + (UpVector * OffsetCoordY);

	FVector SpawnLocation = CameraLocation + (CameraRotation.Vector() * 800.0f) + OffsetVector;

	UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn at %s"), *SpawnLocation.ToString());

	Owner->SetActorLocation(SpawnLocation);
}
