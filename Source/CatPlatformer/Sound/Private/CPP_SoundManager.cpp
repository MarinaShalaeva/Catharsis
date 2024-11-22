// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SoundManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ACPP_SoundManager::ACPP_SoundManager() : PlayerControllerRef(nullptr),
                                         PlayerStateRef(nullptr),
                                         SoundMix(nullptr),
                                         Music_SoundClass(nullptr),
                                         SFX_SoundClass(nullptr),
                                         PlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatPlaylist),
                                         CurrentTrackIndex(0),
                                         CurrentTrackNumber(1),
                                         GeneralNumberOfTracks(-1),
                                         bTrackIsPaused(false),
                                         bShouldReactOnTrackEnding(true)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ACPP_SoundManager::BeginPlay()
{
	Super::BeginPlay();

	FillAllAudioComponentsArray();
}

void ACPP_SoundManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_TurnOnShouldReactOnTrackEnding))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_TurnOnShouldReactOnTrackEnding);
	}

	Super::EndPlay(EndPlayReason);
}

void ACPP_SoundManager::FillAllAudioComponentsArray()
{
}

void ACPP_SoundManager::ApplySettingsFromTheSaveFile(const bool bSetDefaultValues, const bool bIsAdditionalSoundManager)
{
	FillAllAudioComponentsArray();

	if (bSetDefaultValues || !PlayerStateRef.IsValid())
	{
		PlaylistRepeatingMode = EPlaylistRepeatingMode::RepeatPlaylist;

		for (int32 i = 1; i <= GeneralNumberOfTracks; i++)
		{
			ActiveTracksNumbers.Add(i);
		}
		ActivePlaylistTracks.Append(AllAudioComponents);

		CurrentTrackNumber = 1;
		CurrentTrackIndex = 0;

		if (!bIsAdditionalSoundManager)
		{
			if (SoundMix)
			{
				if (Music_SoundClass)
				{
					UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, Music_SoundClass,
					                                           1.0f, 1.0f, 0.0f, true);
				}
				if (SFX_SoundClass)
				{
					UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFX_SoundClass,
					                                           1.0f, 1.0f, 0.0f, true);
				}
			}
			StartPlayingCurrentTrack();
		}
	}
	else if (PlayerStateRef.IsValid() && !bIsAdditionalSoundManager)
	{
		if (SoundMix)
		{
			if (Music_SoundClass)
			{
				UGameplayStatics::SetSoundMixClassOverride(PlayerStateRef->GetWorld(), SoundMix, Music_SoundClass,
				                                           PlayerStateRef->Get_Music_Volume(),
				                                           1.0f, 0.0f, true);
			}
			if (SFX_SoundClass)
			{
				UGameplayStatics::SetSoundMixClassOverride(PlayerStateRef->GetWorld(), SoundMix, SFX_SoundClass,
				                                           PlayerStateRef->Get_SFX_Volume(),
				                                           1.0f, 0.0f, true);
			}
		}
	}
}

void ACPP_SoundManager::SetNewSFXVolume(const float Volume) const
{
	if (SoundMix && SFX_SoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFX_SoundClass,
		                                           Volume,
		                                           1.0f, 0.0f, true);
		if (PlayerStateRef.IsValid())
		{
			PlayerStateRef->Set_SFX_Volume(Volume);
		}
	}
}

void ACPP_SoundManager::SetNewMusicVolume(const float Volume) const
{
	if (SoundMix && Music_SoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, Music_SoundClass,
		                                           Volume,
		                                           1.0f, 0.0f, true);
		if (PlayerStateRef.IsValid())
		{
			PlayerStateRef->Set_Music_Volume(Volume);
		}
	}
}

void ACPP_SoundManager::UpdateCurrentTrackNumber()
{
	if (CurrentTrackIndex == -1)
	{
		CurrentTrackNumber = -1;
		return;
	}

	for (int32 i = 0; i < GeneralNumberOfTracks; i++)
	{
		if (ActivePlaylistTracks[CurrentTrackIndex] == AllAudioComponents[i])
		{
			CurrentTrackNumber = i + 1;
			break;
		}
	}
}

