// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_PLATFORM_H
#define CPP_PLATFORM_H
#include "CatPlatformer/Platform/Classes/CPP_Platform.h"
#endif
class ACPP_Platform;

#include "CPP_RotatingPlatform.generated.h"

/**
 * C++ class representing a platform that should rotate around
 * its axis throughout its existence on the level. 
 */
UCLASS(Blueprintable)
class CATPLATFORMER_API ACPP_RotatingPlatform : public ACPP_Platform
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_RotatingPlatform();

	/**
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** 
	 * Function that is called every frame.
	 * Is needed for updating platform's location.
	 */
	virtual void Tick(float DeltaSeconds) override;

	/** Flag indicating if basic variables were initialized. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bBasicVariablesWereInitialized;

	/**
	 * Function for initializing variables before this
	 * actor appears in the game world.
	 * Should be called before BeginPlay() (between
	 * SpawnActorDeferred() and FinishSpawningActor()).
	 */
	virtual void InitializeBasicVariables_Implementation(const FVector StartLocation) override;

	/** Function for starting the platform's rotation. */
	virtual void ApplyPlatformProperty() override;

private:
	/**
	 * Function for one-time platform's rotating to make it
	 * look like Y-axis rotating object.
	 */
	UFUNCTION()
	void InitialPlatformRotation() const;

	/**
	 * Function for rotating the platform. Is called from the
	 * tick event.
	 */
	UFUNCTION(Server, Reliable)
	void RotatePlatform(const float DeltaSeconds);
	void RotatePlatform_Implementation(const float DeltaSeconds);

protected:
	/**
	 * Axis around which the platform should be rotated.
	 * true = X; false = Z.
	 * Y is not used because of strange performance. To
	 * simulate Y rotation the platform's component should
	 * be rotated by 90 degree at the beginning of the
	 * actor's life.
	 */
	bool bAxis;

	/**
	 * Flag indicating if the platform should simulate Y-axis
	 * rotating.
	 */
	UPROPERTY(Replicated)
	bool bUseAxisY;

	/** Rotation speed. */
	float Speed;

	/** Direction of rotation. Can be positive or negative. */
	int8 Direction;
};
