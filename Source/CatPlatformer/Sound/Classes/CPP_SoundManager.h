// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticLibrary.h"
#endif
enum class EPlaylistRepeatingMode : uint8;

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif
class ACPP_PlayerState;

class USceneComponent;
class UAudioComponent;

#include "CPP_SoundManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FTrackWasSwitched);

/** Class for working with sounds. */
UCLASS(Abstract)
class CATPLATFORMER_API ACPP_SoundManager : public AActor
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_SoundManager();

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
	 * Weak pointer to the instance of ACPP_PlayerController
	 * class.
	 */
	TWeakObjectPtr<ACPP_PlayerController> PlayerControllerRef;

	/** Weak pointer to the instance of ACPP_PlayerState class. */
	TWeakObjectPtr<ACPP_PlayerState> PlayerStateRef;

	/** USoundMix class object reference. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes", meta = (AllowPrivateAccess = true))
	USoundMix* SoundMix;

	/** USoundClass object reference for changing music settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes", meta = (AllowPrivateAccess = true))
	USoundClass* Music_SoundClass;

	/** USoundClass object reference for changing SFX settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override Classes", meta = (AllowPrivateAccess = true))
	USoundClass* SFX_SoundClass;

	/**
	 * The root component for storing all other components
	 * of the actor.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Components | Root", meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	/**
	 * Current repeating mode (looping playlist; looping one
	 * track; no looping at all).
	 */
	EPlaylistRepeatingMode PlaylistRepeatingMode;

	/**
	 * An array for storing chosen by the user tracks that
	 * can be played.
	 */
	UPROPERTY()
	TArray<UAudioComponent*> ActivePlaylistTracks;

	/**
	 * An array for storing all available audio components
	 * of the actor.
	 */
	UPROPERTY()
	TArray<UAudioComponent*> AllAudioComponents;

	/**
	 * An array of the tracks' numbers added to the playlist
	 * by the user.
	 */
	TArray<uint8> ActiveTracksNumbers;

	/**
	 * The current playing (or last played) audio track
	 * index in the ActivePlaylistTracks array.
	 */
	int8 CurrentTrackIndex;

	/**
	 * The current playing (or last played) audio track
	 * ordinal number.
	 */
	uint8 CurrentTrackNumber;

public:
	/**
	 * Delegate for notifying that playing of the track ended
	 * and this track was switched to the next one.
	 */
	FTrackWasSwitched TrackWasSwitchedDelegate;

protected:
	//Variable for storing number of seconds that was played
	//before track was paused.
	//float CurrentPausedTrackTime;

	/** Number of all potentially available tracks. */
	uint8 GeneralNumberOfTracks;

	/**
	 * A flag indicating whether the current track is paused
	 * or is currently playing.
	 */
	bool bTrackIsPaused;

	/**
	 * A flag indicating whether AudioTrackWasEnded function
	 * should be called.
	 */
	bool bShouldReactOnTrackEnding;

	/**
	 * Timer for calling function that will set value of
	 * bShouldReactOnTrackEnding variable to true.
	 */
	FTimerHandle TH_TurnOnShouldReactOnTrackEnding;

	/**
	 * Function for adding all audio components to the
	 * array.
	 */
	UFUNCTION()
	virtual void FillAllAudioComponentsArray();

