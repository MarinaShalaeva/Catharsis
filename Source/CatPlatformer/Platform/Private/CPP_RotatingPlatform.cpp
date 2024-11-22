// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_RotatingPlatform.h"

#include "Net/UnrealNetwork.h"

ACPP_RotatingPlatform::ACPP_RotatingPlatform() : bBasicVariablesWereInitialized(false),
                                                 bAxis(false),
                                                 bUseAxisY(false),
                                                 Speed(25.0f),
                                                 Direction(-1)
{
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	bAllowReceiveTickEventOnDedicatedServer = true;
}

void ACPP_RotatingPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_RotatingPlatform, bUseAxisY);
}

void ACPP_RotatingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bBasicVariablesWereInitialized)
	{
		RotatePlatform(DeltaSeconds);
	}
}

void ACPP_RotatingPlatform::InitializeBasicVariables_Implementation(const FVector StartLocation)
{
	if (bBasicVariablesWereInitialized || !HasAuthority())
		return;

	Direction = FMath::RandBool() ? 1 : -1;

	switch (FMath::RandRange(1, 3))
	{
	case 1:
		{
			// X-axis rotation.
			bAxis = true;
			break;
		}
	case 2:
		{
			// Y-axis rotation.
			bUseAxisY = true;
			bAxis = true;
			break;
		}
	case 3:
		{
			// Z-axis rotation.
			bAxis = false;
			break;
		}
	default:
		{
			bAxis = false;
			break;
		}
	}

	if (bAxis)
	{
		// Z-axis rotation.
		Speed = FMath::FRandRange(14.0f, 16.0f);
	}
	else
	{
		// X-axis or Y-axis rotation.
		Speed = FMath::FRandRange(24.0f, 26.0f);
	}

	bBasicVariablesWereInitialized = true;
}

void ACPP_RotatingPlatform::ApplyPlatformProperty()
{
	Super::ApplyPlatformProperty();

	if (bUseAxisY)
	{
		InitialPlatformRotation();
	}
}

void ACPP_RotatingPlatform::InitialPlatformRotation() const
{
	PlatformBase->AddLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void ACPP_RotatingPlatform::RotatePlatform_Implementation(const float DeltaSeconds)
{
	if (bAxis)
	{
		AddActorLocalRotation(FRotator(0.0, 0.0, Speed * Direction * DeltaSeconds));
	}
	else
	{
		AddActorLocalRotation(FRotator(0.0, Speed * Direction * DeltaSeconds, 0.0));
	}
}
