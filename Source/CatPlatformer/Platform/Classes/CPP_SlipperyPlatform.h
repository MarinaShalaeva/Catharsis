// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_PLATFORM_H
#define CPP_PLATFORM_H
#include "CatPlatformer/Platform/Classes/CPP_Platform.h"
#endif
class ACPP_Platform;

#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
class UTimelineComponent;
class UBoxComponent;

#include "CPP_SlipperyPlatform.generated.h"

/**
 * C++ class representing a slippery platform. 
 */
UCLASS(Blueprintable)
class CATPLATFORMER_API ACPP_SlipperyPlatform : public ACPP_Platform
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_SlipperyPlatform();

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
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* IceAudioComponent;

	/**
	 * Function for initializing variables before this
	 * actor appears in the game world.
	 * Should be called before BeginPlay() (between
	 * SpawnActorDeferred() and FinishSpawningActor()).
	 */
	virtual void InitializeBasicVariables_Implementation(const FVector StartLocation) override;

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

	/**
	 * Function for turning on or off the ice sound. 
	 * @param bTurnOn If true, sound should start playing.
	 * If false, sound should stop playing.
	 */
	UFUNCTION(Client, Unreliable)
	void SwitchSoundState(const bool bTurnOn);
	void SwitchSoundState_Implementation(const bool bTurnOn);

	/**
	 * Collision box to check if the player has stepped on
	 * the surface of the platform.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	//======================Timeline===============================

	/**
	 * Timeline component for playing animation of the
	 * platform's shaking.
	 */
	UPROPERTY()
	UTimelineComponent* TimelineComp;

private:
	/**
	 * Delegate for playing animation of the platform's
	 * circular rotation.
	 */
	UPROPERTY()
	FOnTimelineVector TimelineProgressDelegate;

protected:
	/**
	 * Curve for playing animation of the platform's circular
	 * rotation.
	 */
	UPROPERTY(EditAnywhere, Category = "Circular Rotation | Timeline")
	UCurveVector* CurveVector;

private:
	/**
	 * The start platform's rotation for playing the circular
	 * rotation animation.
	 */
	FRotator StartRotation;

	/**
	 * The end platform's rotation for playing the circular
	 * rotation animation.
	 */
	FRotator EndRotation;

protected:
	/**
	 * The offset for the platform's circular rotation
	 * amplitude.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Circular Rotation | Variables")
	float CircularRotationOffset;

	/**
	 * Type of the platform appearance. If true, basic
	 * platform material should be changed to another one.
	 */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bPlatformAppearanceType;

private:
	/**
	 * Function for the platform's circular rotation
	 * interpolation.
	 */
	UFUNCTION()
	void CircularRotationTimelineProgress(FVector Value);
};
