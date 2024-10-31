// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
class USceneComponent;
class UStaticMeshComponent;
#include "CPP_Platform.generated.h"

/**
 * Parent class for platforms that should be spawn on the
 * map in the beginning of every level.
 */
UCLASS(Abstract)
class CATPLATFORMER_API ACPP_Platform : public AActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_Platform();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Function for initializing variables before this
	 * actor appears in the game world.
	 * Should be called before BeginPlay() (between
	 * SpawnActorDeferred() and FinishSpawningActor()).
	 */
	UFUNCTION(Server, Reliable)
	virtual void InitializeBasicVariables(const FVector StartLocation);
	virtual void InitializeBasicVariables_Implementation(const FVector StartLocation);

protected:
	/**
	 * Function for to launch the main feature of the
	 * current platform type.
	 */
	UFUNCTION()
	virtual void ApplyPlatformProperty()/* PURE_VIRTUAL(ACPP_Platform::ApplyPlatformProperty, )*/;

	/** Root for actor's components. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	/** Static mesh for the platform's representation. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlatformBase;
};