void ACPP_SoundManager::AudioTrackWasEnded()
{
	if (!bShouldReactOnTrackEnding || ActivePlaylistTracks.Num() == 0)
		return;

	switch (PlaylistRepeatingMode)
	{
	case EPlaylistRepeatingMode::RepeatPlaylist:
		if (ActivePlaylistTracks.Num() == 0)
			return;
		if (CurrentTrackIndex + 1 >= ActivePlaylistTracks.Num() ||
			CurrentTrackIndex == -1)
		{
			CurrentTrackIndex = 0;
		}
		else
		{
			CurrentTrackIndex++;
		}
		UpdateCurrentTrackNumber();
		ActivePlaylistTracks[CurrentTrackIndex]->Play();
		TrackWasSwitchedDelegate.Broadcast();
		break;
	case EPlaylistRepeatingMode::RepeatOneTrack:
		GetCurrentAudioComponent()->Play();
		break;
	case EPlaylistRepeatingMode::NoRepeat:
		if (CurrentTrackIndex == -1)
			return;
		if (CurrentTrackIndex + 1 < ActivePlaylistTracks.Num())
		{
			CurrentTrackIndex++;
			UpdateCurrentTrackNumber();
			ActivePlaylistTracks[CurrentTrackIndex]->Play();
			TrackWasSwitchedDelegate.Broadcast();
		}
		break;
	}
}

void ACPP_SoundManager::StartPlayingNextTrack()
{
	StopPlayingCurrentTrack();

	if (CurrentTrackNumber + 1 > GeneralNumberOfTracks)
	{
		CurrentTrackNumber = 1;
	}
	else
	{
		CurrentTrackNumber++;
	}
	CurrentTrackIndex = ActivePlaylistTracks.Find(AllAudioComponents[CurrentTrackNumber - 1]);

	TrackWasSwitchedDelegate.Broadcast();

	if (PlayerStateRef.IsValid() && PlayerStateRef->Get_Music_Volume() <= 0.0f)
		return;

	StartPlayingCurrentTrack();
}

void ACPP_SoundManager::StartPlayingNextTrackFromActivePlaylist()
{
	StopPlayingCurrentTrack();
	if (CurrentTrackIndex + 1 >= ActivePlaylistTracks.Num())
	{
		CurrentTrackIndex = 0;
	}
	else
	{
		CurrentTrackIndex++;
	}
	UpdateCurrentTrackNumber();

	TrackWasSwitchedDelegate.Broadcast();

	if (PlayerStateRef.IsValid() && PlayerStateRef->Get_Music_Volume() <= 0.0f)
		return;

	StartPlayingCurrentTrack();
}

void ACPP_SoundManager::StartPlayingPreviousTrack()
{
	StopPlayingCurrentTrack();

	if (CurrentTrackNumber - 1 < 1)
	{
		CurrentTrackNumber = GeneralNumberOfTracks;
	}
	else
	{
		CurrentTrackNumber--;
	}
	CurrentTrackIndex = ActivePlaylistTracks.Find(AllAudioComponents[CurrentTrackNumber - 1]);

	if (PlayerStateRef.IsValid() && PlayerStateRef->Get_Music_Volume() <= 0.0f)
		return;

	StartPlayingCurrentTrack();
}

void ACPP_SoundManager::TurnOnShouldReactOnTrackEnding()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_TurnOnShouldReactOnTrackEnding))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_TurnOnShouldReactOnTrackEnding);
	}
	bShouldReactOnTrackEnding = true;
}

void ACPP_SoundManager::StopPlayingCurrentTrack()
{
	bShouldReactOnTrackEnding = false;

	if (GetWorld()->GetTimerManager().TimerExists(TH_TurnOnShouldReactOnTrackEnding))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_TurnOnShouldReactOnTrackEnding);
	}
	GetWorld()->GetTimerManager().SetTimer(TH_TurnOnShouldReactOnTrackEnding,
	                                       this,
	                                       &ACPP_SoundManager::TurnOnShouldReactOnTrackEnding,
	                                       1.5f,
	                                       false);

	if (CurrentTrackNumber < 1 || CurrentTrackNumber > GeneralNumberOfTracks)
	{
		for (int32 i = 0; GeneralNumberOfTracks; i++)
		{
			if (AllAudioComponents[i]->IsPlaying())
			{
				AllAudioComponents[i]->Stop();
			}
		}
	}
	else if (AllAudioComponents[CurrentTrackNumber - 1]->IsPlaying())
	{
		AllAudioComponents[CurrentTrackNumber - 1]->Stop();
	}
	SetTrackIsPaused(false);
}

