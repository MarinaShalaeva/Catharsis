// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "CPP_UI_ThreeDimensionalActor.h"

#include "Components/TimelineComponent.h"
class UTimelineComponent;

#include "CPP_UI_ThreeDimensionalObjectWithCurve.generated.h"

UCLASS()
class CATPLATFORMER_API ACPP_UI_ThreeDimensionalObjectWithCurve : public ACPP_UI_ThreeDimensionalActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_UI_ThreeDimensionalObjectWithCurve();

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

	/** Static mesh for the platform's representation. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI Actor | Components",
		meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlatformBase;

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
	 * shaking.
	 */
	FOnTimelineVector TimelineProgressDelegate;

protected:
	/**
	 * Curve for playing animation of the platform's shaking.
	 */
	UPROPERTY(EditAnywhere, Category = "UI Actor | Timeline", meta = (AllowPrivateAccess = true))
	UCurveVector* CurveVector;

private:
	/**
	 * The start platform's rotation for playing the shaking
	 * animation.
	 */
	FRotator StartRotation;

	/**
	 * The end platform's rotation for playing the shaking
	 * animation.
	 */
	FRotator EndRotation;

protected:
	/** The offset for the platform's shaking amplitude. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Actor | Variables")
	float ShakingOffset;

private:
	/** Function for the platform's shaking interpolation. */
	UFUNCTION()
	void ShakingTimelineProgress(FVector Value);
};
