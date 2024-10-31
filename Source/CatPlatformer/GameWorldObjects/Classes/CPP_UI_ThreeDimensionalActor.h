// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CPP_UI_ThreeDimensionalActor.generated.h"

/**
 * Actor that is needed to be shown as three-dimensional
 * object in widgets.
 */
UCLASS(Abstract)
class CATPLATFORMER_API ACPP_UI_ThreeDimensionalActor : public AActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_UI_ThreeDimensionalActor();

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
	 * Function that is called after the C++ constructor and
	 * after the properties have been initialized and is
	 * needed to set new values to the components variables.
	 */
	virtual void PostInitProperties() override;

	//==================Scene Components===========================

	/**
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "UI Actor | Components", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/**
	 * Scene component for storing static or skeletal mesh
	 * and for performing correct rotation by Z-axis.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI Actor | Components",
		meta = (AllowPrivateAccess = true))
	USceneComponent* ComponentToRotate;

	/**
	 * Scene Capture for correct actor's representation in
	 * widgets.
	 */
	UPROPERTY(VisibleAnywhere, Category = "UI Actor | Components", meta = (AllowPrivateAccess = true))
	USceneCaptureComponent2D* SceneCaptureComponent2D;

	//===========Rotating the actor around its axis================
private:
	/** Timer Handle for the actor's rotation. */
	FTimerHandle TH_SceneComponentRotation;

protected:
	/**
	 * Flag indicating if the Scene Component that contains
	 * three-dimensional object should rotate.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Actor | Variables")
	bool bShouldRotate;

	/** The actor's rotation speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Actor | Variables")
	float SceneComponentRotationSpeed;

	/** Axis around which the actor should rotate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Actor | Variables")
	FString RotationAxis;

private:
	/**
	 * Function for rotating the Scene Component that contains
	 * three-dimensional object around its own X axis.
	 * Should be called by a looped timer.
	 */
	UFUNCTION()
	void RotateThreeDimensionalObjectAxisX() const;
	/**
	 * Function for rotating the Scene Component that contains
	 * three-dimensional object around its own Y axis.
	 * Should be called by a looped timer.
	 */
	UFUNCTION()
	void RotateThreeDimensionalObjectAxisY() const;
	/**
	 * Function for rotating the Scene Component that contains
	 * three-dimensional object around its own Z axis.
	 * Should be called by a looped timer.
	 */
	UFUNCTION()
	void RotateThreeDimensionalObjectAxisZ() const;
};