void ACPP_SoundManager::StartPlayingCurrentTrack()
{
	if (CurrentTrackNumber >= 1 && CurrentTrackNumber <= GeneralNumberOfTracks)
	{
		if (AllAudioComponents[CurrentTrackNumber - 1]->bIsPaused)
		{
			AllAudioComponents[CurrentTrackNumber - 1]->SetPaused(false);
			SetTrackIsPaused(false);
		}
		else if (!AllAudioComponents[CurrentTrackNumber - 1]->IsPlaying())
		{
			AllAudioComponents[CurrentTrackNumber - 1]->Play();
		}
	}
}

void ACPP_SoundManager::PauseCurrentTrack()
{
	if (CurrentTrackNumber >= 1 && CurrentTrackNumber <= GeneralNumberOfTracks)
	{
		if (AllAudioComponents[CurrentTrackNumber - 1]->IsPlaying())
		{
			AllAudioComponents[CurrentTrackNumber - 1]->SetPaused(true);
			SetTrackIsPaused(true);
		}
	}
}

void ACPP_SoundManager::UnpauseCurrentTrack()
{
	if (CurrentTrackNumber >= 1 && CurrentTrackNumber <= GeneralNumberOfTracks)
	{
		if (AllAudioComponents[CurrentTrackNumber - 1]->bIsPaused)
		{
			AllAudioComponents[CurrentTrackNumber - 1]->SetPaused(false);
		}
		else
		{
			AllAudioComponents[CurrentTrackNumber - 1]->Play();
		}
		SetTrackIsPaused(false);
	}
}

void ACPP_SoundManager::AddTrackToActivePlaylist(const uint8 TrackNumber)
{
	if (TrackNumber < 1 || TrackNumber > GeneralNumberOfTracks)
		return;

	if (ActivePlaylistTracks.Find(AllAudioComponents[TrackNumber - 1]) != -1)
		return;

	int32 IndexToInsert = -1;
	for (int32 i = TrackNumber; i < GeneralNumberOfTracks; i++)
	{
		IndexToInsert = ActivePlaylistTracks.Find(AllAudioComponents[i]);
		if (IndexToInsert != -1)
			break;
	}
	if (IndexToInsert != -1)
	{
		ActivePlaylistTracks.Insert(AllAudioComponents[TrackNumber - 1], IndexToInsert);
		ActiveTracksNumbers.Insert(TrackNumber, IndexToInsert);
	}
	else
	{
		IndexToInsert = ActivePlaylistTracks.Num();
		ActivePlaylistTracks.Emplace(AllAudioComponents[TrackNumber - 1]);
		ActiveTracksNumbers.Add(TrackNumber);
	}

	if (ActivePlaylistTracks.Num() == 0)
	{
		CurrentTrackIndex = -1;
		return;
	}
	if (TrackNumber == CurrentTrackNumber)
	{
		CurrentTrackIndex = ActiveTracksNumbers.Find(TrackNumber);
		if (CurrentTrackIndex != -1)
			return;
	}

	if (IndexToInsert != -1 && CurrentTrackIndex >= IndexToInsert)
	{
		if (CurrentTrackIndex + 1 >= ActivePlaylistTracks.Num())
		{
			CurrentTrackIndex = 0;
		}
		else
		{
			CurrentTrackIndex++;
		}
	}
}

