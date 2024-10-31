// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SaveManager.h"
#include "Kismet/GameplayStatics.h"

UCPP_SaveManager::UCPP_SaveManager()
{
	SaveGameRef = nullptr;
	bSaveGameObjectIsDeclared = false;
	SaveFileName = FString(TEXT("SaveFile_Slot1"));
}

void UCPP_SaveManager::SetSaveGameObject(UCPP_SaveGame* NewSaveGame)
{
	if (IsValid(NewSaveGame))
	{
		SaveGameRef = NewSaveGame;
		bSaveGameObjectIsDeclared = true;
	}
	else
	{
		SaveGameRef = nullptr;
		bSaveGameObjectIsDeclared = false;
	}
}

UCPP_SaveGame* UCPP_SaveManager::GetSaveGameObject() const
{
	return SaveGameRef;
}

void UCPP_SaveManager::ChooseSaveSlot(const uint8 SaveSlotNumber)
{
	SaveFileName = FString::Printf(TEXT("SaveFile_Slot%d"), SaveSlotNumber);
}

FString UCPP_SaveManager::GetUserNameFromSaveSlot() const
{
	if (bSaveGameObjectIsDeclared)
	{
		return SaveGameRef->GetUserNameFromSaveSlot().TrimStartAndEnd();
	}
	return FString();
}

bool UCPP_SaveManager::GetUserNameAndDateCreationFromSaveSlot(const uint8 SaveSlotNumber,
                                                              FString& OutUserName,
                                                              FDateTime& OutDateCreation)
{
	const FString FileName = FString::Printf(TEXT("SaveFile_Slot%d"), SaveSlotNumber);
	bool bSuccess = UGameplayStatics::DoesSaveGameExist(FileName, 0);
	if (bSuccess)
	{
		USaveGame* Save = UGameplayStatics::LoadGameFromSlot(FileName, 0);
		if (const UCPP_SaveGame* LoadedSaveGameInstance = Cast<UCPP_SaveGame>(Save);
			IsValid(LoadedSaveGameInstance))
		{
			bSuccess = true;
			OutUserName = LoadedSaveGameInstance->GetUserNameFromSaveSlot();
			OutDateCreation = LoadedSaveGameInstance->GetCreationDataFromSaveSlot();
		}
		else
		{
			bSuccess = false;
		}
	}
	return bSuccess;
}

bool UCPP_SaveManager::DoesSaveSlotExist() const
{
	bool bSuccess = UGameplayStatics::DoesSaveGameExist(SaveFileName, 0);
	if (bSuccess)
	{
		USaveGame* Save = UGameplayStatics::LoadGameFromSlot(SaveFileName, 0);
		const UCPP_SaveGame* LoadedSaveGameInstance = Cast<UCPP_SaveGame>(Save);
		bSuccess = IsValid(LoadedSaveGameInstance);
	}
	return bSuccess;
}

UCPP_SaveGame* UCPP_SaveManager::LoadOrCreateSaveGameObject()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveFileName, 0))
	{
		USaveGame* Save = UGameplayStatics::LoadGameFromSlot(SaveFileName, 0);
		UCPP_SaveGame* LoadedSaveGameInstance = Cast<UCPP_SaveGame>(Save);
		if (IsValid(LoadedSaveGameInstance))
		{
			SetSaveGameObject(LoadedSaveGameInstance);
			return SaveGameRef;
		}
	}
	else if (UCPP_SaveGame* SaveGameInstance = Cast<UCPP_SaveGame>(
		UGameplayStatics::CreateSaveGameObject(UCPP_SaveGame::StaticClass())))
	{
		SetSaveGameObject(SaveGameInstance);
		return SaveGameRef;
	}
	return nullptr;
}

bool UCPP_SaveManager::GetDataFromSaveFile(FSaveSlot& OutSaveSlot) const
{
	if (!bSaveGameObjectIsDeclared)
		return false;

	OutSaveSlot = SaveGameRef->GetSaveSlotStruct();
	return true;
}

bool UCPP_SaveManager::SetNewDataToSaveGameObject(const FSaveSlot& InSaveSlot) const
{
	if (bSaveGameObjectIsDeclared)
	{
		SaveGameRef->SetSaveSlotStruct(InSaveSlot);
		return SaveGameInfoToFile();
	}
	return false;
}

bool UCPP_SaveManager::SetNewAudioDataToSaveGameObject(const float SFX_Volume, const float Music_Volume,
                                                       const TArray<uint8>& MM_ActiveTracksNumbers,
                                                       const uint8 MM_LastTrackNumber,
                                                       const EPlaylistRepeatingMode MM_PlaylistRepeatingMode,
                                                       const TArray<uint8>& L_ActiveTracksNumbers,
                                                       const uint8 L_LastTrackNumber,
                                                       const EPlaylistRepeatingMode L_PlaylistRepeatingMode) const
{
	if (bSaveGameObjectIsDeclared)
	{
		SaveGameRef->SetNewSaveSlotAudioParams(SFX_Volume, Music_Volume,
		                                       MM_ActiveTracksNumbers, MM_LastTrackNumber, MM_PlaylistRepeatingMode,
		                                       L_ActiveTracksNumbers, L_LastTrackNumber, L_PlaylistRepeatingMode);
		return SaveGameInfoToFile();
	}
	return false;
}

bool UCPP_SaveManager::SaveGameInfoToFile() const
{
	if (bSaveGameObjectIsDeclared)
	{
		SaveGameRef->UpdateCreationData();
		if (UGameplayStatics::SaveGameToSlot(GetSaveGameObject(), SaveFileName, 0))
		{
			return true;
		}
		return false;
	}
	return false;
}

bool UCPP_SaveManager::DeleteSave()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveFileName, 0))
	{
		const bool bSuccess = UGameplayStatics::DeleteGameInSlot(SaveFileName, 0);

		if (bSuccess)
		{
			SetSaveGameObject(nullptr);
		}

		return bSuccess;
	}
	return false;
}

bool UCPP_SaveManager::DeleteSaveBySlotNumber(const uint8 SaveSlotNumber)
{
	if (const FString FileName = FString::Printf(TEXT("SaveFile_Slot%d"), SaveSlotNumber);
		UGameplayStatics::DoesSaveGameExist(FileName, 0))
	{
		return UGameplayStatics::DeleteGameInSlot(FileName, 0);
	}
	return false;
}
