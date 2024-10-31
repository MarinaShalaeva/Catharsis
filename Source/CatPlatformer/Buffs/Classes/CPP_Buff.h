// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
class USlateBrushAsset;
class USceneComponent;
class UStaticMeshComponent;
class UCurveFloat;

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

#include "CPP_Buff.generated.h"

/**
 * Parent class for storing logic of the buffs' behavior.
 */
UCLASS(Abstract)
class CATPLATFORMER_API ACPP_Buff : public AActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_Buff();

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

public:
	/**
	 * Function for initializing variables before this actor
	 * appears in the game world.
	 * Should be called before BeginPlay() (between
	 * SpawnActorDeferred() and FinishSpawningActor()).
	 */
	UFUNCTION(Server, Reliable)
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

protected:
	/**
	 * Function for applying the buff's property and removing
	 * it from the map.
	 * @param Character Character who collected the buff.
	 */
	UFUNCTION(Server, Reliable)
	virtual void CollectBuff(ACPP_Character* Character);
	virtual void CollectBuff_Implementation(ACPP_Character* Character);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_CollectBuff(ACPP_Character* Character);
	virtual void Multicast_CollectBuff_Implementation(ACPP_Character* Character);

	//==================Scene Components===========================
	/**
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Buff Actor | Components", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/**
	 * The static mesh component for the buff appearance in
	 * the game world.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Buff Actor | Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* SM_Buff;

	//==============Information about the buff=====================
public:
	/**
	 * The identification number of the buff type (for all
	 * instances of one particular child class this id will
	 * be the same).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff Actor | Variables", meta = (AllowPrivateAccess = true))
	int32 BuffTypeId;

	/**
	 * The number of seconds during which this buff should
	 * be active.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff Actor | Variables", meta = (AllowPrivateAccess = true))
	float EffectDuration;

	/**
	 * The picture that represents the buff inside the level
	 * widget.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff Actor | Variables", meta = (AllowPrivateAccess = true))
	USlateBrushAsset* BuffImage;

	/**
	 * The score that should be added to player who collect
	 * this buff. Can be negative.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff Actor | Variables", meta = (AllowPrivateAccess = true))
	int32 ScoreToAdd;

	//===========Rotating the actor around its axis================
private:
	/** Timer Handle for the actor's rotation. */
	FTimerHandle TH_BuffRotation;

protected:
	/** The actor's rotation speed. */
	UPROPERTY(EditAnywhere, Category = "Buff Rotation", meta = (AllowPrivateAccess = true))
	float BuffRotationSpeed;

private:
	/**
	 * Function for rotating the actor around its own axis.
	 * Will be called by a looped timer.
	 */
	UFUNCTION()
	void RotateBuffAroundItsAxis();

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
	UPROPERTY(EditAnywhere, Category = "Smooth Z Movement | Timeline")
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

	/**
	 * Boolean flag for storing current smooth movement
	 * direction.
	 */
	bool bIsMovingUp;

protected:
	/**
	 * The offset for playing the animation of the actor's
	 * smooth movement up and down.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smooth Z Movement | Variables")
	float ZPositionOffset;

private:
	/**
	 * Function for the interpolation of the actor's smooth
	 * movement up and down.
	 */
	UFUNCTION(Unreliable, NetMulticast)
	void SmoothZMovementTimelineProgress(float Value);
	void SmoothZMovementTimelineProgress_Implementation(float Value);

	/**
	 * Function for changing the Z movement direction after
	 * reaching the end of the timeline.
	 */
	UFUNCTION()
	void SmoothZMovementTimelineEnded();
};
