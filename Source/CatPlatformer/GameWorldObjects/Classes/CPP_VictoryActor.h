// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
class USceneComponent;
class UStaticMeshComponent;
class UCurveFloat;

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

#include "CPP_VictoryActor.generated.h"

/**
 * Parent class for storing logic of the actor, that
 * should be collected to successfully end the level.
 */
UCLASS(Abstract)
class CATPLATFORMER_API ACPP_VictoryActor : public AActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_VictoryActor();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied
	 * in the end of the actor's life (but before its
	 * destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/**
	 * Function for initializing variables before this actor
	 * appears in the game world.
	 * Should be called before BeginPlay() (between
	 * SpawnActorDeferred() and FinishSpawningActor()).
	 */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void InitializeBasicVariables(const FVector& StartLocation);
	void InitializeBasicVariables_Implementation(const FVector& StartLocation);
	
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
	void StaticMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

	/**
	 * Function for collecting the victory static mesh and
	 * for calling the event of ending the level.
	 * @param Character Character who collected this actor.
	 */
	UFUNCTION(Server, Reliable)
	virtual void CollectStaticMesh(ACPP_Character* Character);

private:
	virtual void CollectStaticMesh_Implementation(ACPP_Character* Character);

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_CollectStaticMesh(ACPP_Character* Character);
	virtual void Multicast_CollectStaticMesh_Implementation(ACPP_Character* Character);

	//==================Scene Components===========================
protected:
	/**
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Victory Actor | Components", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/**
	 * The static mesh component for the buff appearance in
	 * the game world.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Victory Actor | Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* SM_Base;

	UPROPERTY(EditAnywhere, Category = "Victory Actor | Victory Params", BlueprintReadOnly)
	int32 ScoreToAdd;

	//====================Spawn Position===========================
public:
	/**
	 * Offset for correct actor's spawning on the final
	 * platform.
	 */
	UPROPERTY(EditAnywhere, Category = "Victory Actor | Spawn Params", BlueprintReadOnly)
	FVector SpawningOffset;

	//===========Rotating the actor around its axis================
private:
	/** Timer Handle for the actor's rotation. */
	FTimerHandle TH_Rotation;

	/** The actor's rotation speed. */
	UPROPERTY(EditAnywhere, Category = "Victory Actor | Rotation", meta = (AllowPrivateAccess = true))
	float RotationSpeed;

	/**
	 * Function for rotating the actor around its own axis.
	 * Will be called by a looped timer.
	 */
	UFUNCTION()
	void RotateActorAroundItsAxis();

	//=========Timeline for vertical actor's position==============
public:
	/**
	 * Timeline component for playing animation of the actor's
	 * smooth movement up and down.
	 */
	UPROPERTY()
	UTimelineComponent* TimelineComp;

private:
	/**
	 * Delegate for playing animation of the actor's smooth
	 * movement up and down.
	 */
	FOnTimelineFloat SmoothZMovementProgressDelegate;

	/**
	 * Delegate for storing functions, that will be called
	 * after the Smooth Z Movement timeline playback is
	 * completed.
	 */
	FOnTimelineEvent SmoothZMovementTimelineEndedDelegate;

protected:
	/**
	 * Curve for playing animation of the actor's smooth
	 * movement up and down.
	 */
	UPROPERTY(EditAnywhere, Category = "Victory Actor | Smooth Z Movement")
	UCurveFloat* SmoothZMovementCurveFloat;

private:
	/**
	 * The start actor's location for playing the animation
	 * of the smooth movement up and down.
	 */
	UPROPERTY(Replicated)
	FVector StartPosition;

	/**
	 * The end actor's location for playing the animation of
	 * the smooth movement up and down.
	 */
	UPROPERTY(Replicated)
	FVector EndPosition;

protected:
	/**
	 * The offset for playing the animation of the actor's
	 * smooth movement up and down.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Victory Actor | Smooth Z Movement")
	float ZPositionOffset;

private:
	/**
	 * Function for the interpolation of the actor's smooth
	 * movement up and down.
	 */
	UFUNCTION(Unreliable, NetMulticast)
	void SmoothZMovementTimelineProgress(float Value);
	void SmoothZMovementTimelineProgress_Implementation(float Value);
};
