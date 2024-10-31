// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SimplePlatform.h"

ACPP_SimplePlatform::ACPP_SimplePlatform()
{
	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetupAttachment(RootComponent);
}
