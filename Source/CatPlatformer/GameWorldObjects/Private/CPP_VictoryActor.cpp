// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_VictoryActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

ACPP_VictoryActor::ACPP_VictoryActor() : ScoreToAdd(30),
                                         SpawningOffset(FVector(-34.0f, -38.0f, 255.0f)),
                                         RotationSpeed(4.4f),
                                         StartPosition(FVector(0.0f)),
                                         EndPosition(FVector(0.0f)),
                                         ZPositionOffset(30.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SM_Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Victory Static Mesh"));
	SM_Base->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("TimelineComponent")));

	/*ScoreToAdd = 30;

	SpawningOffset = FVector(-34.0f, -38.0f, 255.0f);

	RotationSpeed = 1.0f;

	StartPosition = FVector(0.0f);
	EndPosition = FVector(0.0f);
	ZPositionOffset = 0.0f;*/

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ACPP_VictoryActor::BeginPlay()
{
	Super::BeginPlay();

	SM_Base->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACPP_VictoryActor::StaticMeshOverlapBegin);

	if (HasAuthority() && SmoothZMovementCurveFloat)
	{
		TimelineComp->Play();
	}
}

void ACPP_VictoryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SM_Base->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_VictoryActor::StaticMeshOverlapBegin);

	if (HasAuthority())
	{
		SmoothZMovementProgressDelegate.Unbind();
		SmoothZMovementTimelineEndedDelegate.Unbind();
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_VictoryActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RotateActorAroundItsAxis(DeltaSeconds);
}

void ACPP_VictoryActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME(ACPP_VictoryActor, StartPosition);
	DOREPLIFETIME(ACPP_VictoryActor, EndPosition);*/
}

void ACPP_VictoryActor::InitializeBasicVariables_Implementation(const FVector& StartLocation)
{
	if (!HasAuthority())
		return;

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
	if (!HasAuthority())
		return;

	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		CollectStaticMesh(Character);
	}
}


void ACPP_VictoryActor::CollectStaticMesh_Implementation(ACPP_Character* Character)
{
	if (!HasAuthority())
		return;

	ACPP_GameState* GS = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GS->AddUserScore(IsValid(Character->GetPlayerStateRef())
		                 ? Character->GetPlayerStateRef()
		                 : Cast<ACPP_PlayerState>(Character->GetPlayerState()), ScoreToAdd);
	GS->LevelWasEnded();
	if (IsValid(this))
	{
		Destroy();
	}
}

void ACPP_VictoryActor::Multicast_CollectStaticMesh_Implementation(ACPP_Character* Character)
{
	if (IsValid(this))
	{
		Destroy();
	}
}

void ACPP_VictoryActor::RotateActorAroundItsAxis(const float DeltaSeconds)
{
	// X = Roll, Y = Pitch, Z = Yaw.
	AddActorWorldRotation(FRotator(0.0f, RotationSpeed * DeltaSeconds, 0.0f));
}

void ACPP_VictoryActor::SmoothZMovementTimelineProgress_Implementation(float Value)
{
	const FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
	SetActorLocation(NewLocation);
}
