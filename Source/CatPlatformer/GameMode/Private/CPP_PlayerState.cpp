// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_PlayerState.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

#include "Net/UnrealNetwork.h"

ACPP_PlayerState::ACPP_PlayerState()
{
	GameInstanceRef = nullptr;
	bIsReadyToStart = false;
	bSaveFileWasCreated = false;
	bIsFirstPlayer = true;
	PlayerData = FSaveSlot{};
	CurrentLevelDeathsNumber = 0;
	UserScore = 0;
	SetTickableWhenPaused(true);

	bReplicates = true;
}

void ACPP_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	ShouldBeginCountdownToStartLevelDelegate.AddUObject(this, &ACPP_PlayerState::CountdownStarted);

	if (!IsValid(GameInstanceRef))
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
		if (bIsFirstPlayer)
		{
			LoadInfoFromSaveFile();
			UpdateCatsColorUpToSavedIndex();
		}
	}
	if (PlayerData.MM_ActiveTracksNumbers.IsEmpty())
	{
		PlayerData.MM_ActiveTracksNumbers.Add(1);
		PlayerData.MM_ActiveTracksNumbers.Add(2);
		PlayerData.MM_ActiveTracksNumbers.Add(3);
	}
	if (PlayerData.L_ActiveTracksNumbers.IsEmpty())
	{
		PlayerData.L_ActiveTracksNumbers.Add(1);
		PlayerData.L_ActiveTracksNumbers.Add(2);
		PlayerData.L_ActiveTracksNumbers.Add(3);
		PlayerData.L_ActiveTracksNumbers.Add(4);
		PlayerData.L_ActiveTracksNumbers.Add(5);
	}
	if (bIsFirstPlayer)
	{
		GetWorld()->GetTimerManager().SetTimer(TH_IncrementTimeInGame,
		                                       this,
		                                       &ACPP_PlayerState::IncrementTimeInGame,
		                                       1.0f,
		                                       true,
		                                       1.0f);
	}
}

void ACPP_PlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SaveDataToFile();

	if (GetWorld()->GetTimerManager().TimerExists(TH_IncrementTimeInGame))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_IncrementTimeInGame);
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_IncrementTimeOnLevel))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_IncrementTimeOnLevel);
	}

	if (ScoreChangedDelegate.IsBound())
	{
		ScoreChangedDelegate.Unbind();
	}
	ShouldBeginCountdownToStartLevelDelegate.Clear();
	PlayerIsReadyForGameDelegate.Clear();

	Super::EndPlay(EndPlayReason);
}

void ACPP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_PlayerState, bIsReadyToStart);
	DOREPLIFETIME(ACPP_PlayerState, UserScore);
}

bool ACPP_PlayerState::LoadInfoFromSaveFile()
{
	if (!IsValid(GameInstanceRef))
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}
	if (IsValid(GameInstanceRef))
	{
		if (!bIsFirstPlayer)
			return false;

		if (GameInstanceRef->GetSaveManager()->GetDataFromSaveFile(PlayerData))
		{
			bSaveFileWasCreated = true;
			return true;
		}
		return false;
	}
	return false;
}

void ACPP_PlayerState::SaveDataToFile()
{
	if (!bIsFirstPlayer)
		return;

	if (IsValid(GameInstanceRef))
	{
		bSaveFileWasCreated = true;
		CollectAudioInfoForSavingItToFile();
		GameInstanceRef->GetSaveManager()->SetNewDataToSaveGameObject(PlayerData);
	}
}

void ACPP_PlayerState::CollectAudioInfoForSavingItToFile()
{
	if (!bIsFirstPlayer)
		return;

	if (IsValid(GameInstanceRef))
	{
		bSaveFileWasCreated = true;
		GameInstanceRef->GetSaveManager()->SetNewAudioDataToSaveGameObject(
			Get_SFX_Volume(), Get_Music_Volume(),
			Get_MM_ActiveTracksNumbers(),
			Get_MM_LastTrackNumber(),
			Get_MM_PlaylistRepeatingMode(),
			Get_L_ActiveTracksNumbers(),
			Get_L_LastTrackNumber(),
			Get_L_PlaylistRepeatingMode());
	}
}

