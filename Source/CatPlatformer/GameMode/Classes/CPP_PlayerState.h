// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticLibrary.h"
#endif

#include "CPP_PlayerState.generated.h"

class ACPP_PlayerState;

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerIsReadyForGame, ACPP_PlayerState*);

DECLARE_MULTICAST_DELEGATE(FShouldBeginCountdownToStartLevel);

DECLARE_MULTICAST_DELEGATE_OneParam(FDestroyLoadingScreen, const bool /* bShouldHideBackgroundImage */);

DECLARE_MULTICAST_DELEGATE_OneParam(FScoreChanged,
                                    const int32 /* ScoreToAdd */);

DECLARE_MULTICAST_DELEGATE_OneParam(FLevelWasEnded, const bool /* Is Player A Winner? */);

DECLARE_DELEGATE_OneParam(FTimeInGameChanged, const uint32 /* New Time */)

UCLASS()
class CATPLATFORMER_API ACPP_PlayerState : public APlayerState
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_PlayerState();

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
	 * Weak pointer to the instance of UCPP_GameInstance
	 * class.
	 */
	TWeakObjectPtr<UCPP_GameInstance> GameInstanceRef;

public:
	/**
	 * Has the player indicated that he is ready to start
	 * the game?
	 */
	UPROPERTY(Replicated)
	bool bIsReadyToStart;

	/**
	 * Delegate for notifying Game Mode class that player
	 * is ready to start playing.
	 */
	FPlayerIsReadyForGame PlayerIsReadyForGameDelegate;
	/**
	 * Delegate for notifying HUD that all players are ready
	 * to start the level and decreasing countdown can be begun.
	 */
	FShouldBeginCountdownToStartLevel ShouldBeginCountdownToStartLevelDelegate;
	/**
	 * Delegate for notifying HUD that the player's loading
	 * screen should be destroyed.
	 */
	FDestroyLoadingScreen DestroyLoadingScreenDelegate;
	/**
	 * Delegate for notifying widgets that user's score was
	 * changed.
	 */
	FScoreChanged ScoreChangedDelegate;
	/**
	 * Delegate for notifying HUD that level was ended.
	 * const bool — Is Player A Winner?
	 */
	FLevelWasEnded LevelWasEndedDelegate;
	/**
	 * Delegate for notifying statistics widget that value
	 * of TimeInGame variable was changed.
	 */
	FTimeInGameChanged TimeInGameChangedDelegate;

	/**
	 * Flag indicating if save data was loaded or saved at
	 * least once.
	 */
	bool bSaveFileWasCreated;

	/**
	 * Function to apply values from save file to the Player
	 * State.
	 */
	bool LoadInfoFromSaveFile();

	/** Function for saving current data structure to file. */
	UFUNCTION(Client, Reliable)
	void SaveDataToFile();
	void SaveDataToFile_Implementation();

	/**
	 * Function for saving info about SFX and Music volumes
	 * and current audio tracks' state.
	 */
	void CollectAudioInfoForSavingItToFile();

	/**
	 * Getter for the bIsReadyToStart variable.
	 * @return bIsReadyToStart variable value.
	 */
	UFUNCTION()
	bool GetPlayerIsReadyToStartGame() const { return bIsReadyToStart; }

	/**
	 * Function for changing value of the bIsReadyToStart
	 * variable.
	 * @param bIsReady Is player ready to start the level?
	 */
	UFUNCTION(Server, Reliable)
	void SetPlayerIsReadyToStartGame(const bool bIsReady);

private:
	void SetPlayerIsReadyToStartGame_Implementation(const bool bIsReady);

	/** Structure for storing information about the player. */
	UPROPERTY(Replicated)
	FSaveSlot PlayerData;

