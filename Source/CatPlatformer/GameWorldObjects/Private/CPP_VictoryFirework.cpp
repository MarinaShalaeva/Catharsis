// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_VictoryFirework.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ACPP_VictoryFirework::ACPP_VictoryFirework() : FireworkSoundCue0(nullptr),
                                               FireworkSoundCue1(nullptr),
                                               NS_Firework(nullptr),
                                               NiagaraComponent(nullptr),
                                               FireworkExplosionsCounter(0)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACPP_VictoryFirework::BeginPlay()
{
	Super::BeginPlay();

	if (NS_Firework->IsValid())
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), NS_Firework, SpawnLocation, FRotator(),
			FVector(1.0f), true, true,
			ENCPoolMethod::None, true);

		if (IsValid(NiagaraComponent) && !NiagaraComponent->OnSystemFinished.IsAlreadyBound(
			this, &ACPP_VictoryFirework::OnFireworkSystemFinished))
		{
			NiagaraComponent->OnSystemFinished.AddUniqueDynamic(this, &ACPP_VictoryFirework::OnFireworkSystemFinished);
		}

		GetWorld()->GetTimerManager().SetTimer(TH_SoundSpawning,
		                                       this,
		                                       &ACPP_VictoryFirework::SpawnSound,
		                                       2.0f,
		                                       true,
		                                       0.0f);
	}
}

void ACPP_VictoryFirework::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(NiagaraComponent) && NiagaraComponent->OnSystemFinished.IsAlreadyBound(
		this, &ACPP_VictoryFirework::OnFireworkSystemFinished))
	{
		NiagaraComponent->OnSystemFinished.RemoveDynamic(this, &ACPP_VictoryFirework::OnFireworkSystemFinished);
	}
	ClearSoundSpawningTimer();
	Super::EndPlay(EndPlayReason);
}

void ACPP_VictoryFirework::SpawnSound()
{
	FireworkExplosionsCounter++;
	if (IsValid(NiagaraComponent) && NiagaraComponent->IsActive() && FireworkExplosionsCounter <= 5)
	{
		if (SoundsToPlay.Num() >= FireworkExplosionsCounter)
		{
			if (SoundsToPlay[FireworkExplosionsCounter - 1])
			{
				if (IsValid(FireworkSoundCue1))
				{
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireworkSoundCue1, SpawnLocation);
				}
			}
			else
			{
				if (IsValid(FireworkSoundCue0))
				{
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireworkSoundCue0, SpawnLocation);
				}
			}
		}
	}
	else
	{
		ClearSoundSpawningTimer();
	}
}

void ACPP_VictoryFirework::OnFireworkSystemFinished(UNiagaraComponent* PSystem)
{
	ClearSoundSpawningTimer();
}

void ACPP_VictoryFirework::ClearSoundSpawningTimer()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_SoundSpawning))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_SoundSpawning);
	}
}