void ACPP_PlayerState::SetPlayerIsReadyToStartGame_Implementation(const bool bIsReady)
{
	bIsReadyToStart = bIsReady;
	if (bIsReadyToStart)
	{
		PlayerIsReadyForGameDelegate.Broadcast(this);
	}
}

void ACPP_PlayerState::GetTheSaveSlot(FSaveSlot& OutSaveSlot) const
{
	OutSaveSlot = PlayerData;
}

void ACPP_PlayerState::SetIsFirstPlayer(const bool bNewValue)
{
	bIsFirstPlayer = bNewValue;
	if (!bIsFirstPlayer)
	{
		if (GetWorld()->GetTimerManager().TimerExists(TH_IncrementTimeInGame))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_IncrementTimeInGame);
		}
	}
}

void ACPP_PlayerState::SetUserName(const FString& NewValue)
{
	PlayerData.UserName = NewValue;
}

void ACPP_PlayerState::SetMaxOpenedLevelNumber(const uint8 NewValue)
{
	if (NewValue < 1)
	{
		PlayerData.MaxOpenedLevelNumber = 1;
	}
	else if (NewValue > 6)
	{
		PlayerData.MaxOpenedLevelNumber = 6;
	}
	else
	{
		PlayerData.MaxOpenedLevelNumber = NewValue;
	}
}

void ACPP_PlayerState::SetCatColorIndex(const uint8 NewValue)
{
	PlayerData.CatColorIndex = NewValue % 3;
	UpdateCatsColorUpToSavedIndex();
}

void ACPP_PlayerState::UpdateCatsColorUpToSavedIndex_Implementation() const
{
	Multicast_UpdateCatsColorUpToSavedIndex();
}

void ACPP_PlayerState::Multicast_UpdateCatsColorUpToSavedIndex_Implementation() const
{
	if (ACPP_Character* Character = GetPawn<ACPP_Character>())
	{
		Character->SetCurrentColorMaterialByIndex(PlayerData.CatColorIndex);
		Character->SetCurrentColorVariableByIndex(PlayerData.CatColorIndex);
	}
}

void ACPP_PlayerState::SetGeneralDeathsNumber(const uint32 NewValue)
{
	PlayerData.DeathsNumber = NewValue;
}

void ACPP_PlayerState::IncrementDeathsNumber()
{
	CurrentLevelDeathsNumber++;
	PlayerData.DeathsNumber++;
}

void ACPP_PlayerState::IncrementOnlineLevelsWinsNumber()
{
	if (IsValid(GameInstanceRef) && (GameInstanceRef->GetPlayingModeAsInt() == 2 ||
		GameInstanceRef->GetPlayingModeAsInt() == 3 ||
		GameInstanceRef->GetPlayingModeAsInt() == 4))
	{
		PlayerData.OnlineWinsNumber++;
	}
}

void ACPP_PlayerState::IncrementOnlineLossesNumber()
{
	if (IsValid(GameInstanceRef) && (GameInstanceRef->GetPlayingModeAsInt() == 2 ||
		GameInstanceRef->GetPlayingModeAsInt() == 3 ||
		GameInstanceRef->GetPlayingModeAsInt() == 4))
	{
		PlayerData.OnlineLossesNumber++;
	}
}

void ACPP_PlayerState::CountdownStarted()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_IncrementTimeOnLevel))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_IncrementTimeOnLevel);
	}
	TimeOnLevel = 0;
	GetWorld()->GetTimerManager().SetTimer(TH_IncrementTimeOnLevel,
	                                       this,
	                                       &ACPP_PlayerState::IncrementTimeOnLevel,
	                                       0.01f,
	                                       true,
	                                       3.01f);
}

void ACPP_PlayerState::IncrementTimeOnLevel()
{
	TimeOnLevel += 0.01f;
}

void ACPP_PlayerState::SetTimeInGame(const uint32 NewValue)
{
	PlayerData.TimeInGame = NewValue;

	if (GetWorld()->GetTimerManager().TimerExists(TH_IncrementTimeInGame))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_IncrementTimeInGame);
	}

	GetWorld()->GetTimerManager().SetTimer(TH_IncrementTimeInGame,
	                                       this,
	                                       &ACPP_PlayerState::IncrementTimeInGame,
	                                       1.0f,
	                                       true,
	                                       1.0f);
}

