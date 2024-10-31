// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticLibrary.h"
#endif

#include "CPP_SaveGame.generated.h"

/**
 * Save Game object for saving and loading information
 * about the player.
 */
UCLASS()
class CATPLATFORMER_API UCPP_SaveGame : public USaveGame
{
	GENERATED_BODY()

	/** Structure with parameters that should be saved. */
	UPROPERTY()
	FSaveSlot SaveSlotStruct;

	/**
	 * Structure with date and time of file creation (or
	 * updating).
	 */
	UPROPERTY()
	FDateTime CreationData;

public:
	/**
	 * Function for setting current time to the CreationData
	 * variable.
	 */
	UFUNCTION()
	void UpdateCreationData();

	/**
	 * Setting new Save Slot struct to local variable
	 * SaveSlotStruct.
	 * @param InSaveSlot New SaveSlotStruct value.
	 */
	UFUNCTION()
	void SetSaveSlotStruct(const FSaveSlot& InSaveSlot);

	/**
	 * Setting new values to audio variables from the save
	 * slot struct.
	 */
	UFUNCTION()
	void SetNewSaveSlotAudioParams(const float SFX_Volume, const float Music_Volume,
	                               const TArray<uint8>& MM_ActiveTracksNumbers,
	                               const uint8 MM_LastTrackNumber,
	                               const EPlaylistRepeatingMode MM_PlaylistRepeatingMode,
	                               const TArray<uint8>& L_ActiveTracksNumbers,
	                               const uint8 L_LastTrackNumber,
	                               const EPlaylistRepeatingMode L_PlaylistRepeatingMode);

	/**
	 * Getter for the UserName variable from struct.
	 * @return The username from the save slot.
	 */
	UFUNCTION()
	FORCEINLINE FString GetUserNameFromSaveSlot() const { return SaveSlotStruct.UserName; }

	/**
	 * Getter for the TimeInGame variable from struct.
	 * @return The time spent in the game from the save slot.
	 */
	UFUNCTION()
	FORCEINLINE uint32 GetTimeInGameFromSaveSlot() const { return SaveSlotStruct.TimeInGame; }

	/**
	 * Getter for the CreationData variable.
	 * @return The creation data of the save file from the
	 * save slot.
	 */
	UFUNCTION()
	FORCEINLINE FDateTime GetCreationDataFromSaveSlot() const { return CreationData; }

	/**
	 * Getter for the SaveSlotStruct variable.
	 * @return SaveSlotStruct variable.
	 */
	UFUNCTION()
	FORCEINLINE FSaveSlot GetSaveSlotStruct() const { return SaveSlotStruct; }
};
