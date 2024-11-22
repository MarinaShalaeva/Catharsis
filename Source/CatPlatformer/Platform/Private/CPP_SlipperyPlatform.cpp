// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SlipperyPlatform.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

ACPP_SlipperyPlatform::ACPP_SlipperyPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Collision")));
	CollisionBox->SetupAttachment(PlatformBase);

	IceAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("Ice Sound")));
	IceAudioComponent->SetupAttachment(PlatformBase);

	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(FName(TEXT("Timeline Component")));

	StartRotation = FRotator(0.0f);
	EndRotation = FRotator(0.0f);

	CircularRotationOffset = 8.0f;
}

void ACPP_SlipperyPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_SlipperyPlatform::CollisionBoxOverlapBegin);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACPP_SlipperyPlatform::CollisionBoxOverlapEnd);

		TimelineComp->SetIsReplicated(true);
		if (CurveVector)
		{
			StartRotation = GetActorRotation();
			EndRotation = FRotator(StartRotation.Pitch + CircularRotationOffset,
			                       StartRotation.Yaw,
			                       StartRotation.Roll + CircularRotationOffset);

			TimelineProgressDelegate.BindUFunction(this, FName(TEXT("CircularRotationTimelineProgress")));

			TimelineComp->AddInterpVector(CurveVector, TimelineProgressDelegate);
			TimelineComp->SetLooping(true);
			TimelineComp->SetIgnoreTimeDilation(true);
		}
	}
}

void ACPP_SlipperyPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_SlipperyPlatform::CollisionBoxOverlapBegin);
		CollisionBox->OnComponentEndOverlap.RemoveDynamic(this, &ACPP_SlipperyPlatform::CollisionBoxOverlapEnd);

		if (CurveVector)
		{
			TimelineProgressDelegate.Unbind();
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_SlipperyPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_SlipperyPlatform, bPlatformAppearanceType);
}

void ACPP_SlipperyPlatform::InitializeBasicVariables_Implementation(const FVector StartLocation)
{
	if (!HasAuthority())
		return;

	bPlatformAppearanceType = FMath::RandBool();
}

void ACPP_SlipperyPlatform::CollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                     bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		Character->ChangeCharactersSliding(true);

		SwitchSoundState(true);

		if (!TimelineComp->IsPlaying())
		{
			TimelineComp->PlayFromStart();
		}
	}
}

void ACPP_SlipperyPlatform::CollisionBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority())
		return;

	if (ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		Character->ChangeCharactersSliding(false);

		TArray<AActor*> OverlappedActors;
		CollisionBox->GetOverlappingActors(OverlappedActors, ACPP_Character::StaticClass());

		if (OverlappedActors.Num() == 0)
		{
			SwitchSoundState(false);
			if (TimelineComp->IsPlaying())
			{
				TimelineComp->Stop();
			}
		}
	}
}

void ACPP_SlipperyPlatform::SwitchSoundState_Implementation(const bool bTurnOn)
{
	if (IsValid(IceAudioComponent) && IsValid(IceAudioComponent->GetSound()))
	{
		if (bTurnOn)
		{
			if (!IceAudioComponent->IsPlaying())
			{
				IceAudioComponent->Play(0.0f);
			}
		}
		else
		{
			if (IceAudioComponent->IsPlaying())
			{
				IceAudioComponent->Stop();
			}
		}
	}
}

void ACPP_SlipperyPlatform::CircularRotationTimelineProgress(FVector Value)
{
	const FRotator NewRotation = FRotator(FMath::Lerp(StartRotation.Pitch, EndRotation.Pitch, Value.Y),
	                                      FMath::Lerp(StartRotation.Yaw, EndRotation.Yaw, Value.Z),
	                                      FMath::Lerp(StartRotation.Roll, EndRotation.Roll, Value.X));
	SetActorRotation(NewRotation);
}
