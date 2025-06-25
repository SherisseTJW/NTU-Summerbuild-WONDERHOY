// Fill out your copyright notice in the Description page of Project Settings.


#include "HitObject.h"
#include "BeatComponent.h"
#include "Parser/headers/hit-object.h"
#include "Parser/headers/parser.h"

#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHitObject::AHitObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CharacterVisual = CreateDefaultSubobject<UChildActorComponent>(TEXT("CharacterVisual"));
	CharacterVisual->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<AActor> CharacterBP(TEXT("/Game/Assets/Characters/BP_Character_Emu"));
	if (CharacterBP.Succeeded()) {
		CharacterVisual->SetChildActorClass(CharacterBP.Class);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to find BP_CharacterBase"));
	}

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

	JudgementWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("JudgementWidget"));
	JudgementWidgetComponent->SetupAttachment(RootComponent);
	JudgementWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	JudgementWidgetComponent->SetDrawSize(FVector2D(200.f, 100.f));
	JudgementWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // Slightly above the actor
	JudgementWidgetComponent->SetVisibility(false); // Start hidden

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

	Mesh->OnBeginCursorOver.AddDynamic(this, &AHitObject::OnMouseOverStart);
	Mesh->OnEndCursorOver.AddDynamic(this, &AHitObject::OnMouseOverEnd);
}

// Called every frame
void AHitObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AHitObject::OnMouseOverStart(UPrimitiveComponent* TouchedComp) {
	if (TouchedComp == Mesh) {
		StartMouseOverTime = (UGameplayStatics::GetRealTimeSeconds(GetWorld()) * 1000) - LeadTime; // Convert to milliseconds
	}
}

void AHitObject::OnMouseOverEnd(UPrimitiveComponent* TouchedComp) {
	if (TouchedComp == Mesh) {
		float CurrentRunTime = (UGameplayStatics::GetRealTimeSeconds(GetWorld()) * 1000) - LeadTime;
		float RegisterTime = (CurrentRunTime + StartMouseOverTime) / 2;

		float ExpectedTime = HitObject->getTime();
		FString TimeString = FString::Printf(TEXT("Registered: %.2f seconds, expected time: %.2f seconds"), RegisterTime, ExpectedTime);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TimeString);

		beatmap::HitObject::Judgement JudgementResult = Beatmap->getJudgement(CurrentRunTime, HitObject);

		FString JudgementResultStr;
		switch (JudgementResult) {
		case beatmap::HitObject::MISS: 
			JudgementResultStr = TEXT("MISS");
			break;
		case beatmap::HitObject::BAD: 
			JudgementResultStr = TEXT("BAD");
			break;
		case beatmap::HitObject::GOOD: 
			JudgementResultStr = TEXT("GOOD");
			break;
		case beatmap::HitObject::GREAT: 
			JudgementResultStr = TEXT("GREAT");
			break;
		case beatmap::HitObject::PERFECT: 
			JudgementResultStr = TEXT("PERFECT");
			break;
		default:
			JudgementResultStr = TEXT("UNKNOWN");
			break;
		}

		FString Output = FString::Printf(TEXT("Judgement: %s"), *JudgementResultStr);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Output);

		// Once hovered, destroy the HitObject so its not clickable again or visible
		this->Destroy();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Mouse is not over the HitObject Mesh."));
	}
}

void AHitObject::Initialize(beatmap::HitObject* HitObjectArg, beatmap::Coord Loc, beatmap::Beatmap* BeatmapArg) {
	if (!beatComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. BeatComponent not created successfully"));
		return;
	}

	if (!Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("Error Initializing HitObject.. Null Mesh"));
		return;
	}

	Beatmap = BeatmapArg;
	HitObject = HitObjectArg;
	HitObjectType = HitObjectArg->getType();

	switch (HitObjectType) {
		case beatmap::HitObject::ObjectType::HIT_CIRCLE:
			UE_LOG(LogTemp, Warning, TEXT("HitObject Type: HIT_CIRCLE"));
			break;
		case beatmap::HitObject::ObjectType::SLIDER:
			UE_LOG(LogTemp, Warning, TEXT("HitObject Type: SLIDER"));
			break;
		case beatmap::HitObject::ObjectType::SPINNER:
			UE_LOG(LogTemp, Warning, TEXT("HitObject Type: SPINNER"));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("HitObject Type: DEFAULT"));
			break;
	}

	/*if (LoadedMesh) {
		Mesh->SetStaticMesh(LoadedMesh);
	}*/

	int _Time = HitObject->getTime();
	int StartTime = _Time - 10; // Adjust a bit for unreal delay
	int EndTime = _Time + OffsetTime;

	beatComponent->Initialize(StartTime, EndTime, Loc.getX(), Loc.getY(), _Time);
}
