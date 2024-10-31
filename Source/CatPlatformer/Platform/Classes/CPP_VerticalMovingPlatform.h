// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "CPP_Platform.h"
#include "Components/TimelineComponent.h"
class UCurveFloat;
#include "CPP_VerticalMovingPlatform.generated.h"

/**
 * C++ class representing a platform that moves up and down
 * along the Z axis. 
 */
UCLASS()
class CATPLATFORMER_API ACPP_VerticalMovingPlatform : public ACPP_Platform
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_VerticalMovingPlatform();

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

	/**
	 * Function for initializing variables before this
	 * actor appears in the game world.
	 * Should be called before BeginPlay() (between
	 * SpawnActorDeferred() and FinishSpawningActor()).
	 */
	virtual void InitializeBasicVariables_Implementation(const FVector StartLocation) override;
	
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
