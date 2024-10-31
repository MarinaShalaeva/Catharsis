// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef CPP_SAVEGAME_H
#define CPP_SAVEGAME_H
#include "CatPlatformer/SaveGame/Classes/CPP_SaveGame.h"
#endif
class UCPP_SaveGame;

#include "CPP_SaveManager.generated.h"

/**
 * Class for working with the Save Game object (saving
 * and loading information about the players, creating
 * and deleting save file, etc).
 */
UCLASS()
class CATPLATFORMER_API UCPP_SaveManager : public UObject
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	UCPP_SaveManager();

	/** Reference to the instance of UCPP_SaveGame class. */
	UPROPERTY()
	UCPP_SaveGame* SaveGameRef;

	/**
	 * The name of the file that stores saved information
	 * about the game.
	 */
	UPROPERTY()
	FString SaveFileName;

public:
	/** Is variable SaveGameRef valid now? */
	UPROPERTY()
	bool bSaveGameObjectIsDeclared;

	/**
	 * Setter for the SaveGameRef variable.
	 * @param NewSaveGame Value to set.
	 */
	UFUNCTION()
	void SetSaveGameObject(UCPP_SaveGame* NewSaveGame);

	/** Getter for the SaveGameRef variable. */
	UFUNCTION()
	UCPP_SaveGame* GetSaveGameObject() const;

	/**
	 * Function for changing current active save slot.
	 * @param SaveSlotNumber Number of chosen save slot.
	 */
	void ChooseSaveSlot(const uint8 SaveSlotNumber);

	/**
	 * Function for getting the username from current save
	 * slot.
	 * @return Username from save slot.
	 */
	FString GetUserNameFromSaveSlot() const;

	/**
	 * Function for getting the username from save slot.
	 * @param SaveSlotNumber Number of save slot to check.
	 * @param OutUserName Username from save slot.
	 * @param OutDateCreation Date of save file creation.
	 * @return Does save slot with such number exist?
	 */
	static bool GetUserNameAndDateCreationFromSaveSlot(const uint8 SaveSlotNumber,
	                                                   FString& OutUserName,
	                                                   FDateTime& OutDateCreation);

	/**
	 * Function for checking if the save file with current
	 * name (SaveFileName) exists and is valid.
	 * @return Does the save slot exist?
	 */
	bool DoesSaveSlotExist() const;

	/**
	 * Function for creating a Save Game object with information
	 * loaded from the save file, if the file exists. Otherwise,
	 * a new Save Game object will be created with variables
	 * that store default values.
	 * @return Loaded ot created UCPP_SaveGame instance.
	 */
	UFUNCTION()
	UCPP_SaveGame* LoadOrCreateSaveGameObject();

	/**
	 * Function for getting data from the save file. 
	 * @param OutSaveSlot Structure with saved parameters.
	 * @return Success.
	 */
	UFUNCTION()
	bool GetDataFromSaveFile(FSaveSlot& OutSaveSlot) const;

	/**
	 * Function for updating the save game object data and
	 * saving it to file.
	 * @param InSaveSlot Structure with parameters that should
	 * be saved.
	 * @return Was saving process successful?
	 */
	UFUNCTION()
	bool SetNewDataToSaveGameObject(const FSaveSlot& InSaveSlot) const;

	/**
	 * Function for updating the save game object audio data
	 * and saving it to file.
	 * @return Was saving process successful?
	 */
	UFUNCTION()
	bool SetNewAudioDataToSaveGameObject(const float SFX_Volume, const float Music_Volume,
	                                     const TArray<uint8>& MM_ActiveTracksNumbers,
	                                     const uint8 MM_LastTrackNumber,
	                                     const EPlaylistRepeatingMode MM_PlaylistRepeatingMode,
	                                     const TArray<uint8>& L_ActiveTracksNumbers,
	                                     const uint8 L_LastTrackNumber,
	                                     const EPlaylistRepeatingMode L_PlaylistRepeatingMode) const;
	
	/**
	 * Function for saving current Save Game object settings
	 * to file.
	 * @return Was saving process successful?
	 */
	bool SaveGameInfoToFile() const;

	/**
	 * Function for deleting current save game file.
	 * @return Was deleting successful?
	 */
	UFUNCTION()
	bool DeleteSave();

	/**
	 * Function for deleting file containing the particular
	 * save slot.
	 * @param SaveSlotNumber Number of save slot to delete.
	 * @return Was deleting successful?
	 */
	UFUNCTION()
	static bool DeleteSaveBySlotNumber(const uint8 SaveSlotNumber);
};
