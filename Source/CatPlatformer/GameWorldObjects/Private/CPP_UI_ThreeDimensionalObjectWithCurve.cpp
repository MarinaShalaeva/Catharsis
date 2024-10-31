// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_UI_ThreeDimensionalObjectWithCurve.h"

ACPP_UI_ThreeDimensionalObjectWithCurve::ACPP_UI_ThreeDimensionalObjectWithCurve()
{
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform")));
	PlatformBase->SetupAttachment(ComponentToRotate);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("Timeline Component")));
	
	ShakingOffset = 2.0f;
}

void ACPP_UI_ThreeDimensionalObjectWithCurve::BeginPlay()
{
	Super::BeginPlay();

	if (CurveVector)
	{
		StartRotation = PlatformBase->GetRelativeRotation();
		// X = Roll, Y = Pitch, Z = Yaw.
		EndRotation = FRotator(StartRotation.Pitch + ShakingOffset,
		                       StartRotation.Yaw,
		                       StartRotation.Roll + ShakingOffset);

		TimelineProgressDelegate.BindUFunction(this, FName(TEXT("ShakingTimelineProgress")));

		TimelineComp->AddInterpVector(CurveVector, TimelineProgressDelegate);
		TimelineComp->SetLooping(true);
		TimelineComp->SetIgnoreTimeDilation(true);
		TimelineComp->PlayFromStart();
	}
}

void ACPP_UI_ThreeDimensionalObjectWithCurve::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurveVector)
	{
		TimelineProgressDelegate.Unbind();
	}

	Super::EndPlay(EndPlayReason);
}

void ACPP_UI_ThreeDimensionalObjectWithCurve::ShakingTimelineProgress(FVector Value)
{
	const FRotator NewRotation = FRotator(FMath::Lerp(StartRotation.Pitch, EndRotation.Pitch, Value.Y),
	                                      FMath::Lerp(StartRotation.Yaw, EndRotation.Yaw, Value.Z),
	                                      FMath::Lerp(StartRotation.Roll, EndRotation.Roll, Value.X));
	PlatformBase->SetRelativeRotation(NewRotation);
}
