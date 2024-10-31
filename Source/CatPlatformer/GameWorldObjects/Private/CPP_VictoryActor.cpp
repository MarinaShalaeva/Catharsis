// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_VictoryActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

ACPP_VictoryActor::ACPP_VictoryActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SM_Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Victory Static Mesh"));
	SM_Base->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("TimelineComponent")));

	ScoreToAdd = 30;

	SpawningOffset = FVector(-34.0f, -38.0f, 255.0f);

	RotationSpeed = 1.0f;

	StartPosition = FVector(0.0f);
	EndPosition = FVector(0.0f);
	ZPositionOffset = 0.0f;

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ACPP_VictoryActor::BeginPlay()
{
	Super::BeginPlay();

	SM_Base->OnComponentBeginOverlap.AddDynamic(this, &ACPP_VictoryActor::StaticMeshOverlapBegin);

	if (SmoothZMovementCurveFloat)
	{
		TimelineComp->Play();
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_Rotation,
		this,
		&ACPP_VictoryActor::RotateActorAroundItsAxis,
		0.05f,
		true);
}

void ACPP_VictoryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SmoothZMovementProgressDelegate.Unbind();
	SmoothZMovementTimelineEndedDelegate.Unbind();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void ACPP_VictoryActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_VictoryActor, StartPosition);
	DOREPLIFETIME(ACPP_VictoryActor, EndPosition);
}

void ACPP_VictoryActor::InitializeBasicVariables_Implementation(const FVector& StartLocation)
{
	if (SmoothZMovementCurveFloat)
	{
		StartPosition = StartLocation;
		EndPosition = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z + ZPositionOffset);

		SmoothZMovementProgressDelegate.BindUFunction(this, FName(TEXT("SmoothZMovementTimelineProgress")));

		TimelineComp->AddInterpFloat(SmoothZMovementCurveFloat, SmoothZMovementProgressDelegate);
		TimelineComp->SetLooping(true);
		TimelineComp->SetIgnoreTimeDilation(true);

		TimelineComp->SetTimelineFinishedFunc(SmoothZMovementTimelineEndedDelegate);

		TimelineComp->SetNewTime(FMath::RandRange(0.0f, TimelineComp->GetTimelineLength() - 0.01f));
	}
}

void ACPP_VictoryActor::StaticMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		CollectStaticMesh(Character);
	}
}

void ACPP_VictoryActor::CollectStaticMesh_Implementation(ACPP_Character* Character)
{
	ACPP_GameState* GS = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GS->AddUserScore(IsValid(Character->GetPlayerStateRef())
		                 ? Character->GetPlayerStateRef()
		                 : Cast<ACPP_PlayerState>(Character->GetPlayerState()), ScoreToAdd);
	//Character->Multicast_AddScore(ScoreToAdd);
	GS->LevelWasEnded();
	Multicast_CollectStaticMesh(Character);
}

void ACPP_VictoryActor::Multicast_CollectStaticMesh_Implementation(ACPP_Character* Character)
{
	if (IsValid(this))
	{
		Destroy();
	}
}

void ACPP_VictoryActor::RotateActorAroundItsAxis()
{
	// X = Roll, Y = Pitch, Z = Yaw.
	AddActorWorldRotation(FRotator(0.0f, RotationSpeed, 0.0f));
}

void ACPP_VictoryActor::SmoothZMovementTimelineProgress_Implementation(float Value)
{
	const FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
	SetActorLocation(NewLocation);
}
