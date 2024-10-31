// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_Platform.h"

ACPP_Platform::ACPP_Platform()
{
	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(Root);

	PlatformBase = nullptr;
	
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_Platform::BeginPlay()
{
	Super::BeginPlay();

	ApplyPlatformProperty();
}

void ACPP_Platform::InitializeBasicVariables_Implementation(const FVector StartLocation)
{
}

void ACPP_Platform::ApplyPlatformProperty()
{
}
