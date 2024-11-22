// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "CPP_VictoryFirework.generated.h"

/** Actor representing the victory firework. */
UCLASS()
class CATPLATFORMER_API ACPP_VictoryFirework : public AActor
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_VictoryFirework();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the actor's life (but before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	/** Firework sound related to the index = 0. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firework | Sound")
	USoundBase* FireworkSoundCue0;

	/** Firework sound related to the index = 1. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firework | Sound")
	USoundBase* FireworkSoundCue1;

	/**
	 * Particle system for firework that should be spawn in
	 * the end on the level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firework | Niagara")
	UNiagaraSystem* NS_Firework;

private:
	/** Component related to the current Niagara System. */
	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;

public:
	/** Location to spawn sound and the niagara effect. */
	FVector SpawnLocation;

	/**
	 * Array with flags indicating which firework sound
	 * should be spawned in every new explosion.
	 */
	TArray<bool> SoundsToPlay;

private:
	/** Current number of firework explosions. */
	int8 FireworkExplosionsCounter;

	/** Timer handle for sound spawning. */
	FTimerHandle TH_SoundSpawning;

	/** Function for spawning a firework sound. */
	void SpawnSound();

	/**
	 * Function that is called when the Niagara System
	 * finishes its work.
	 */
	UFUNCTION()
	void OnFireworkSystemFinished(UNiagaraComponent* PSystem);

	/** Function for clearing the timer. */
	void ClearSoundSpawningTimer();
};
