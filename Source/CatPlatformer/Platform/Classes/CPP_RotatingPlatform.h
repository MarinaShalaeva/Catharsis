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
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/** 
	 * Function that is called every frame.
	 * Is needed for updating platform's location.
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UFUNCTION(Server, Reliable)
	void InitialPlatformRotation();
	void InitialPlatformRotation_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InitialPlatformRotation();
	void Multicast_InitialPlatformRotation_Implementation();

	/**
	 * Function for rotating the platform. Is called from the
	 * tick event.
	 */
	UFUNCTION(Server, Reliable)
	void RotatePlatform(const float& DeltaSeconds);
	void RotatePlatform_Implementation(const float& DeltaSeconds);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RotatePlatform(const float& DeltaSeconds);
	void Multicast_RotatePlatform_Implementation(const float& DeltaSeconds);

protected:
	/** Timer handle for platform's rotation. */
	// FTimerHandle TH_Rotation;

	/** Axis around which the platform should rotate. */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FString Coordinate;

	/** Rotation speed. */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float Speed;

	/** Direction of rotation. Can be positive or negative. */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	int32 Direction;
};