public:
	/**
	 * Function for creating the SaveSlot struct with current
	 * saved data.
	 * @param OutSaveSlot Saved data.
	 * @return Is the save slot valid?
	 */
	void GetTheSaveSlot(FSaveSlot& OutSaveSlot) const;

	/**
	 * Getter for the bSaveFileWasCreated variable.
	 * @return bSaveFileWasCreated variable.
	 */
	FORCEINLINE bool GetSaveFileWasCreated() const { return bSaveFileWasCreated; }

	/**
	 * Flag indicating if this player state is owned by the
	 * main player controller. Will be false for local
	 * players with index > 0 in local multiplayer mode;
	 * will be true otherwise.
	 */
	bool bIsFirstPlayer;

	/**
	 * Getter for the bIsFirstPlayer variable.
	 * @return bIsFirstPlayer variable.
	 */
	FORCEINLINE bool GetIsFirstPlayer() const { return bIsFirstPlayer; }

	/**
	 * Setter for the bIsFirstPlayer variable.
	 * @param bNewValue Value to set to bIsFirstPlayer
	 * variable.
	 */
	void SetIsFirstPlayer(const bool bNewValue);

	/**
	 * Getter for the UserName variable.
	 * @return UserName variable.
	 */
	FORCEINLINE FString GetUserName() const { return PlayerData.UserName; }

	/**
	 * Setter for the UserName variable.
	 * @param NewValue Value to set to UserName variable.
	 */
	void SetUserName(const FString& NewValue);

	/**
	 * Function for getting the MaxOpenedLevelNumber variable.
	 * @return The highest opened level's number (amount of
	 * opened levels).
	 */
	FORCEINLINE uint8 GetMaxOpenedLevelNumber() const { return PlayerData.MaxOpenedLevelNumber; }

	/**
	 * Setter for the MaxOpenedLevelNumber variable.
	 * @param NewValue New maximum opened level number.
	 */
	void SetMaxOpenedLevelNumber(const uint8 NewValue);

	/** Function for getting the CatColorIndex variable. */
	FORCEINLINE uint8 GetCatColorIndex() const { return PlayerData.CatColorIndex; }

	/** Setter for the CatColorIndex variable. */
	void SetCatColorIndex(const uint8 NewValue);

	/**
	 * Function for applying the saved color to the
	 * character's mesh.
	 */
	UFUNCTION(Server, Reliable)
	void UpdateCatsColorUpToSavedIndex() const;

private:
	void UpdateCatsColorUpToSavedIndex_Implementation() const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateCatsColorUpToSavedIndex() const;
	void Multicast_UpdateCatsColorUpToSavedIndex_Implementation() const;

public:
	/** Function for getting the NumberOfStartedLevels variable. */
	FORCEINLINE uint32 GetNumberOfStartedLevels() const { return PlayerData.NumberOfStartedLevels; }

	/** Setter for the NumberOfStartedLevels variable. */
	FORCEINLINE void SetNumberOfStartedLevels(const uint32 NewValue) { PlayerData.NumberOfStartedLevels = NewValue; }

	/**
	 * Function for increasing the NumberOfStartedLevels
	 * variable by one.
	 */
	FORCEINLINE void IncrementNumberOfStartedLevels() { PlayerData.NumberOfStartedLevels++; }

	/** Function for getting the NumberOfEndedLevels variable. */
	FORCEINLINE uint32 GetNumberOfEndedLevels() const { return PlayerData.NumberOfEndedLevels; }

	/** Setter for the NumberOfEndedLevels variable. */
	FORCEINLINE void SetNumberOfEndedLevels(const uint32 NewValue) { PlayerData.NumberOfEndedLevels = NewValue; }

	/**
	 * Function for increasing the NumberOfEndedLevels
	 * variable by one.
	 */
	FORCEINLINE void IncrementNumberOfEndedLevels() { PlayerData.NumberOfEndedLevels++; }

private:
	/** The number of player's deaths during current level. */
	uint32 CurrentLevelDeathsNumber;

