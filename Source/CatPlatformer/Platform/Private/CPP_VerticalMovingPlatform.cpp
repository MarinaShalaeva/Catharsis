// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_VerticalMovingPlatform.h"
#include "Net/UnrealNetwork.h"

ACPP_VerticalMovingPlatform::ACPP_VerticalMovingPlatform() : SmoothZMovementCurveFloat(nullptr),
                                                             StartPosition(FVector(0.0f)),
                                                             EndPosition(FVector(0.0f)),
                                                             bIsMovingUp(true),
                                                             ZPositionOffset(45.0f)
{
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetupAttachment(RootComponent);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("TimelineComponent")));
}

void ACPP_VerticalMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TimelineComp->SetIsReplicated(true);
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

void ACPP_VerticalMovingPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		SmoothZMovementProgressDelegate.Unbind();
		SmoothZMovementTimelineEndedDelegate.Unbind();
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_VerticalMovingPlatform::InitializeBasicVariables_Implementation(const FVector StartLocation)
{
	Super::InitializeBasicVariables_Implementation(StartLocation);

	if (HasAuthority())
	{
		if (SmoothZMovementCurveFloat)
		{
			StartPosition = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z - ZPositionOffset);
			EndPosition = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z + ZPositionOffset);

			SmoothZMovementProgressDelegate.BindUFunction(this, FName(TEXT("SmoothZMovementTimelineProgress")));

			TimelineComp->AddInterpFloat(SmoothZMovementCurveFloat, SmoothZMovementProgressDelegate);
			TimelineComp->SetLooping(false);
			TimelineComp->SetIgnoreTimeDilation(true);

			SmoothZMovementTimelineEndedDelegate.BindUFunction(this, FName(TEXT("SmoothZMovementTimelineEnded")));
			TimelineComp->SetTimelineFinishedFunc(SmoothZMovementTimelineEndedDelegate);

			TimelineComp->SetNewTime(FMath::RandRange(0.0f, TimelineComp->GetTimelineLength() - 0.01f));

			bIsMovingUp = FMath::RandBool();
		}
	}
}

void ACPP_VerticalMovingPlatform::SmoothZMovementTimelineProgress(float Value)
{
	const FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
	SetActorLocation(NewLocation);
}

void ACPP_VerticalMovingPlatform::SmoothZMovementTimelineEnded()
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
