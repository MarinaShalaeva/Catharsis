// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_Buff.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;
class ACPP_PlayerState;

ACPP_Buff::ACPP_Buff()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(Root);

	SM_Buff = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Buff Static Mesh")));
	SM_Buff->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("TimelineComponent")));

	SmoothZMovementCurveFloat = nullptr;

	BuffTypeId = -1;
	EffectDuration = 5.0f;
	BuffImage = nullptr;
	ScoreToAdd = 10;

	bIsMovingUp = true;
	StartPosition = FVector(0.0f);
	EndPosition = FVector(0.0f);
	ZPositionOffset = 45.0f;
	BuffRotationSpeed = 1.0f;

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ACPP_Buff::BeginPlay()
{
	Super::BeginPlay();

	SM_Buff->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Buff::StaticMeshOverlapBegin);

	if (SmoothZMovementCurveFloat)
	{
		TimelineComp->Play();
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_BuffRotation,
		this,
		&ACPP_Buff::RotateBuffAroundItsAxis,
		0.05f,
		true);
}

void ACPP_Buff::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SM_Buff->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_Buff::StaticMeshOverlapBegin);

	SmoothZMovementProgressDelegate.Unbind();
	SmoothZMovementTimelineEndedDelegate.Unbind();

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void ACPP_Buff::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_Buff, StartPosition);
	DOREPLIFETIME(ACPP_Buff, EndPosition);
}

void ACPP_Buff::InitializeBasicVariables_Implementation(const FVector& StartLocation)
{
	if (SmoothZMovementCurveFloat)
	{
		StartPosition = StartLocation;
		EndPosition = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z + ZPositionOffset);

		SmoothZMovementProgressDelegate.BindUFunction(this, FName(TEXT("SmoothZMovementTimelineProgress")));

		TimelineComp->AddInterpFloat(SmoothZMovementCurveFloat, SmoothZMovementProgressDelegate);
		TimelineComp->SetLooping(false);
		TimelineComp->SetIgnoreTimeDilation(true);

		SmoothZMovementTimelineEndedDelegate.BindUFunction(this, FName(TEXT("SmoothZMovementTimelineEnded")));
		TimelineComp->SetTimelineFinishedFunc(SmoothZMovementTimelineEndedDelegate);

		TimelineComp->SetNewTime(FMath::RandRange(0.0f, TimelineComp->GetTimelineLength() - 0.01f));

		bIsMovingUp = true;
	}
}

void ACPP_Buff::StaticMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		CollectBuff(Character);
	}
}

void ACPP_Buff::CollectBuff_Implementation(ACPP_Character* Character)
{
	if (ACPP_GameState* GameState =
			Cast<ACPP_GameState>(UGameplayStatics::GetGameState(Character->GetWorld()));
		IsValid(GameState))
	{
		GameState->AddUserScore(IsValid(Character->GetPlayerStateRef())
			                        ? Character->GetPlayerStateRef()
			                        : Cast<ACPP_PlayerState>(Character->GetPlayerState()), ScoreToAdd);
	}
	Multicast_CollectBuff(Character);
}

void ACPP_Buff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
	if (IsValid(this))
	{
		Destroy();
	}
}

void ACPP_Buff::RotateBuffAroundItsAxis()
{
	// X = Roll, Y = Pitch, Z = Yaw.
	AddActorWorldRotation(FRotator(0.0f, BuffRotationSpeed, 0.0f));
}

void ACPP_Buff::SmoothZMovementTimelineProgress_Implementation(float Value)
{
	const FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
	SetActorLocation(NewLocation);
}

void ACPP_Buff::SmoothZMovementTimelineEnded()
{
	if (bIsMovingUp)
	{
		TimelineComp->ReverseFromEnd();
		bIsMovingUp = false;
	}
	else
	{
		TimelineComp->PlayFromStart();
		bIsMovingUp = true;
	}
}