public:
	/**
	 * Getter for the DeathsNumber variable.
	 * @return DeathsNumber variable.
	 */
	FORCEINLINE uint32 GetGeneralDeathsNumber() const { return PlayerData.DeathsNumber; }

	/**
	 * Setter for the DeathsNumber variable.
	 * @param NewValue Value to set to DeathsNumber variable.
	 */
	void SetGeneralDeathsNumber(uint32 NewValue);

	/**
	 * Getter for the CurrentLevelDeathsNumber variable.
	 * @return CurrentLevelDeathsNumber variable.
	 */
	FORCEINLINE uint32 GetCurrentLevelDeathsNumber() const { return CurrentLevelDeathsNumber; }

	/**
	 * Function for increasing the DeathsNumber variable by
	 * one.
	 */
	void IncrementDeathsNumber();

	/** Function for getting the JumpsNumber variable. */
	FORCEINLINE uint32 GetJumpsNumber() const { return PlayerData.JumpsNumber; }

	/** Setter for the JumpsNumber variable. */
	FORCEINLINE void SetJumpsNumber(const uint32 NewValue) { PlayerData.JumpsNumber = NewValue; }

	/**
	 * Function for increasing the JumpsNumber variable by
	 * one.
	 */
	FORCEINLINE void IncrementJumpsNumber() { PlayerData.JumpsNumber++; }

	/**
	 * Function for getting the CollectedBuffsNumber
	 * variable.
	 */
	FORCEINLINE uint32 GetCollectedBuffsNumber() const { return PlayerData.CollectedBuffsNumber; }

	/** Setter for the CollectedBuffsNumber variable. */
	FORCEINLINE void SetCollectedBuffsNumber(const uint32 NewValue) { PlayerData.CollectedBuffsNumber = NewValue; }

	/**
	 * Function for increasing the CollectedBuffsNumber
	 * variable by one.
	 */
	FORCEINLINE void IncrementCollectedBuffsNumber() { PlayerData.CollectedBuffsNumber++; }

	/** Function for getting the NPCsKilledNumber variable. */
	FORCEINLINE uint32 GetNPCsKilledNumber() const { return PlayerData.NPCsKilledNumber; }

	/** Setter for the NPCsKilledNumber variable. */
	FORCEINLINE void SetNPCsKilledNumber(const uint32 NewValue) { PlayerData.NPCsKilledNumber = NewValue; }

	/**
	 * Function for increasing the NPCsKilledNumber variable
	 * by one.
	 */
	FORCEINLINE void IncrementNPCsKilledNumber() { PlayerData.NPCsKilledNumber++; }

	/**
	 * Getter for the OnlineLevelsWinsNumber variable.
	 * @return OnlineLevelsWinsNumber variable.
	 */
	FORCEINLINE uint32 GetOnlineLevelsWinsNumber() const { return PlayerData.OnlineWinsNumber; }

	/**
	 * Setter for the OnlineLevelsWinsNumber variable.
	 * @param NewValue Value to set to OnlineLevelsWinsNumber
	 * variable.
	 */
	FORCEINLINE void SetOnlineLevelsWinsNumber(const uint32 NewValue) { PlayerData.OnlineWinsNumber = NewValue; }

	/**
	 * Function for increasing the OnlineLevelsWinsNumber variable by
	 * one.
	 */
	void IncrementOnlineLevelsWinsNumber();

	/** Getter for the OnlineLossesNumber variable. */
	FORCEINLINE uint32 GetOnlineLossesNumber() const { return PlayerData.OnlineLossesNumber; }

	/** Setter for the OnlineLossesNumber variable. */
	FORCEINLINE void SetOnlineLossesNumber(const uint32 NewValue) { PlayerData.OnlineLossesNumber = NewValue; }

	/**
	 * Function for increasing the OnlineLossesNumber
	 * variable by one.
	 */
	void IncrementOnlineLossesNumber();

private:
	/** Seconds spent on playing current level. */
	float TimeOnLevel;

	/** Timer Handle for incrementing TimeOnLevel variable. */
	FTimerHandle TH_IncrementTimeOnLevel;

public:
	/**
	 * Function for triggering the Destroy Loading Screen
	 * Delegate.
	 */
	UFUNCTION(Client, Reliable)
	void TriggerDestroyLoadingScreenDelegate(const bool bShouldHideBackgroundImage);
	void TriggerDestroyLoadingScreenDelegate_Implementation(const bool bShouldHideBackgroundImage);

	/**
	 * Function for triggering the Should Begin Countdown
	 * To Start Level Delegate.
	 */
	UFUNCTION(Client, Reliable)
	void TriggerShouldBeginCountdownDelegate();
	void TriggerShouldBeginCountdownDelegate_Implementation();

private:
	/**
	 * Function for starting the TH_IncrementTimeOnLevel
	 * timer.
	 */
	void CountdownStarted();

public:
	/**
	 * Getter for the TimeOnLevel variable.
	 * @return TimeOnLevel variable.
	 */
	FORCEINLINE float GetTimeOnLevel() const { return TimeOnLevel; }

	/** Function for incrementing TimeOnLevel variable. */
	UFUNCTION()
	void IncrementTimeOnLevel();

private:
	/** Player's score on current level. */
	UPROPERTY(Replicated)
	int32 UserScore;

