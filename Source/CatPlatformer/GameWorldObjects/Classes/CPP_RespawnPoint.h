// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
class UBoxComponent;

#ifndef CPP_GAMEMODE_H
#define CPP_GAMEMODE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameMode.h"
#endif
class ACPP_GameMode;
class ACPP_GameState;

#include "CPP_RespawnPoint.generated.h"

/**
 * Actor for the player's respawn in case he falls off the
 * platform.
 */
UCLASS()
class CATPLATFORMER_API ACPP_RespawnPoint : public AActor
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_RespawnPoint();

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
	/** Root for actor's components. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	/**
	 * Collision box to check if the player reached the
	 * point on the map from which he needs to be moved
	 * to the starting position.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	/** Weak pointer to the instance of ACPP_GameMode class. */
	TWeakObjectPtr<ACPP_GameMode> GameModeRef;
	/** Weak pointer to the instance of ACPP_GameState class. */
	TWeakObjectPtr<ACPP_GameState> GameStateRef;

	/**
	 * Function for replying to the start of overlap with a
	 * static mesh component.
	 * @param OverlappedComponent The component whose
	 * intersection we are checking (CollisionBox).
	 * @param OtherActor The actor who overlapped the
	 * component being checked.
	 * @param OtherComp The component that overlaps with the
	 * component being checked.
	 * @param OtherBodyIndex Refers to the index of the
	 * specific primitive component of the other actor that
	 * the current actor is overlapping with.
	 * This is useful when the other actor has multiple
	 * primitive components, and you want to know which one
	 * is causing the overlap.
	 * @param bFromSweep It returns True or False, depending
	 * on whether something overlaps the collision.
	 * @param SweepResult Hit result structure needed to
	 * access some additional outputs.
	 */
	UFUNCTION()
	void CollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								  const FHitResult& SweepResult);
};
