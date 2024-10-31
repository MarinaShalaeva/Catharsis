// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_FallingPlatform.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACharacter;
class ACPP_Character;

ACPP_FallingPlatform::ACPP_FallingPlatform()
{
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Collision")));
	CollisionBox->SetupAttachment(PlatformBase);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("Timeline Component")));

	StartTransform = FTransform();
	StartRotation = FRotator(0.0f);
	EndRotation = FRotator(0.0f);

	FallingSpeed = 9.0f;
	SecondsBeforeFall = 3.0f;
	ShakingOffset = 2.0f;
}

void ACPP_FallingPlatform::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_FallingPlatform::CollisionBoxOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACPP_FallingPlatform::CollisionBoxOverlapEnd);

	StartTransform = GetActorTransform();

	if (CurveVector)
	{
		StartRotation = GetActorRotation();
		// X = Roll, Y = Pitch, Z = Yaw.
		EndRotation = FRotator(StartRotation.Pitch + ShakingOffset,
		                       StartRotation.Yaw,
		                       StartRotation.Roll + ShakingOffset);

		TimelineProgressDelegate.BindUFunction(this, FName(TEXT("ShakingTimelineProgress")));

		TimelineComp->AddInterpVector(CurveVector, TimelineProgressDelegate);
		TimelineComp->SetLooping(true);
		TimelineComp->SetIgnoreTimeDilation(true);
	}
}

void ACPP_FallingPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CollisionBox->OnComponentEndOverlap.RemoveDynamic(this, &ACPP_FallingPlatform::CollisionBoxOverlapEnd);
	CollisionBox->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_FallingPlatform::CollisionBoxOverlapBegin);

	if (CurveVector)
	{
		TimelineProgressDelegate.Unbind();
	}

	Super::EndPlay(EndPlayReason);
}

void ACPP_FallingPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_FallingPlatform, FallingSpeed);
	DOREPLIFETIME(ACPP_FallingPlatform, SecondsBeforeFall);
}

void ACPP_FallingPlatform::InitializeBasicVariables_Implementation(const FVector StartLocation)
{
	Super::InitializeBasicVariables_Implementation(StartLocation);

	FallingSpeed = FMath::FRandRange(9.0f, 12.0f);
	SecondsBeforeFall = FMath::FRandRange(2.5f, 5.0f);
}

void ACPP_FallingPlatform::CollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		Character->SetIsOnGrass(true);
		if (!GetWorld()->GetTimerManager().TimerExists(TH_FallingTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(
				TH_FallingTimer,
				this,
				&ACPP_FallingPlatform::Falling,
				0.025f,
				true,
				SecondsBeforeFall);

			if (!TimelineComp->IsPlaying())
			{
				TimelineComp->PlayFromStart();
			}
		}
	}
}

void ACPP_FallingPlatform::CollisionBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		Character->SetIsOnGrass(false);
	}
}

void ACPP_FallingPlatform::Falling()
{
	if (TimelineComp->IsPlaying())
	{
		TimelineComp->Stop();
	}

	this->AddActorWorldOffset(FVector(0.0f, 0.0f, -FallingSpeed));

	if (this->GetActorLocation().Z <= -1200.0f)
	{
		TeleportPlatformToStartPosition();
	}
}

void ACPP_FallingPlatform::TeleportPlatformToStartPosition()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_FallingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FallingTimer);
	}

	this->TeleportTo(StartTransform.GetLocation(), StartTransform.Rotator());
}

void ACPP_FallingPlatform::ShakingTimelineProgress(FVector Value)
{
	const FRotator NewRotation = FRotator(FMath::Lerp(StartRotation.Pitch, EndRotation.Pitch, Value.Y),
	                                      FMath::Lerp(StartRotation.Yaw, EndRotation.Yaw, Value.Z),
	                                      FMath::Lerp(StartRotation.Roll, EndRotation.Roll, Value.X));
	SetActorRotation(NewRotation);
}