public:
	/**
	 * Getter for the UserScore variable.
	 * @return UserScore variable.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetUserScore() const { return UserScore; }

	/**
	 * Setter for the UserScore variable.
	 * @param NewValue Value to set to UserScore variable.
	 */
	UFUNCTION(Server, Reliable)
	void SetUserScore(const int32 NewValue);
	void SetUserScore_Implementation(const int32 NewValue);

	/**
	 * Function for adding (or removing) points to (or from)
	 * score.
	 * @param PointsToAdd Difference between old and new
	 * score amount.
	 */
	UFUNCTION(Server, Reliable)
	void AddNewPointsToUserScore(const int32 PointsToAdd);
	void AddNewPointsToUserScore_Implementation(const int32 PointsToAdd);

	/**
	 * Function for triggering the delegate to notify widgets
	 * about score changing.
	 * @param PointsToAdd Difference between old and new
	 * score amount.
	 */
	UFUNCTION(Client, Reliable)
	void TriggerScoreChangedDelegate(const int32 PointsToAdd);
	void TriggerScoreChangedDelegate_Implementation(const int32 PointsToAdd);

	/** Getter for the GeneralScore variable. */
	FORCEINLINE uint32 GetGeneralScore() const { return PlayerData.GeneralScore; }

	/** Setter for the GeneralScore variable. */
	FORCEINLINE void SetGeneralScore(const int32 NewValue) { PlayerData.GeneralScore = NewValue; }

	/**
	 * Function for summation current score and general one
	 * and setting this value to the GeneralScore variable.
	 */
	void AddCurrentScoreToGeneralScore();

	/**
	 * Function for broadcasting the Level Was Ended Delegate.
	 * @param bIsWinner Is current player a winner?
	 */
	UFUNCTION(Client, Reliable)
	void TriggerLevelWasEndedDelegate(const bool bIsWinner);
	void TriggerLevelWasEndedDelegate_Implementation(const bool bIsWinner);

private:
	/** Timer Handle for incrementing TimeInGame variable. */
	FTimerHandle TH_IncrementTimeInGame;

