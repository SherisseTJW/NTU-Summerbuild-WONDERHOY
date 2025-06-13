// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatComponent.h"

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

	// Only when between StartTime and EndTime, appear
	float CurrentTime = GetWorld()->GetTimeSeconds();
	bool Visible = CurrentTime >= this->StartTime && CurrentTime <= this->EndTime;
	bool AfterVisiblePeriod = CurrentTime > this->EndTime;

	if (AActor* Owner = GetOwner()) {
		if (AfterVisiblePeriod) {
			Owner->Destroy();
		}
		else {
			Owner->SetActorHiddenInGame(Visible);
		}
	}
}

void UBeatComponent::Initialize(float StartTimeArg, float EndTimeArg, float CoordXArg, float CoordYArg)
{
	this->StartTime = StartTimeArg;
	this->EndTime = EndTimeArg;
	this->CoordX = CoordXArg;
	this->CoordY = CoordYArg;
}