public:
	/** Function to apply saved data or to apply basic values. */
	UFUNCTION()
	virtual void ApplySettingsFromTheSaveFile(const bool bSetDefaultValues, const bool bIsAdditionalSoundManager);

	/** Function for updating current SFX volume. */
	void SetNewSFXVolume(const float Volume) const;

	/** Function for updating current music volume. */
	void SetNewMusicVolume(const float Volume) const;

	/**
	 * Function for updating current track number by matching
	 * audio components from the Active Playlist Tracks array
	 * and audio components from All Audio Components array
	 * (= audio components declared inside a class).
	 */
	UFUNCTION()
	void UpdateCurrentTrackNumber();

	/**
	 * Function that is called when any music track is
	 * finished playing.
	 */
	UFUNCTION()
	void AudioTrackWasEnded();

	/**
	 * Function for switching current track to the next one
	 * and for starting to play it.
	 */
	UFUNCTION()
	void StartPlayingNextTrack();

	/**
	 * Function for switching current track to the next one
	 * and for starting to play it. For choosing track the
	 * ActivePlaylistTracks array is used.
	 */
	UFUNCTION()
	void StartPlayingNextTrackFromActivePlaylist();

	/**
	 * Function for switching current track to the previous
	 * one and for starting to play it.
	 */
	UFUNCTION()
	void StartPlayingPreviousTrack();

	/**
	 * Function for set value of bShouldReactOnTrackEnding
	 * variable to true.
	 */
	void TurnOnShouldReactOnTrackEnding();

	/** Function for stopping the current playing audio track. */
	UFUNCTION()
	void StopPlayingCurrentTrack();

	/** Function for starting the current playing audio track. */
	UFUNCTION()
	void StartPlayingCurrentTrack();

	/** Function for pausing current track. */
	UFUNCTION()
	void PauseCurrentTrack();

	/** Function for unpausing current track. */
	UFUNCTION()
	void UnpauseCurrentTrack();

	/** Function for adding a track to the playlist. */
	UFUNCTION()
	void AddTrackToActivePlaylist(const uint8 TrackNumber);

	/** Function for removing a track from the playlist. */
	UFUNCTION()
	void RemoveTrackFromActivePlaylist(const uint8 TrackNumber);

	/**
	 * Function for starting playing custom playlist created
	 * by user.
	 */
	UFUNCTION()
	void StartPlayingUserPlaylist();

	/**
	 * Function fot checking if any track is playing.
	 * @return True if any track is playing at the moment,
	 * false otherwise.
	 */
	bool IsAnyTrackPlaying();

	/**
	 * Setter for the PlaylistRepeatingMode variable (as enum).
	 * @param NewValue Value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPlaylistRepeatingMode(const EPlaylistRepeatingMode NewValue);

	/**
	 * Getter for the PlaylistRepeatingMode variable (as enum).
	 * @return PlaylistRepeatingMode variable as enum.
	 */
	UFUNCTION()
	EPlaylistRepeatingMode GetPlaylistRepeatingMode() const;

	/**
	 * Setter for the PlaylistRepeatingMode variable (as int).
	 * RepeatPlaylist = 0,
	 * RepeatOneTrack = 1,
	 * NoRepeat = 2.
	 * @param NewValue Index of value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPlaylistRepeatingModeAsInt(const int32 NewValue);

	/**
	 * Getter for the PlaylistRepeatingMode variable (as int).
	 * RepeatPlaylist = 0,
	 * RepeatOneTrack = 1,
	 * NoRepeat = 2.
	 * @return PlaylistRepeatingMode variable as int.
	 */
	UFUNCTION()
	int32 GetPlaylistRepeatingModeAsInt() const;

	/**
	 * Setter for the CurrentTrackIndex variable.
	 * @param NewNumber Value to set.
	 */
	UFUNCTION()
	FORCEINLINE void SetCurrentTrackIndex(const uint8 NewNumber) { CurrentTrackIndex = NewNumber; }

	/** Getter for the CurrentTrackIndex variable. */
	UFUNCTION()
	FORCEINLINE uint8 GetCurrentTrackIndex() const { return CurrentTrackIndex; }

	/**
	 * Setter for the CurrentTrackNumber variable.
	 * @param NewNumber Value to set.
	 */
	UFUNCTION()
	FORCEINLINE void SetCurrentTrackNumber(const uint8 NewNumber) { CurrentTrackNumber = NewNumber; }

	/** Getter for the CurrentTrackNumber variable. */
	UFUNCTION()
	FORCEINLINE uint8 GetCurrentTrackNumber() const { return CurrentTrackNumber; }

	/**
	 * Getter for the Audio Component related to the
	 * current playing track (or to the last played
	 * track, if there are not playing tracks now).
	 */
	UFUNCTION()
	UAudioComponent* GetCurrentAudioComponent() const;

	/**
	 * Function for getting playlist tracks numbers.
	 * @return Numbers of tracks that where added to the
	 * playlist by the user.
	 */
	UFUNCTION()
	TArray<uint8>& GetActivePlaylistTracksNumbers();

	/**
	 * Setter for the PlayerControllerRef variable.
	 * @param NewPlayerController New reference to the Player
	 * Controller.
	 */
	UFUNCTION()
	void SetPlayerControllerRef(ACPP_PlayerController* NewPlayerController);

	/** Getter for the bTrackIsPaused variable. */
	UFUNCTION()
	FORCEINLINE bool GetTrackIsPaused() const { return bTrackIsPaused; }

	/**
	 * Setter for the bTrackIsPaused variable.
	 * @param bNewValue Value to set.
	 */
	UFUNCTION()
	FORCEINLINE void SetTrackIsPaused(const bool bNewValue) { bTrackIsPaused = bNewValue; }

	/** Getter for the GeneralNumberOfTracks variable. */
	FORCEINLINE uint8 GetGeneralNumberOfTracks() const { return GeneralNumberOfTracks; }
};
