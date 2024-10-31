// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_PLATFORM_H
#define CPP_PLATFORM_H
#include "CatPlatformer/Platform/Classes/CPP_Platform.h"
#endif
class ACPP_Platform;

#include "GameFramework/Character.h"
class ACharacter;

#ifndef CPP_ENEMYCHARACTER_H
#define CPP_ENEMYCHARACTER_H
#include "CatPlatformer/AI/Enemy/Classes/CPP_EnemyCharacter.h"
#endif
class ACPP_EnemyCharacter;
class USceneComponent;
class UBoxComponent;

#include "CPP_PlatformWithNPC.generated.h"

/**
 * Platform with some NPC on it.
 */
UCLASS(Blueprintable)
class CATPLATFORMER_API ACPP_PlatformWithNPC : public ACPP_Platform
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_PlatformWithNPC();

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

	/**
	 * Function for replying to the end of overlap with a
	 * box component.
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
	 */
	UFUNCTION()
	void CollisionBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Timer handle for calling enemy spawning. */
	FTimerHandle TH_SpawnCrow;

	UFUNCTION()
	void SpawnCrow();

	/**
	 * Object for getting location and rotation for spawning
	 * NPC.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Target_For_NPC_Spawning;

	/** Collision box for detecting characters. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	/** Class of NPC that should be spawned on the platform. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	TSubclassOf<ACharacter> NPC_Class;

	UPROPERTY()
	ACPP_EnemyCharacter* EnemyRef;
};
