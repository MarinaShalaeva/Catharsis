// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;
class ACPP_PlayerState;

#ifndef CPP_VICTORYFIREWORK_H
#define CPP_VICTORYFIREWORK_H
#include "CatPlatformer/GameWorldObjects/Classes/CPP_VictoryFirework.h"
#endif
class ACPP_VictoryFirework;

#include "CPP_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeSkyMaterial, int32, NewMaterialIndex);

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayersNumberChanged, const int32 /* New Players Number */)

/**
 * C++ parent Game State class.
 */
UCLASS()
class CATPLATFORMER_API ACPP_GameState : public AGameState
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_GameState();

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
	 * Delegate for notifying that players number on server
	 * was changed.
	 */
	FPlayersNumberChanged PlayersNumberChangedDelegate;

	/**
	 * Event dispatcher for updating the sky material after
	 * changing the level's number.
	 */
	UPROPERTY(BlueprintAssignable)
	FChangeSkyMaterial ED_ChangeSkyMaterial;

private:
	/** Current level's number. */
	UPROPERTY(Replicated)
	int32 LevelNumber;

public:
	/** Setter for the LevelNumber variable. */
	UFUNCTION(Server, Reliable)
	void SetLevelNumber(const int32 NewValue);
	void SetLevelNumber_Implementation(const int32 NewValue);

	/** Getter for the LevelNumber variable. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetLevelNumber() const { return LevelNumber; }

private:
	/**
	 * Index of the material that should be relevant for the
	 * current level.
	 */
	UPROPERTY(Replicated)
	int32 SkyMaterialIndex;

public:
	/** Getter for the SkyMaterialIndex variable. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSkyMaterialIndex() const { return SkyMaterialIndex; }

private:
	/**
	 * Function that is called for adding new player state
	 * to the list with actual players.
	 * Is needed for binding player state's delegate.
	 * @param PlayerState Player State to add.
	 */
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddPlayerState(APlayerState* PlayerState);
	void Multicast_AddPlayerState_Implementation(APlayerState* PlayerState);

	/**
	 * Function that is called for removing the player state
	 * from the list with actual players.
	 * Is needed for unbinding player state's delegate.
	 * @param PlayerState Player State to remove.
	 */
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RemovePlayerState();
	void Multicast_RemovePlayerState_Implementation();

public:
	/** Function for calling all cats' colors updating. */
	UFUNCTION(Server, Reliable)
	void UpdateAllCharactersAppearance();
	void UpdateAllCharactersAppearance_Implementation();

	/**
	 * Function for calling the destroying of loading
	 * screen from all players' HUDs.
	 */
	UFUNCTION(Server, Reliable)
	void CallLoadingScreenClosing();

private:
	void CallLoadingScreenClosing_Implementation();

public:
	/**
	 * Function that should be called after any player tells
	 * that he is ready to start the level.
	 * @param PlayerState Player State of the player who is
	 * ready to start the level.
	*/
	UFUNCTION(Server, Reliable)
	void PlayerCanStartTheLevel(ACPP_PlayerState* PlayerState);

private:
	void PlayerCanStartTheLevel_Implementation(ACPP_PlayerState* PlayerState);

	/**
	 * Function for notifying other classes that all players
	 * are ready to start the level.
	*/
	UFUNCTION(Server, Reliable)
	void NotifyThatLevelCanBeStarted();
	void NotifyThatLevelCanBeStarted_Implementation();

public:
	/**
	 * Function for adding (or removing) the player's score
	 * points.
	 */
	UFUNCTION(Server, Reliable)
	void AddUserScore(ACPP_PlayerState* PlayerState, const int32 ScoreToAdd);

private:
	void AddUserScore_Implementation(ACPP_PlayerState* PlayerState, const int32 ScoreToAdd);

public:
	/**
	 * Function that is called after one of the players
	 * collects the victory item and the level ends.
	 */
	UFUNCTION(Server, Reliable)
	void LevelWasEnded();

private:
	void LevelWasEnded_Implementation();

	/**
	 * Function for getting the winner (or winners) of current
	 * level.
	 * @param OutWinners An array of Player States of winners.
	 */
	UFUNCTION()
	void GetTheWinner(TArray<ACPP_PlayerState*>& OutWinners);

public:
	/**
	 * Function for calling the process of ending session on
	 * clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CallSessionEnding();

private:
	void Multicast_CallSessionEnding_Implementation();

protected:
	/**
	 * Blueprint class representing the firework that should
	 * be spawned nearby the winners.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Victory")
	TSubclassOf<ACPP_VictoryFirework> VictoryFireworkClass;

private:
	/**
	 * Function for spawning the firework nearby the winners. 
	 * @param InWinners The list of winners.
	 */
	UFUNCTION(Server, Reliable)
	void SpawnVictoryFireworks(const TArray<ACPP_PlayerState*>& InWinners);
	void SpawnVictoryFireworks_Implementation(const TArray<ACPP_PlayerState*>& InWinners);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnVictoryFireworks(const TArray<bool>& SoundsToPlay,
	                                     const TArray<FVector>& FireworkLocations);
	void Multicast_SpawnVictoryFireworks_Implementation(const TArray<bool>& SoundsToPlay,
	                                                    const TArray<FVector>& FireworkLocations);

public:
	/**
	 * Function for staring a non-seamless travel for the
	 * level with online players.
	 */
	UFUNCTION(Server, Reliable)
	void StartNonSeamlessTravel();

private:
	void StartNonSeamlessTravel_Implementation();
};
