// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatComponent.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

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

	bool bPastPeriod = CurrentRunTime > this->EndTime;

	if (bPastPeriod) {
		Owner->Destroy();
		return;
	}

	float StartTimeInS = this->StartTime / 1000;
	float EndTimeInS = this->EndTime / 1000;

	bool bVisible = !(CurrentRunTime >= StartTimeInS && CurrentRunTime <= EndTimeInS);
	Owner->SetActorHiddenInGame(bVisible);
	Owner->SetActorEnableCollision(!bVisible);

	UStaticMeshComponent* MeshComp = Owner->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComp) {
		MeshComp->SetCollisionEnabled(!bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}

	bool beforeClick = CurrentRunTime >= StartTimeInS && CurrentRunTime <= BaseTime;
	FLinearColor IndicatorColor;

	if (beforeClick) {
		IndicatorColor = FLinearColor::Red;
	}
	else {
		IndicatorColor = FLinearColor::Green;
	}

	if (MeshComp) {
		UMaterialInstanceDynamic* DynMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMaterial) {
			DynMaterial->SetVectorParameterValue("BaseColor", IndicatorColor);
		}
	}
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

	FVector SpawnLocation = CameraLocation + (CameraRotation.Vector() * 1000.0f) + OffsetVector;

	UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn at %s"), *SpawnLocation.ToString());

	Owner->SetActorLocation(SpawnLocation);
}