void ACPP_SoundManager::RemoveTrackFromActivePlaylist(const uint8 TrackNumber)
{
	if (TrackNumber < 1 || TrackNumber > GeneralNumberOfTracks)
		return;

	if (ActivePlaylistTracks.Contains(AllAudioComponents[TrackNumber - 1]))
	{
		const int32 IndexInAllAudioComponentsToRemove = TrackNumber - 1;
		const int32 IndexToRemoveFromActivePlaylist = ActivePlaylistTracks.Find(
			AllAudioComponents[IndexInAllAudioComponentsToRemove]);
		ActivePlaylistTracks.Remove(AllAudioComponents[IndexInAllAudioComponentsToRemove]);
		ActiveTracksNumbers.Remove(TrackNumber);

		if (ActivePlaylistTracks.Num() == 0)
		{
			CurrentTrackIndex = -1;
		}
		else if (CurrentTrackIndex == IndexToRemoveFromActivePlaylist && IndexToRemoveFromActivePlaylist ==
			ActiveTracksNumbers.Num())
		{
			CurrentTrackIndex = 0;
		}
		else if (CurrentTrackIndex > IndexToRemoveFromActivePlaylist)
		{
			CurrentTrackIndex--;
		}
	}
}

void ACPP_SoundManager::StartPlayingUserPlaylist()
{
	if (PlayerStateRef.IsValid() && PlayerStateRef->Get_Music_Volume() <= 0.0f)
		return;

	if (ActivePlaylistTracks.Num() <= 0)
	{
		StopPlayingCurrentTrack();
		CurrentTrackIndex = -1;
		CurrentTrackNumber = -1;
		return;
	}

	if (CurrentTrackIndex == -1)
	{
		CurrentTrackIndex = 0;
	}
	if (GetCurrentAudioComponent() != ActivePlaylistTracks[CurrentTrackIndex])
	{
		StopPlayingCurrentTrack();
		UpdateCurrentTrackNumber();
	}

	StartPlayingCurrentTrack();
}

bool ACPP_SoundManager::IsAnyTrackPlaying()
{
	bool bIsPlaying;

	if (CurrentTrackNumber >= 1 && CurrentTrackNumber <= GeneralNumberOfTracks)
	{
		bIsPlaying = AllAudioComponents[CurrentTrackNumber - 1]->IsPlaying() &&
			!AllAudioComponents[CurrentTrackNumber - 1]->bIsPaused;
	}
	else
	{
		bIsPlaying = false;
		for (int32 i = 0; i < GeneralNumberOfTracks; i++)
		{
			if (AllAudioComponents[i]->IsPlaying() &&
				!AllAudioComponents[i]->bIsPaused)
			{
				bIsPlaying = true;
				break;
			}
		}
	}

	return bIsPlaying;
}

void ACPP_SoundManager::SetPlaylistRepeatingMode(const EPlaylistRepeatingMode NewValue)
{
	PlaylistRepeatingMode = NewValue;
}

EPlaylistRepeatingMode ACPP_SoundManager::GetPlaylistRepeatingMode() const
{
	return PlaylistRepeatingMode;
}

void ACPP_SoundManager::SetPlaylistRepeatingModeAsInt(const int32 NewValue)
{
	PlaylistRepeatingMode = static_cast<EPlaylistRepeatingMode>(NewValue);
}

int32 ACPP_SoundManager::GetPlaylistRepeatingModeAsInt() const
{
	return static_cast<int32>(PlaylistRepeatingMode);
}

UAudioComponent* ACPP_SoundManager::GetCurrentAudioComponent() const
{
	if (CurrentTrackNumber >= 1 && CurrentTrackNumber <= GeneralNumberOfTracks)
	{
		return AllAudioComponents[CurrentTrackNumber - 1];
	}
	return nullptr;
}

TArray<uint8>& ACPP_SoundManager::GetActivePlaylistTracksNumbers()
{
	return ActiveTracksNumbers;
}

void ACPP_SoundManager::SetPlayerControllerRef(ACPP_PlayerController* NewPlayerController)
{
	if (IsValid(NewPlayerController))
	{
		PlayerControllerRef = NewPlayerController;
		PlayerStateRef = PlayerControllerRef->GetPlayerState<ACPP_PlayerState>();
		if (!PlayerStateRef.IsValid() && IsValid(PlayerControllerRef->GetCharacter()))
		{
			PlayerStateRef = PlayerControllerRef->GetCharacter()->GetPlayerState<ACPP_PlayerState>();
		}
	}
}
