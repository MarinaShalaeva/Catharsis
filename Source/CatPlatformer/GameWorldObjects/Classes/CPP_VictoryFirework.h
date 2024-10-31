// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
class UNiagaraSystem;
class UNiagaraComponent;
class USoundBase;
#include "CPP_VictoryFirework.generated.h"

/** Actor for spawning the victory firework. */
UCLASS()
class CATPLATFORMER_API ACPP_VictoryFirework : public AActor
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_VictoryFirework();

protected:
	/**
	 * Particle system for firework that should be spawn in
	 * the end on the level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Firework | Niagara")
	UNiagaraSystem* NS_Firework;
};
