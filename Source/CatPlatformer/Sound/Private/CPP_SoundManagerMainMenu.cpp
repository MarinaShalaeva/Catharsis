// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SoundManagerMainMenu.h"
#include "Components/AudioComponent.h"

ACPP_SoundManagerMainMenu::ACPP_SoundManagerMainMenu() : ACPP_SoundManager()
{
	MM_TrackAudioComponent_1 = CreateDefaultSubobject<UAudioComponent>(TEXT("Main Menu Audio Component 1"));
	MM_TrackAudioComponent_1->SetupAttachment(RootComponent);
	MM_TrackAudioComponent_1->bIsUISound = true;

	MM_TrackAudioComponent_2 = CreateDefaultSubobject<UAudioComponent>(TEXT("Main Menu Audio Component 2"));
	MM_TrackAudioComponent_2->SetupAttachment(RootComponent);
	MM_TrackAudioComponent_2->bIsUISound = true;

	MM_TrackAudioComponent_3 = CreateDefaultSubobject<UAudioComponent>(TEXT("Main Menu Audio Component 3"));
	MM_TrackAudioComponent_3->SetupAttachment(RootComponent);
	MM_TrackAudioComponent_3->bIsUISound = true;

	GeneralNumberOfTracks = 3;
}

void ACPP_SoundManagerMainMenu::BeginPlay()
{
	Super::BeginPlay();

	MM_TrackAudioComponent_1->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	MM_TrackAudioComponent_2->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	MM_TrackAudioComponent_3->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
}

void ACPP_SoundManagerMainMenu::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MM_TrackAudioComponent_1->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	MM_TrackAudioComponent_2->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	MM_TrackAudioComponent_3->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);

	Super::EndPlay(EndPlayReason);
}

void ACPP_SoundManagerMainMenu::FillAllAudioComponentsArray()
{
	if (AllAudioComponents.Num() == 3)
		return;
	if (AllAudioComponents.Num() != 0)
	{
		AllAudioComponents.Reset(3);
	}
	else
	{
		AllAudioComponents.Reserve(3);
	}
	AllAudioComponents.Emplace(MM_TrackAudioComponent_1);
	AllAudioComponents.Emplace(MM_TrackAudioComponent_2);
	AllAudioComponents.Emplace(MM_TrackAudioComponent_3);
}

void ACPP_SoundManagerMainMenu::ApplySettingsFromTheSaveFile(const bool bSetDefaultValues,
                                                             const bool bIsAdditionalSoundManager)
{
	Super::ApplySettingsFromTheSaveFile(bSetDefaultValues, bIsAdditionalSoundManager);

	if (!bSetDefaultValues && IsValid(PlayerStateRef))
	{
		PlaylistRepeatingMode = PlayerStateRef->Get_MM_PlaylistRepeatingMode();

		for (const auto& Number : PlayerStateRef->Get_MM_ActiveTracksNumbers())
		{
			AddTrackToActivePlaylist(Number);
		}

		CurrentTrackIndex = -1;
		CurrentTrackNumber = PlayerStateRef->Get_MM_LastTrackNumber();
		if (CurrentTrackNumber < 1 || CurrentTrackNumber > 3)
		{
			CurrentTrackIndex = -1;
		}
		else
		{
			CurrentTrackIndex = ActivePlaylistTracks.Find(AllAudioComponents[CurrentTrackNumber - 1]);
		}

		if (CurrentTrackIndex < 0 || CurrentTrackIndex > 2 ||
			CurrentTrackIndex == GeneralNumberOfTracks - 1 && PlaylistRepeatingMode == EPlaylistRepeatingMode::NoRepeat)
		{
			CurrentTrackIndex = 0;
			UpdateCurrentTrackNumber();

			if (!bIsAdditionalSoundManager && PlayerStateRef->Get_Music_Volume() != 0)
			{
				StartPlayingCurrentTrack();
			}
		}
		else if (PlaylistRepeatingMode == EPlaylistRepeatingMode::RepeatOneTrack &&
			!bIsAdditionalSoundManager && PlayerStateRef->Get_Music_Volume() != 0)
		{
			StartPlayingCurrentTrack();
		}
		else if (!bIsAdditionalSoundManager && PlayerStateRef->Get_Music_Volume() != 0)
		{
			StartPlayingNextTrackFromActivePlaylist();
		}
	}
}
