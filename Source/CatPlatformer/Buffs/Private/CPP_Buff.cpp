// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_Buff.h"
#include "Kismet/GameplayStatics.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;
class ACPP_PlayerState;

ACPP_Buff::ACPP_Buff() : BuffTypeId(-1),
                         EffectDuration(5.0f),
                         BuffImage(nullptr),
                         ScoreToAdd(10),
                         BuffRotationSpeed(4.5f),
                         SmoothZMovementCurveFloat(nullptr),
                         StartPosition(FVector(0.0f)),
                         EndPosition(FVector(0.0f)),
                         bIsMovingUp(true),
                         ZPositionOffset(45.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(Root);

	SM_Buff = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Buff Static Mesh")));
	SM_Buff->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("TimelineComponent")));

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ACPP_Buff::BeginPlay()
{
	Super::BeginPlay();

	SM_Buff->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACPP_Buff::StaticMeshOverlapBegin);

	if (HasAuthority() && SmoothZMovementCurveFloat)
	{
		if (bIsMovingUp)
		{
			TimelineComp->Play();
		}
		else
		{
			TimelineComp->Reverse();
		}
	}
}

void ACPP_Buff::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SM_Buff->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_Buff::StaticMeshOverlapBegin);

	if (HasAuthority())
	{
		SmoothZMovementProgressDelegate.Unbind();
		SmoothZMovementTimelineEndedDelegate.Unbind();
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_Buff::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RotateBuffAroundItsAxis(DeltaSeconds);
}

void ACPP_Buff::InitializeBasicVariables_Implementation(const FVector& StartLocation)
{
	if (!HasAuthority())
		return;

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
	if (!HasAuthority())
		return;

	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		CollectBuff(Character);
	}
}

void ACPP_Buff::CollectBuff_Implementation(ACPP_Character* Character)
{
	if (!HasAuthority())
		return;

	Multicast_CollectBuff(Character);
	
	if (ACPP_GameState* GameState =
			Cast<ACPP_GameState>(UGameplayStatics::GetGameState(Character->GetWorld()));
		IsValid(GameState))
	{
		GameState->AddUserScore(IsValid(Character->GetPlayerStateRef())
			                        ? Character->GetPlayerStateRef()
			                        : Cast<ACPP_PlayerState>(Character->GetPlayerState()), ScoreToAdd);
	}

	if (IsValid(this))
	{
		Destroy();
	}
}

void ACPP_Buff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
}

void ACPP_Buff::RotateBuffAroundItsAxis(const float DeltaSeconds)
{
	if (HasAuthority())
	{
		// X = Roll, Y = Pitch, Z = Yaw.
		AddActorWorldRotation(FRotator(0.0f, BuffRotationSpeed * DeltaSeconds, 0.0f));
	}
}

void ACPP_Buff::SmoothZMovementTimelineProgress_Implementation(float Value)
{
	if (HasAuthority())
	{
		const FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
		SetActorLocation(NewLocation);
	}
}

void ACPP_Buff::SmoothZMovementTimelineEnded()
{
	if (HasAuthority())
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
}
