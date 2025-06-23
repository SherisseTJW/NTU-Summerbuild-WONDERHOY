// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"
#include "Parser/headers/hit-object.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AHitObject::AHitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->bSelectable = true;
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->bReturnMaterialOnMove = true;

	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCanEverAffectNavigation(false);

	beatComponent = CreateDefaultSubobject<UBeatComponent>(TEXT("beatComponent"));
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("BeatComponent not created successfully"));
	}
}

// Called when the game starts or when spawned
void AHitObject::BeginPlay() {
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PC->SetInputMode(InputMode);
	}

	Mesh->OnClicked.AddDynamic(this, &AHitObject::OnMeshClicked);
}

// Called every frame
void AHitObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AHitObject::OnMeshClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed) {
	if (ClickedComp == Mesh) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("VALID CLICK CLICK CLICK"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("CLICK CLICK CLICK"));
	}
}

void AHitObject::Initialize(beatmap::HitObject* HitObjectArg, beatmap::Coord Loc) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	if (!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. Null Mesh"));
		return;
	}


	UStaticMesh* LoadedMesh; 
	HitObjectType = HitObjectArg->getType();
	switch (HitObjectType) {
		case beatmap::HitObject::ObjectType::HIT_CIRCLE:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			break;
		case beatmap::HitObject::ObjectType::SLIDER:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
			break;
		case beatmap::HitObject::ObjectType::SPINNER:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			break;
		default:
			LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			break;
	}

	if (LoadedMesh) {
		Mesh->SetStaticMesh(LoadedMesh);
	}

	int _Time = HitObjectArg->getTime();
	int StartTime = _Time - OffsetTime;
	int EndTime = _Time + OffsetTime;

	beatComponent->Initialize(StartTime, EndTime, Loc.getX(), Loc.getY());
}