public:
	/**
	 * Getter for the TimeInGame variable.
	 * @return TimeInGame variable.
	 */
	FORCEINLINE uint32 GetTimeInGame() const { return PlayerData.TimeInGame; }

	/**
	 * Setter for the TimeInGame variable.
	 * @param NewValue Value to set to TimeInGame variable.
	 */
	void SetTimeInGame(uint32 NewValue);

	/** Function for incrementing TimeInGame variable. */
	UFUNCTION()
	void IncrementTimeInGame();

	/**
	 * Function for checking if the player has broken his
	 * record and for updating needed data if he has.
	 * @param LevelNumber Number of the ended level.
	 * @param SpentTime Number of seconds that were spent to
	 * end the level.
	 */
	void TryToUpdateBestResult(const uint8 LevelNumber, const float SpentTime);

	/** Getter for the BestTimeLevel_1 variable. */
	FORCEINLINE float GetBestTimeLevel_1() const { return PlayerData.BestTimeLevel_1; }

	/** Setter for the BestTimeLevel_1 variable. */
	FORCEINLINE void SetBestTimeLevel_1(const float NewValue) { PlayerData.BestTimeLevel_1 = NewValue; }

	/** Getter for the BestTimeLevel_2 variable. */
	FORCEINLINE float GetBestTimeLevel_2() const { return PlayerData.BestTimeLevel_2; }

	/** Setter for the BestTimeLevel_2 variable. */
	FORCEINLINE void SetBestTimeLevel_2(const float NewValue) { PlayerData.BestTimeLevel_2 = NewValue; }

	/** Getter for the BestTimeLevel_3 variable. */
	FORCEINLINE float GetBestTimeLevel_3() const { return PlayerData.BestTimeLevel_3; }

	/** Setter for the BestTimeLevel_3 variable. */
	FORCEINLINE void SetBestTimeLevel_3(const float NewValue) { PlayerData.BestTimeLevel_3 = NewValue; }

	/** Getter for the BestTimeLevel_4 variable. */
	FORCEINLINE float GetBestTimeLevel_4() const { return PlayerData.BestTimeLevel_4; }

	/** Setter for the BestTimeLevel_4 variable. */
	FORCEINLINE void SetBestTimeLevel_4(const float NewValue) { PlayerData.BestTimeLevel_4 = NewValue; }

	/** Getter for the BestTimeLevel_5 variable. */
	FORCEINLINE float GetBestTimeLevel_5() const { return PlayerData.BestTimeLevel_5; }

	/** Setter for the BestTimeLevel_5 variable. */
	FORCEINLINE void SetBestTimeLevel_5(const float NewValue) { PlayerData.BestTimeLevel_5 = NewValue; }

	/** Getter for the BestTimeLevel_6 variable. */
	FORCEINLINE float GetBestTimeLevel_6() const { return PlayerData.BestTimeLevel_6; }

	/** Setter for the BestTimeLevel_6 variable. */
	FORCEINLINE void SetBestTimeLevel_6(const float NewValue) { PlayerData.BestTimeLevel_6 = NewValue; }

	/**
	 * Getter for the SFX_Volume variable.
	 * @return SFX_Volume variable.
	 */
	FORCEINLINE float Get_SFX_Volume() const { return PlayerData.SFX_Volume; }

	/**
	 * Setter for the SFX_Volume variable.
	 * @param NewValue Value to set to SFX_Volume variable.
	 */
	void Set_SFX_Volume(const float NewValue);

	/**
	 * Getter for the Music_Volume variable.
	 * @return Music_Volume variable.
	 */
	FORCEINLINE float Get_Music_Volume() const { return PlayerData.Music_Volume; }

	/**
	 * Setter for the Music_Volume variable.
	 * @param NewValue Value to set to Music_Volume variable.
	 */
	void Set_Music_Volume(const float NewValue);

	/**
	 * Getter for the MM_ActiveTracksNumbers variable.
	 * @return MM_ActiveTracksNumbers variable.
	 */
	FORCEINLINE TArray<uint8> Get_MM_ActiveTracksNumbers() const { return PlayerData.MM_ActiveTracksNumbers; }

	/**
	 * Setter for the MM_ActiveTracksNumbers variable.
	 * @param NewValue Value to set to MM_ActiveTracksNumbers
	 * variable.
	 */
	void Set_MM_ActiveTracksNumbers(const TArray<uint8>& NewValue);

	/**
	 * Getter for the MM_LastTrackNumber variable.
	 * @return MM_LastTrackNumber variable.
	 */
	FORCEINLINE uint8 Get_MM_LastTrackNumber() const { return PlayerData.MM_LastTrackNumber; }

	/**
	 * Setter for the MM_LastTrackNumber variable.
	 * @param NewValue Value to set to MM_LastTrackNumber
	 * variable.
	 */
	void Set_MM_LastTrackNumber(const uint8 NewValue);

	/**
	 * Getter for the MM_PlaylistRepeatingMode variable.
	 * @return MM_PlaylistRepeatingMode variable.
	 */
	FORCEINLINE EPlaylistRepeatingMode Get_MM_PlaylistRepeatingMode() const
	{
		return PlayerData.MM_PlaylistRepeatingMode;
	}

	/**
	 * Setter for the MM_PlaylistRepeatingMode variable.
	 * @param NewValue Value to set to MM_PlaylistRepeatingMode
	 * variable.
	 */
	void Set_MM_PlaylistRepeatingMode(const EPlaylistRepeatingMode NewValue);

	/**
	 * Getter for the L_ActiveTracksNumbers variable.
	 * @return L_ActiveTracksNumbers variable.
	 */
	FORCEINLINE TArray<uint8> Get_L_ActiveTracksNumbers() const { return PlayerData.L_ActiveTracksNumbers; }

	/**
	 * Setter for the L_ActiveTracksNumbers variable.
	 * @param NewValue Value to set to L_ActiveTracksNumbers
	 * variable.
	 */
	void Set_L_ActiveTracksNumbers(const TArray<uint8>& NewValue);

	/**
	 * Getter for the L_LastTrackNumber variable.
	 * @return L_LastTrackNumber variable.
	 */
	FORCEINLINE uint8 Get_L_LastTrackNumber() const { return PlayerData.L_LastTrackNumber; }

	/**
	 * Setter for the L_LastTrackNumber variable.
	 * @param NewValue Value to set to L_LastTrackNumber
	 * variable.
	 */
	void Set_L_LastTrackNumber(const uint8 NewValue);

	/**
	 * Getter for the L_PlaylistRepeatingMode variable.
	 * @return L_PlaylistRepeatingMode variable.
	 */
	FORCEINLINE EPlaylistRepeatingMode Get_L_PlaylistRepeatingMode() const
	{
		return PlayerData.L_PlaylistRepeatingMode;
	}

	/**
	 * Setter for the L_PlaylistRepeatingMode variable.
	 * @param NewValue Value to set to L_PlaylistRepeatingMode
	 * variable.
	 */
	void Set_L_PlaylistRepeatingMode(const EPlaylistRepeatingMode NewValue);
};
