// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatComponent.h"
#include "Kismet/GameplayStatics.h"

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

	//bool bVisible = !(CurrentRunTime >= this->StartTime && CurrentRunTime <= this->EndTime);
	//Owner->SetActorHiddenInGame(bVisible);
}

void UBeatComponent::Initialize(float StartTimeArg, float EndTimeArg, float CoordXArg, float CoordYArg)
{
	this->StartTime = StartTimeArg;
	this->EndTime = EndTimeArg;
	this->CoordX = CoordXArg;
	this->CoordY = CoordYArg;

	AActor* Owner = GetOwner();

	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("BeatComponent does not have an Owner"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BeatComponent initialized with StartTime: %f, EndTime: %f, CoordX: %f, CoordY: %f"), StartTimeArg, EndTimeArg, CoordXArg, CoordYArg);

	// Transforming to 2D visuals
	FVector SpawnLocation(CoordX, 0.0f, CoordY);
	Owner->SetActorLocation(SpawnLocation);
}
