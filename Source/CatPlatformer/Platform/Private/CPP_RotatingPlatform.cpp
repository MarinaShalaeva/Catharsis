// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_RotatingPlatform.h"
#include "Net/UnrealNetwork.h"

ACPP_RotatingPlatform::ACPP_RotatingPlatform() : Super(),
                                                 bBasicVariablesWereInitialized(false),
                                                 Coordinate(FString(TEXT("X"))),
                                                 Speed(25.0f),
                                                 Direction(-1)
{
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	bAllowReceiveTickEventOnDedicatedServer = true;
}

void ACPP_RotatingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_RotatingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bBasicVariablesWereInitialized)
	{
		RotatePlatform(DeltaSeconds);
	}
}

void ACPP_RotatingPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_RotatingPlatform, Coordinate);
	DOREPLIFETIME(ACPP_RotatingPlatform, Speed);
	DOREPLIFETIME(ACPP_RotatingPlatform, Direction);
}

void ACPP_RotatingPlatform::InitializeBasicVariables_Implementation(const FVector StartLocation)
{
	if (bBasicVariablesWereInitialized)
		return;

	Direction = FMath::RandBool() ? 1 : -1;

	switch (FMath::RandRange(1, 3))
	{
	case 1:
		{
			Coordinate = FString(TEXT("X"));
			break;
		}
	case 2:
		{
			InitialPlatformRotation();
			Coordinate = FString(TEXT("X"));
			break;
		}
	case 3:
		{
			Coordinate = FString(TEXT("Z"));
			break;
		}
	default:
		{
			Coordinate = FString(TEXT("Z"));
			break;
		}
	}

	if (Coordinate == FString(TEXT("Z")))
	{
		Speed = FMath::FRandRange(24.0f, 26.0f);
	}
	else
	{
		Speed = FMath::FRandRange(14.0f, 16.0f);
	}

	bBasicVariablesWereInitialized = true;
}

void ACPP_RotatingPlatform::ApplyPlatformProperty()
{
	Super::ApplyPlatformProperty();

	/*GetWorld()->GetTimerManager().SetTimer(
		TH_Rotation,
		this,
		&ACPP_RotatingPlatform::RotatePlatform,
		0.017f,
		true,
		0.0f);*/
}

void ACPP_RotatingPlatform::InitialPlatformRotation_Implementation()
{
	Multicast_InitialPlatformRotation();
}

void ACPP_RotatingPlatform::Multicast_InitialPlatformRotation_Implementation()
{
	PlatformBase->AddLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void ACPP_RotatingPlatform::RotatePlatform_Implementation(const float& DeltaSeconds)
{
	Multicast_RotatePlatform(DeltaSeconds);
}

void ACPP_RotatingPlatform::Multicast_RotatePlatform_Implementation(const float& DeltaSeconds)
{
	if (Coordinate.Equals(FString(TEXT("X"))))
	{
		AddActorLocalRotation(FRotator(0.0, 0.0, Speed * Direction * DeltaSeconds));
	}
	else if (Coordinate.Equals(FString(TEXT("Z"))))
	{
		AddActorLocalRotation(FRotator(0.0, Speed * Direction * DeltaSeconds, 0.0));
	}
}