void ACPP_PlayerState::IncrementTimeInGame()
{
	PlayerData.TimeInGame++;
	if (TimeInGameChangedDelegate.IsBound())
	{
		TimeInGameChangedDelegate.Execute(PlayerData.TimeInGame);
	}
}

void ACPP_PlayerState::TryToUpdateBestResult(const uint8 LevelNumber, const float SpentTime)
{
	switch (LevelNumber)
	{
	case 1:
		{
			if (GetBestTimeLevel_1() == 0.0f || SpentTime < GetBestTimeLevel_1())
			{
				SetBestTimeLevel_1(SpentTime);
			}
			break;
		}
	case 2:
		{
			if (GetBestTimeLevel_2() == 0.0f || SpentTime < GetBestTimeLevel_2())
			{
				SetBestTimeLevel_2(SpentTime);
			}
			break;
		}
	case 3:
		{
			if (GetBestTimeLevel_3() == 0.0f || SpentTime < GetBestTimeLevel_3())
			{
				SetBestTimeLevel_3(SpentTime);
			}
			break;
		}
	case 4:
		{
			if (GetBestTimeLevel_4() == 0.0f || SpentTime < GetBestTimeLevel_4())
			{
				SetBestTimeLevel_4(SpentTime);
			}
			break;
		}
	case 5:
		{
			if (GetBestTimeLevel_5() == 0.0f || SpentTime < GetBestTimeLevel_5())
			{
				SetBestTimeLevel_5(SpentTime);
			}
			break;
		}
	case 6:
		{
			if (GetBestTimeLevel_6() == 0.0f || SpentTime < GetBestTimeLevel_6())
			{
				SetBestTimeLevel_6(SpentTime);
			}
			break;
		}
	default: break;
	}
}

void ACPP_PlayerState::SetUserScore(const int32 NewValue)
{
	if (NewValue < 0)
	{
		UserScore = 0;
	}
	else
	{
		UserScore = NewValue;
	}
}

void ACPP_PlayerState::AddNewPointsToUserScore(const int32 PointsToAdd)
{
	SetUserScore(UserScore + PointsToAdd);
}

void ACPP_PlayerState::AddCurrentScoreToGeneralScore()
{
	PlayerData.GeneralScore += UserScore;
}

void ACPP_PlayerState::Set_SFX_Volume(const float NewValue)
{
	if (NewValue < 0.0f)
	{
		PlayerData.SFX_Volume = 0.0f;
	}
	else if (NewValue > 5.0f)
	{
		PlayerData.SFX_Volume = 5.0f;
	}
	else
	{
		PlayerData.SFX_Volume = NewValue;
	}
}

void ACPP_PlayerState::Set_Music_Volume(const float NewValue)
{
	if (NewValue < 0.0f)
	{
		PlayerData.Music_Volume = 0.0f;
	}
	else if (NewValue > 5.0f)
	{
		PlayerData.Music_Volume = 5.0f;
	}
	else
	{
		PlayerData.Music_Volume = NewValue;
	}
}

void ACPP_PlayerState::Set_MM_ActiveTracksNumbers(const TArray<uint8>& NewValue)
{
	PlayerData.MM_ActiveTracksNumbers = NewValue;
}

void ACPP_PlayerState::Set_MM_LastTrackNumber(const uint8 NewValue)
{
	if (NewValue < 1 || NewValue > 3)
		return;

	PlayerData.MM_LastTrackNumber = NewValue;
}

void ACPP_PlayerState::Set_MM_PlaylistRepeatingMode(const EPlaylistRepeatingMode NewValue)
{
	PlayerData.MM_PlaylistRepeatingMode = NewValue;
}

void ACPP_PlayerState::Set_L_ActiveTracksNumbers(const TArray<uint8>& NewValue)
{
	PlayerData.L_ActiveTracksNumbers = NewValue;
}

void ACPP_PlayerState::Set_L_LastTrackNumber(const uint8 NewValue)
{
	if (NewValue < 1 || NewValue > 5)
		return;

	PlayerData.L_LastTrackNumber = NewValue;
}

void ACPP_PlayerState::Set_L_PlaylistRepeatingMode(const EPlaylistRepeatingMode NewValue)
{
	PlayerData.L_PlaylistRepeatingMode = NewValue;
}
