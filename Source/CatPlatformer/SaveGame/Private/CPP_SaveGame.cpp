// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SaveGame.h"

void UCPP_SaveGame::UpdateCreationData()
{
	CreationData = FDateTime::Now();
}

void UCPP_SaveGame::SetSaveSlotStruct(const FSaveSlot& InSaveSlot)
{
	SaveSlotStruct = InSaveSlot;
}

void UCPP_SaveGame::SetNewSaveSlotAudioParams(const float SFX_Volume, const float Music_Volume,
                                              const TArray<uint8>& MM_ActiveTracksNumbers,
                                              const uint8 MM_LastTrackNumber,
                                              const EPlaylistRepeatingMode MM_PlaylistRepeatingMode,
                                              const TArray<uint8>& L_ActiveTracksNumbers,
                                              const uint8 L_LastTrackNumber,
                                              const EPlaylistRepeatingMode L_PlaylistRepeatingMode)
{
	SaveSlotStruct.SFX_Volume = SFX_Volume;
	SaveSlotStruct.Music_Volume = Music_Volume;
	SaveSlotStruct.MM_ActiveTracksNumbers = MM_ActiveTracksNumbers;
	SaveSlotStruct.MM_LastTrackNumber = MM_LastTrackNumber;
	SaveSlotStruct.MM_PlaylistRepeatingMode = MM_PlaylistRepeatingMode;
	SaveSlotStruct.L_ActiveTracksNumbers = L_ActiveTracksNumbers;
	SaveSlotStruct.L_LastTrackNumber = L_LastTrackNumber;
	SaveSlotStruct.L_PlaylistRepeatingMode = L_PlaylistRepeatingMode;
}
