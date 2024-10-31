// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SoundManagerLevel.h"
#include "Components/AudioComponent.h"

ACPP_SoundManagerLevel::ACPP_SoundManagerLevel() : ACPP_SoundManager()
{
	L_TrackAudioComponent_1 = CreateDefaultSubobject<UAudioComponent>(TEXT("Level Audio Component 1"));
	L_TrackAudioComponent_1->SetupAttachment(RootComponent);
	L_TrackAudioComponent_1->bIsUISound = true;

	L_TrackAudioComponent_2 = CreateDefaultSubobject<UAudioComponent>(TEXT("Level Audio Component 2"));
	L_TrackAudioComponent_2->SetupAttachment(RootComponent);
	L_TrackAudioComponent_2->bIsUISound = true;

	L_TrackAudioComponent_3 = CreateDefaultSubobject<UAudioComponent>(TEXT("Level Audio Component 3"));
	L_TrackAudioComponent_3->SetupAttachment(RootComponent);
	L_TrackAudioComponent_3->bIsUISound = true;

	L_TrackAudioComponent_4 = CreateDefaultSubobject<UAudioComponent>(TEXT("Level Audio Component 4"));
	L_TrackAudioComponent_4->SetupAttachment(RootComponent);
	L_TrackAudioComponent_4->bIsUISound = true;

	L_TrackAudioComponent_5 = CreateDefaultSubobject<UAudioComponent>(TEXT("Level Audio Component 5"));
	L_TrackAudioComponent_5->SetupAttachment(RootComponent);
	L_TrackAudioComponent_5->bIsUISound = true;

	GeneralNumberOfTracks = 5;
}

void ACPP_SoundManagerLevel::BeginPlay()
{
	Super::BeginPlay();

	L_TrackAudioComponent_1->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_2->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_3->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_4->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_5->OnAudioFinished.AddDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
}

void ACPP_SoundManagerLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	L_TrackAudioComponent_1->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_2->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_3->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_4->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);
	L_TrackAudioComponent_5->OnAudioFinished.RemoveDynamic(this, &ACPP_SoundManager::AudioTrackWasEnded);

	Super::EndPlay(EndPlayReason);
}

void ACPP_SoundManagerLevel::FillAllAudioComponentsArray()
{
	if (AllAudioComponents.Num() == 5)
		return;
	if (AllAudioComponents.Num() != 0)
	{
		AllAudioComponents.Reset(5);
	}
	else
	{
		AllAudioComponents.Reserve(5);
	}
	AllAudioComponents.Emplace(L_TrackAudioComponent_1);
	AllAudioComponents.Emplace(L_TrackAudioComponent_2);
	AllAudioComponents.Emplace(L_TrackAudioComponent_3);
	AllAudioComponents.Emplace(L_TrackAudioComponent_4);
	AllAudioComponents.Emplace(L_TrackAudioComponent_5);
}

void ACPP_SoundManagerLevel::ApplySettingsFromTheSaveFile(const bool bSetDefaultValues,
                                                          const bool bIsAdditionalSoundManager)
{
	Super::ApplySettingsFromTheSaveFile(bSetDefaultValues, bIsAdditionalSoundManager);

	if (!bSetDefaultValues && IsValid(PlayerStateRef))
	{
		PlaylistRepeatingMode = PlayerStateRef->Get_L_PlaylistRepeatingMode();

		for (const auto& Number : PlayerStateRef->Get_L_ActiveTracksNumbers())
		{
			AddTrackToActivePlaylist(Number);
		}

		CurrentTrackIndex = -1;
		CurrentTrackNumber = PlayerStateRef->Get_L_LastTrackNumber();
		if (CurrentTrackNumber < 1 || CurrentTrackNumber > 5)
		{
			CurrentTrackIndex = -1;
		}
		else
		{
			CurrentTrackIndex = ActivePlaylistTracks.Find(AllAudioComponents[CurrentTrackNumber - 1]);
		}

		if (CurrentTrackIndex < 0 || CurrentTrackIndex > 4 ||
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
