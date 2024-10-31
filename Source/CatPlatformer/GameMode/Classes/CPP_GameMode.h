// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

#ifndef CPP_PLATFORMSPAWNER_H
#define CPP_PLATFORMSPAWNER_H
#include "CatPlatformer/Platform/Classes/CPP_PlatformSpawner.h"
#endif
class UCPP_PlatformSpawner;
class ACPP_Platform;
class ACPP_Buff;
class ACPP_VictoryActor;

#include "CPP_GameMode.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(FUniqueNetIdRepl, FCallRegisterPlayer,
                                 const APlayerController* /* InPlayerController */);

DECLARE_DELEGATE_TwoParams(FCallUnregisterPlayer,
                           const APlayerController* /* PlayerController */,
                           const FUniqueNetIdRepl& /* UniqueNetId */);

DECLARE_DELEGATE_OneParam(FCallRegisterAllExistingPlayers, const UWorld* /* World */);

DECLARE_DELEGATE_RetVal_OneParam(bool, FStartTravel,
                                 const bool /* bIsSeamless */);

/**
 * C++ Game Mode class for storing the «Cat Platformer» game
 * rules and information about base classes that will be
 * possessed in the game world.
 */
UCLASS()
class CATPLATFORMER_API ACPP_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	/**
	 * The constructor to set base classes that will be
	 * possessed in world.
	 */
	ACPP_GameMode();

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
	 * Function for choosing the place to spawn the player
	 * on the map.
	 * @param Player Player that should be placed on the
	 * map.
	 * @return Reference to the player start actor.
	 */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/**
	 * Function that is called after a successful player's
	 * login.
	 * @param NewPlayer New player controller.
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	/**
	 * Function for unregister disconnected online player.
	 * @param InPlayerController Player to unregister.
	 */
	UFUNCTION()
	void PreLogout(const APlayerController* InPlayerController) const;

	/** Reference to the Game State. */
	UPROPERTY()
	ACPP_GameState* GameStateRef;

	/**
	 * Flag indicating if platforms and buffs were already
	 * spawned.
	 */
	bool bLevelWasGenerated;

	/**
	 * Delegate for calling register of all existing online
	 * players.
	 */
	FCallRegisterAllExistingPlayers CallRegisterAllExistingPlayersDelegate;

	/** Delegate for calling register of online player. */
	FCallRegisterPlayer CallRegisterPlayerDelegate;

	/** Delegate for calling unregister of online player. */
	FCallUnregisterPlayer CallUnregisterPlayerDelegate;

	/**
	 * Delegate for calling the seamless or non-seamless
	 * travel.
	 */
	FStartTravel StartTravelDelegate;

	/**
	 * Timer handle for calling Call Updating All Characters
	 * Appearance function.
	 */
	FTimerHandle TH_CallUpdatingAllCharactersAppearance;

	/**
	 * Calling function from the Game State class which is
	 * needed for all cats' colors updating.
	 */
	UFUNCTION(Server, Reliable)
	void CallUpdatingAllCharactersAppearance();
	void CallUpdatingAllCharactersAppearance_Implementation();

	/**
	 * Function for destroying the old character and for
	 * creating the new one.
	 * @param Controller The controller related to the old
	 * character (controller of the character who needs to
	 * be respawned).
	 */
	UFUNCTION(Server, Reliable)
	void RespawnPlayer(AController* Controller);

private:
	void RespawnPlayer_Implementation(AController* Controller);

public:
	/**
	 * Function for calling the process of ending session on
	 * clients.
	 */
	UFUNCTION(Server, Reliable)
	void CallSessionEnding();

private:
	void CallSessionEnding_Implementation();

public:
	/**
	 * Function for starting new level's creating.
	 * @param InLevelNumber Level number to generate.
	 */
	void CallLevelGeneration(const int32 InLevelNumber);

private:
	/**
	 * Function for calling platforms and buffs generating
	 * with particular parameters.
	 * @param PlatformsLength How many platforms should be
	 * in one row (X coordinate).
	 * @param PlatformsWidth How many platforms should be
	 * in one line (Y coordinate).
	 * @param PlatformsZCoordinateOffset Maximum offset for
	 * the Z coordinate of each platform.
	 * @param TotalBuffsNumber Number of buffs that should
	 * be generated on the level.
	 */
	void CallLevelGeneration(const uint32 PlatformsLength,
	                         const uint32 PlatformsWidth,
	                         const float PlatformsZCoordinateOffset,
	                         const uint32 TotalBuffsNumber);

	/**
	 * Function for calling the destruction on the loading
	 * screen.
	 */
	void StartDestroyingLoadingScreen();

protected:
	/** The distance between neighboring platforms. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnDistance;
	/**
	 * Soft references to platforms' classes that should be
	 * spawned on level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftClassPtr<ACPP_Platform>> PlatformClasses;
	/** Soft reference to the final platform class. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ACPP_Platform> FinalPlatform;
	/**
	 * Soft references to buffs' classes that should be
	 * spawned on level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftClassPtr<ACPP_Buff>> BuffsClasses;
	/**
	 * An array with probabilities with which buffs from the
	 * BuffsClasses array can be spawned on the map.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> BuffsSelectionProbabilities;
	/**
	 * Soft reference to the class of actor, that should be
	 * collected to end the level.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ACPP_VictoryActor> VictoryActorClass;

private:
	/** Reference to the platform spawner object. */
	UPROPERTY()
	UCPP_PlatformSpawner* PlatformSpawner;

	/** Timer for calling loading screens destroying. */
	FTimerHandle TH_CallLoadingScreenDestroying;
};
