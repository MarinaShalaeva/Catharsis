// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_UI_ThreeDimensionalActor.h"

ACPP_UI_ThreeDimensionalActor::ACPP_UI_ThreeDimensionalActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.025);

	SceneComponentRotationSpeed = 3.5f;
	RotationAxis = FString(TEXT("Z"));
	bShouldRotate = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(Root);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(FName(TEXT("SceneCaptureComponent2D")));
	SceneCaptureComponent2D->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ComponentToRotate = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Container")));
	ComponentToRotate->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACPP_UI_ThreeDimensionalActor::BeginPlay()
{
	Super::BeginPlay();

	if (bShouldRotate)
	{
		if (RotationAxis.Equals(FString(TEXT("X"))))
		{
			GetWorld()->GetTimerManager().SetTimer(
				TH_SceneComponentRotation,
				this,
				&ACPP_UI_ThreeDimensionalActor::RotateThreeDimensionalObjectAxisX,
				0.05f,
				true);
		}
		else if (RotationAxis.Equals(FString(TEXT("Y"))))
		{
			GetWorld()->GetTimerManager().SetTimer(
				TH_SceneComponentRotation,
				this,
				&ACPP_UI_ThreeDimensionalActor::RotateThreeDimensionalObjectAxisY,
				0.05f,
				true);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(
				TH_SceneComponentRotation,
				this,
				&ACPP_UI_ThreeDimensionalActor::RotateThreeDimensionalObjectAxisZ,
				0.05f,
				true);
		}
	}
}

void ACPP_UI_ThreeDimensionalActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bShouldRotate)
	{
		if (GetWorld()->GetTimerManager().TimerExists(TH_SceneComponentRotation))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_SceneComponentRotation);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_UI_ThreeDimensionalActor::PostInitProperties()
{
	Super::PostInitProperties();

	SceneCaptureComponent2D->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SceneCaptureComponent2D->FOVAngle = 45.0f;
}

void ACPP_UI_ThreeDimensionalActor::RotateThreeDimensionalObjectAxisX() const
{
	ComponentToRotate->AddLocalRotation(FRotator(0.0f, 0.0f, SceneComponentRotationSpeed));
}

void ACPP_UI_ThreeDimensionalActor::RotateThreeDimensionalObjectAxisY() const
{
	ComponentToRotate->AddLocalRotation(FRotator(SceneComponentRotationSpeed, 0.0f, 0.0f));
}

void ACPP_UI_ThreeDimensionalActor::RotateThreeDimensionalObjectAxisZ() const
{
	ComponentToRotate->AddLocalRotation(FRotator(0.0f, SceneComponentRotationSpeed, 0.0f));
}
