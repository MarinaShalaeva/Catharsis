// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Settings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_Settings::UWCPP_Settings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              PanelsSwitcher(nullptr),
                                                                              PanelsNumber(0),
                                                                              SoundManagersDataTable(nullptr),
                                                                              SoundManagerMainMenuRef(nullptr),
                                                                              SoundManagerLevelRef(nullptr),
                                                                              PlayerStateRef(nullptr),
                                                                              bIsMainMenu(false),
                                                                              BottomPanel_WidgetSwitcher(nullptr),
                                                                              OpenPreviousPanelButton(nullptr),
                                                                              OpenNextPanelButton(nullptr),
                                                                              SFX_Volume_Slider(nullptr),
                                                                              Music_Volume_Slider(nullptr),
                                                                              MM_RepeatingMode(),
                                                                              MM_Repeat_Button(nullptr),
                                                                              MM_PreviousTrack_Button(nullptr),
                                                                              MM_NextTrack_Button(nullptr),
                                                                              MM_PlayPause_Button(nullptr),
                                                                              TB_MainMenuTrackName(nullptr),
                                                                              Music_EchoOfSadness_CheckBox(nullptr),
                                                                              Music_Memories_CheckBox(nullptr),
                                                                              Music_Tenderness_CheckBox(nullptr),
                                                                              L_RepeatingMode(),
                                                                              L_Repeat_Button(nullptr),
                                                                              L_PreviousTrack_Button(nullptr),
                                                                              L_NextTrack_Button(nullptr),
                                                                              L_PlayPause_Button(nullptr),
                                                                              TB_LevelTrackName(nullptr),
                                                                              Music_CreativeMinds_CheckBox(nullptr),
                                                                              Music_Elevate_CheckBox(nullptr),
                                                                              Music_GroovyHipHop_CheckBox(nullptr),
                                                                              Music_Punky_CheckBox(nullptr),
                                                                              Music_Rumble_CheckBox(nullptr),
                                                                              GameUserSettings(nullptr),
                                                                              TB_CurrentScreenResolution(nullptr),
                                                                              ApplyChangesButton(nullptr),
                                                                              ResolutionLeftButton(nullptr),
                                                                              ResolutionWidgetSwitcher(nullptr),
                                                                              ResolutionRightButton(nullptr),
                                                                              TextureQualityLeftButton(nullptr),
                                                                              TextureQualityWidgetSwitcher(nullptr),
                                                                              TextureQualityRightButton(nullptr),
                                                                              ShadowsQualityLeftButton(nullptr),
                                                                              ShadowsQualityWidgetSwitcher(nullptr),
                                                                              ShadowsQualityRightButton(nullptr)
{
	MM_RepeatingMode = EPlaylistRepeatingMode::RepeatPlaylist;
	Music_EchoOfSadness_Inscription = FText::FromString(TEXT("Echo Of Sadness"));
	Music_Memories_Inscription = FText::FromString(TEXT("Memories"));
	Music_Tenderness_Inscription = FText::FromString(TEXT("Tenderness"));

	L_RepeatingMode = EPlaylistRepeatingMode::RepeatPlaylist;
	Music_CreativeMinds_Inscription = FText::FromString(TEXT("Creative Minds"));
	Music_Elevate_Inscription = FText::FromString(TEXT("Elevate"));
	Music_GroovyHipHop_Inscription = FText::FromString(TEXT("Groovy Hip Hop"));
	Music_Punky_Inscription = FText::FromString(TEXT("Punky"));
	Music_Rumble_Inscription = FText::FromString(TEXT("Rumble"));

	CurrentScreenResolution = FIntPoint(1920, 1080);
}

void UWCPP_Settings::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(GameInstanceRef))
	{
		GameInstanceRef = GetGameInstance<UCPP_GameInstance>();
	}
	if (!IsValid(PlayerControllerRef))
	{
		PlayerControllerRef = GetOwningPlayer<ACPP_PlayerController>();
	}
	if (IsValid(PlayerControllerRef))
	{
		if (IsValid(PlayerControllerRef) && IsValid(PlayerControllerRef->GetCharacterRef())
			&& IsValid(PlayerControllerRef->GetCharacterRef()->GetPlayerStateRef()))
		{
			PlayerStateRef = PlayerControllerRef->GetCharacterRef()->GetPlayerStateRef();
		}
		else if (IsValid(PlayerControllerRef->GetPlayerState<ACPP_PlayerState>()))
		{
			PlayerStateRef = PlayerControllerRef->GetPlayerState<ACPP_PlayerState>();
		}

		bIsGamepadMode = PlayerControllerRef->GetIsGamepadMode();
		if (bIsGamepadMode)
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}

		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.AddUObject(
			this, &UWCPP_Settings::AnyKeyPressed);
		DH_GamepadHorizontalScroll = PlayerControllerRef->GamepadHorizontalScrollDelegate.AddUObject(
			this, &UWCPP_Settings::GamepadHorizontalScrollCalled);

		if (bIsMainMenu)
		{
			SoundManagerMainMenuRef = Cast<ACPP_SoundManagerMainMenu>(PlayerControllerRef->GetSoundManagerRef());
			InitializeMainMenuTracksPanel();

			if (UClass* SoundManagerClass = GameInstanceRef->GetActorClassBySoftReference(
				UCPP_StaticWidgetLibrary::GetSoftReferenceToSoundManagerByRowName(
					SoundManagersDataTable,
					FName(TEXT("LevelSoundManager")))))
			{
				ACPP_SoundManagerLevel* TmpSoundManager = GetWorld()->SpawnActorDeferred<ACPP_SoundManagerLevel>(
					SoundManagerClass,
					FTransform::Identity,
					nullptr,
					nullptr,
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

				if (IsValid(TmpSoundManager))
				{
					TmpSoundManager->SetPlayerControllerRef(PlayerControllerRef);
					TmpSoundManager->ApplySettingsFromTheSaveFile(
						IsValid(PlayerStateRef)
							? !PlayerStateRef->GetSaveFileWasCreated()
							: true, true);
					SoundManagerLevelRef = TmpSoundManager;
					UGameplayStatics::FinishSpawningActor(TmpSoundManager, FTransform());
					InitializeLevelTracksPanel();
				}
			}
		}
		else
		{
			SoundManagerLevelRef = Cast<ACPP_SoundManagerLevel>(PlayerControllerRef->GetSoundManagerRef());
			InitializeLevelTracksPanel();

			if (UClass* SoundManagerClass = GameInstanceRef->GetActorClassBySoftReference(
				UCPP_StaticWidgetLibrary::GetSoftReferenceToSoundManagerByRowName(
					SoundManagersDataTable,
					FName(TEXT("MainMenuSoundManager")))))
			{
				ACPP_SoundManagerMainMenu* TmpSoundManager = GetWorld()->SpawnActorDeferred<ACPP_SoundManagerMainMenu>(
					SoundManagerClass,
					FTransform::Identity,
					nullptr,
					nullptr,
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

				if (IsValid(TmpSoundManager))
				{
					TmpSoundManager->SetPlayerControllerRef(PlayerControllerRef);
					TmpSoundManager->ApplySettingsFromTheSaveFile(
						IsValid(PlayerStateRef)
							? !PlayerStateRef->GetSaveFileWasCreated()
							: true, true);
					SoundManagerMainMenuRef = TmpSoundManager;
					UGameplayStatics::FinishSpawningActor(TmpSoundManager, FTransform());
					InitializeMainMenuTracksPanel();
				}
			}
		}
	}

	//=====================All Panels==============================

	PanelsNumber = PanelsSwitcher->GetNumWidgets();

	//===================Bottom Panel==============================

	OpenPreviousPanelButton->OnClicked.AddDynamic(this, &UWCPP_Settings::LeftArrowOnClick);
	OpenNextPanelButton->OnClicked.AddDynamic(this, &UWCPP_Settings::RightArrowOnClick);

	//====================Audio Panel==============================

	if (IsValid(PlayerStateRef))
	{
		SFX_Volume_Slider->SetValue(PlayerStateRef->Get_SFX_Volume());
		Music_Volume_Slider->SetValue(PlayerStateRef->Get_Music_Volume());
	}

	SFX_Volume_Slider->OnValueChanged.AddDynamic(this, &UWCPP_Settings::SFX_Volume_Slider_OnValueChanged);
	Music_Volume_Slider->OnValueChanged.AddDynamic(this, &UWCPP_Settings::Music_Volume_Slider_OnValueChanged);

	MM_Repeat_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::MM_Repeat_Button_OnClick);
	MM_PreviousTrack_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::MM_PreviousTrack_Button_OnClick);
	MM_NextTrack_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::MM_NextTrack_Button_OnClick);
	MM_PlayPause_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::MM_PlayPause_Button_OnClick);
	Music_EchoOfSadness_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_EchoOfSadness_OnCheckStateChanged);
	Music_Memories_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_Memories_OnCheckStateChanged);
	Music_Tenderness_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_Tenderness_OnCheckStateChanged);

	L_Repeat_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::L_Repeat_Button_OnClick);
	L_PreviousTrack_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::L_PreviousTrack_Button_OnClick);
	L_NextTrack_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::L_NextTrack_Button_OnClick);
	L_PlayPause_Button->OnClicked.AddDynamic(this, &UWCPP_Settings::L_PlayPause_Button_OnClick);
	Music_CreativeMinds_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_CreativeMinds_OnCheckStateChanged);
	Music_Elevate_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_Elevate_OnCheckStateChanged);
	Music_GroovyHipHop_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_GroovyHipHop_OnCheckStateChanged);
	Music_Punky_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_Punky_OnCheckStateChanged);
	Music_Rumble_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_Settings::Music_Rumble_OnCheckStateChanged);

	//=================Video Quality Panel=========================

	GameUserSettings = UGameUserSettings::GetGameUserSettings();
	CurrentScreenResolution = GameUserSettings->GetScreenResolution();
	SetCurrentResolutionToTextBlock();

	if (const int32 TextureQuality = GameUserSettings->GetTextureQuality();
		TextureQuality < TextureQualityWidgetSwitcher->GetNumWidgets())
	{
		TextureQualityWidgetSwitcher->SetActiveWidgetIndex(TextureQuality);
	}

	if (const int32 ShadowQuality = GameUserSettings->GetShadowQuality();
		ShadowQuality < ShadowsQualityWidgetSwitcher->GetNumWidgets())
	{
		ShadowsQualityWidgetSwitcher->SetActiveWidgetIndex(ShadowQuality);
	}

	ApplyChangesButton->OnClicked.AddDynamic(this, &UWCPP_Settings::ApplyChangesButtonOnClick);
	ResolutionLeftButton->OnClicked.AddDynamic(this, &UWCPP_Settings::ResolutionLeftButtonOnClick);
	ResolutionRightButton->OnClicked.AddDynamic(this, &UWCPP_Settings::ResolutionRightButtonOnClick);
	TextureQualityLeftButton->OnClicked.AddDynamic(this, &UWCPP_Settings::TextureQualityLeftButtonOnClick);
	TextureQualityRightButton->OnClicked.AddDynamic(this, &UWCPP_Settings::TextureQualityRightButtonOnClick);
	ShadowsQualityLeftButton->OnClicked.AddDynamic(this, &UWCPP_Settings::ShadowsQualityLeftButtonOnClick);
	ShadowsQualityRightButton->OnClicked.AddDynamic(this, &UWCPP_Settings::ShadowsQualityRightButtonOnClick);

	//================Gamepad & Keyboard===========================
}

void UWCPP_Settings::NativeDestruct()
{
	if (IsValid(SoundManagerMainMenuRef) && IsValid(SoundManagerLevelRef))
	{
		SoundManagerMainMenuRef->TrackWasSwitchedDelegate.Remove(DH_MM_TrackWasSwitched);
		DH_MM_TrackWasSwitched.Reset();

		SoundManagerLevelRef->TrackWasSwitchedDelegate.Remove(DH_L_TrackWasSwitched);
		DH_L_TrackWasSwitched.Reset();

		SoundManagerMainMenuRef->UpdateCurrentTrackNumber();
		SoundManagerLevelRef->UpdateCurrentTrackNumber();
		if (bIsMainMenu)
		{
			SoundManagerLevelRef->StopPlayingCurrentTrack();
			if (SoundManagerMainMenuRef->IsAnyTrackPlaying())
			{
				SoundManagerMainMenuRef->StartPlayingUserPlaylist();
			}

			if (IsValid(PlayerStateRef))
			{
				PlayerStateRef->Set_MM_ActiveTracksNumbers(SoundManagerMainMenuRef->
					GetActivePlaylistTracksNumbers());
				PlayerStateRef->Set_MM_LastTrackNumber(SoundManagerMainMenuRef->
					GetCurrentTrackNumber());
				PlayerStateRef->Set_MM_PlaylistRepeatingMode(SoundManagerMainMenuRef->
					GetPlaylistRepeatingMode());

				PlayerStateRef->Set_L_ActiveTracksNumbers(SoundManagerLevelRef->
					GetActivePlaylistTracksNumbers());
				PlayerStateRef->Set_L_LastTrackNumber(SoundManagerLevelRef->
					GetCurrentTrackNumber());
				PlayerStateRef->Set_L_PlaylistRepeatingMode(SoundManagerLevelRef->
					GetPlaylistRepeatingMode());

				PlayerStateRef->CollectAudioInfoForSavingItToFile();

				if (PlayerStateRef->Get_Music_Volume() <= 0.0f)
				{
					SoundManagerMainMenuRef->PauseCurrentTrack();
				}
			}

			SoundManagerLevelRef->Destroy();
			SoundManagerMainMenuRef = nullptr;
		}
		else
		{
			SoundManagerMainMenuRef->StopPlayingCurrentTrack();
			if (SoundManagerLevelRef->IsAnyTrackPlaying())
			{
				SoundManagerLevelRef->StartPlayingUserPlaylist();
			}

			if (IsValid(PlayerStateRef))
			{
				PlayerStateRef->Set_MM_ActiveTracksNumbers(SoundManagerMainMenuRef->
					GetActivePlaylistTracksNumbers());
				PlayerStateRef->Set_MM_LastTrackNumber(SoundManagerMainMenuRef->
					GetCurrentTrackNumber());
				PlayerStateRef->Set_MM_PlaylistRepeatingMode(SoundManagerMainMenuRef->
					GetPlaylistRepeatingMode());

				PlayerStateRef->Set_L_ActiveTracksNumbers(SoundManagerLevelRef->
					GetActivePlaylistTracksNumbers());
				PlayerStateRef->Set_L_LastTrackNumber(SoundManagerLevelRef->
					GetCurrentTrackNumber());
				PlayerStateRef->Set_L_PlaylistRepeatingMode(SoundManagerLevelRef->
					GetPlaylistRepeatingMode());

				PlayerStateRef->CollectAudioInfoForSavingItToFile();

				if (PlayerStateRef->Get_Music_Volume() <= 0.0f)
				{
					SoundManagerLevelRef->StopPlayingCurrentTrack();
				}
			}

			SoundManagerMainMenuRef->Destroy();
			SoundManagerLevelRef = nullptr;
		}
	}

	//===================Bottom Panel==============================

	OpenPreviousPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::LeftArrowOnClick);
	OpenNextPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::RightArrowOnClick);

	//====================Audio Panel==============================

	SFX_Volume_Slider->OnValueChanged.RemoveDynamic(this, &UWCPP_Settings::SFX_Volume_Slider_OnValueChanged);
	Music_Volume_Slider->OnValueChanged.RemoveDynamic(this, &UWCPP_Settings::Music_Volume_Slider_OnValueChanged);

	MM_Repeat_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::MM_Repeat_Button_OnClick);
	MM_PreviousTrack_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::MM_PreviousTrack_Button_OnClick);
	MM_NextTrack_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::MM_NextTrack_Button_OnClick);
	MM_PlayPause_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::MM_PlayPause_Button_OnClick);
	Music_EchoOfSadness_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_EchoOfSadness_OnCheckStateChanged);
	Music_Memories_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_Memories_OnCheckStateChanged);
	Music_Tenderness_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_Tenderness_OnCheckStateChanged);

	L_Repeat_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::L_Repeat_Button_OnClick);
	L_PreviousTrack_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::L_PreviousTrack_Button_OnClick);
	L_NextTrack_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::L_NextTrack_Button_OnClick);
	L_PlayPause_Button->OnClicked.RemoveDynamic(this, &UWCPP_Settings::L_PlayPause_Button_OnClick);
	Music_CreativeMinds_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_CreativeMinds_OnCheckStateChanged);
	Music_Elevate_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_Elevate_OnCheckStateChanged);
	Music_GroovyHipHop_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_GroovyHipHop_OnCheckStateChanged);
	Music_Punky_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_Punky_OnCheckStateChanged);
	Music_Rumble_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_Settings::Music_Rumble_OnCheckStateChanged);

	//=================Video Quality Panel=========================

	ApplyChangesButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::ApplyChangesButtonOnClick);
	ResolutionLeftButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::ResolutionLeftButtonOnClick);
	ResolutionRightButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::ResolutionRightButtonOnClick);
	TextureQualityLeftButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::TextureQualityLeftButtonOnClick);
	TextureQualityRightButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::TextureQualityRightButtonOnClick);
	ShadowsQualityLeftButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::ShadowsQualityLeftButtonOnClick);
	ShadowsQualityRightButton->OnClicked.RemoveDynamic(this, &UWCPP_Settings::ShadowsQualityRightButtonOnClick);

	//================Gamepad & Keyboard===========================

	Super::NativeDestruct();
}

void UWCPP_Settings::RightArrowOnClick()
{
	if (const int32 CurrentIndex = PanelsSwitcher->GetActiveWidgetIndex();
		CurrentIndex + 1 == PanelsNumber)
	{
		PanelsSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		PanelsSwitcher->SetActiveWidgetIndex(CurrentIndex + 1);
	}
}

void UWCPP_Settings::LeftArrowOnClick()
{
	if (const int32 CurrentIndex = PanelsSwitcher->GetActiveWidgetIndex();
		CurrentIndex - 1 == -1)
	{
		PanelsSwitcher->SetActiveWidgetIndex(PanelsNumber - 1);
	}
	else
	{
		PanelsSwitcher->SetActiveWidgetIndex(CurrentIndex - 1);
	}
}

void UWCPP_Settings::InitializeMainMenuTracksPanel()
{
	if (IsValid(SoundManagerMainMenuRef))
	{
		for (const auto& TrackNumber : SoundManagerMainMenuRef->GetActivePlaylistTracksNumbers())
		{
			switch (TrackNumber)
			{
			case 1:
				Music_EchoOfSadness_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			case 2:
				Music_Memories_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			case 3:
				Music_Tenderness_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			default:
				break;
			}
		}
		MM_UpdateTrackNameTextBlock();
		ChangeRepeatImage(true, SoundManagerMainMenuRef->GetPlaylistRepeatingMode());
		if (SoundManagerMainMenuRef->IsAnyTrackPlaying())
		{
			ChangePlayPauseImage(true, true);
		}
		else
		{
			ChangePlayPauseImage(true, false);
		}
		DH_MM_TrackWasSwitched = SoundManagerMainMenuRef->TrackWasSwitchedDelegate.AddUObject(
			this, &UWCPP_Settings::MM_UpdateTrackNameTextBlock);
	}
}

void UWCPP_Settings::InitializeLevelTracksPanel()
{
	if (IsValid(SoundManagerLevelRef))
	{
		for (const auto& TrackNumber : SoundManagerLevelRef->GetActivePlaylistTracksNumbers())
		{
			switch (TrackNumber)
			{
			case 1:
				Music_CreativeMinds_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			case 2:
				Music_Elevate_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			case 3:
				Music_GroovyHipHop_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			case 4:
				Music_Punky_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			case 5:
				Music_Rumble_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				break;
			default:
				break;
			}
		}
		L_UpdateTrackNameTextBlock();
		ChangeRepeatImage(false, SoundManagerLevelRef->GetPlaylistRepeatingMode());
		if (SoundManagerLevelRef->IsAnyTrackPlaying())
		{
			ChangePlayPauseImage(false, true);
		}
		else
		{
			ChangePlayPauseImage(false, false);
		}
		DH_L_TrackWasSwitched = SoundManagerLevelRef->TrackWasSwitchedDelegate.AddUObject(
			this, &UWCPP_Settings::L_UpdateTrackNameTextBlock);
	}
}

void UWCPP_Settings::SFX_Volume_Slider_OnValueChanged(const float Value)
{
	if (bIsMainMenu && IsValid(SoundManagerMainMenuRef))
	{
		if (IsValid(PlayerStateRef))
		{
			PlayerStateRef->Set_SFX_Volume(Value);
		}
		SoundManagerMainMenuRef->SetNewSFXVolume(Value);
	}
	else if (!bIsMainMenu && IsValid(SoundManagerLevelRef))
	{
		if (IsValid(PlayerStateRef))
		{
			PlayerStateRef->Set_SFX_Volume(Value);
		}
		SoundManagerLevelRef->SetNewSFXVolume(Value);
	}
}

void UWCPP_Settings::Music_Volume_Slider_OnValueChanged(const float Value)
{
	if (bIsMainMenu && IsValid(SoundManagerMainMenuRef))
	{
		if (IsValid(PlayerStateRef))
		{
			PlayerStateRef->Set_Music_Volume(Value);
		}
		SoundManagerMainMenuRef->SetNewMusicVolume(Value);
	}
	else if (!bIsMainMenu && IsValid(SoundManagerLevelRef))
	{
		if (IsValid(PlayerStateRef))
		{
			PlayerStateRef->Set_Music_Volume(Value);
		}
		SoundManagerLevelRef->SetNewMusicVolume(Value);
	}
}

void UWCPP_Settings::MM_Repeat_Button_OnClick()
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	switch (SoundManagerMainMenuRef->GetPlaylistRepeatingMode())
	{
	case EPlaylistRepeatingMode::RepeatPlaylist:
		SoundManagerMainMenuRef->SetPlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatOneTrack);
		ChangeRepeatImage(true, EPlaylistRepeatingMode::RepeatOneTrack);
		break;
	case EPlaylistRepeatingMode::RepeatOneTrack:
		SoundManagerMainMenuRef->SetPlaylistRepeatingMode(EPlaylistRepeatingMode::NoRepeat);
		ChangeRepeatImage(true, EPlaylistRepeatingMode::NoRepeat);
		break;
	case EPlaylistRepeatingMode::NoRepeat:
		SoundManagerMainMenuRef->SetPlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatPlaylist);
		ChangeRepeatImage(true, EPlaylistRepeatingMode::RepeatPlaylist);
		break;
	default:
		break;
	}
}

void UWCPP_Settings::MM_PreviousTrack_Button_OnClick()
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	if (IsValid(SoundManagerLevelRef) && SoundManagerLevelRef->IsAnyTrackPlaying())
	{
		SoundManagerLevelRef->PauseCurrentTrack();
		ChangePlayPauseImage(false, false);
	}
	SoundManagerMainMenuRef->StartPlayingPreviousTrack();
	ChangePlayPauseImage(true, SoundManagerMainMenuRef->IsAnyTrackPlaying());
	MM_UpdateTrackNameTextBlock();
}

void UWCPP_Settings::MM_NextTrack_Button_OnClick()
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	if (IsValid(SoundManagerLevelRef) && SoundManagerLevelRef->IsAnyTrackPlaying())
	{
		SoundManagerLevelRef->PauseCurrentTrack();
		ChangePlayPauseImage(false, false);
	}
	SoundManagerMainMenuRef->StartPlayingNextTrack();
	ChangePlayPauseImage(true, SoundManagerMainMenuRef->IsAnyTrackPlaying());
	MM_UpdateTrackNameTextBlock();
}

void UWCPP_Settings::MM_PlayPause_Button_OnClick()
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	if (SoundManagerMainMenuRef->GetTrackIsPaused()) // Track was paused before.
	{
		SoundManagerMainMenuRef->UnpauseCurrentTrack();
		ChangePlayPauseImage(true, true);
		if (IsValid(SoundManagerLevelRef) && SoundManagerLevelRef->IsAnyTrackPlaying())
		{
			SoundManagerLevelRef->PauseCurrentTrack();
			ChangePlayPauseImage(false, false);
		}
	}
	else if (!SoundManagerMainMenuRef->IsAnyTrackPlaying()) // Track was stopped before.
	{
		SoundManagerMainMenuRef->StartPlayingCurrentTrack();
		ChangePlayPauseImage(true, true);
		if (IsValid(SoundManagerLevelRef) && SoundManagerLevelRef->IsAnyTrackPlaying())
		{
			SoundManagerLevelRef->PauseCurrentTrack();
			ChangePlayPauseImage(false, false);
		}
	}
	else // Track is playing.
	{
		SoundManagerMainMenuRef->PauseCurrentTrack();
		ChangePlayPauseImage(true, false);
	}
}

void UWCPP_Settings::MM_UpdateTrackNameTextBlock()
{
	switch (SoundManagerMainMenuRef->GetCurrentTrackNumber())
	{
	case 1:
		TB_MainMenuTrackName->SetText(Music_EchoOfSadness_Inscription);
		break;
	case 2:
		TB_MainMenuTrackName->SetText(Music_Memories_Inscription);
		break;
	case 3:
		TB_MainMenuTrackName->SetText(Music_Tenderness_Inscription);
		break;
	default:
		break;
	}
}

void UWCPP_Settings::Music_EchoOfSadness_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	if (bNewState)
	{
		SoundManagerMainMenuRef->AddTrackToActivePlaylist(1);
	}
	else
	{
		SoundManagerMainMenuRef->RemoveTrackFromActivePlaylist(1);
	}
}

void UWCPP_Settings::Music_Memories_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	if (bNewState)
	{
		SoundManagerMainMenuRef->AddTrackToActivePlaylist(2);
	}
	else
	{
		SoundManagerMainMenuRef->RemoveTrackFromActivePlaylist(2);
	}
}

void UWCPP_Settings::Music_Tenderness_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerMainMenuRef))
		return;

	if (bNewState)
	{
		SoundManagerMainMenuRef->AddTrackToActivePlaylist(3);
	}
	else
	{
		SoundManagerMainMenuRef->RemoveTrackFromActivePlaylist(3);
	}
}

void UWCPP_Settings::L_Repeat_Button_OnClick()
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	switch (SoundManagerLevelRef->GetPlaylistRepeatingMode())
	{
	case EPlaylistRepeatingMode::RepeatPlaylist:
		SoundManagerLevelRef->SetPlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatOneTrack);
		ChangeRepeatImage(false, EPlaylistRepeatingMode::RepeatOneTrack);
		break;
	case EPlaylistRepeatingMode::RepeatOneTrack:
		SoundManagerLevelRef->SetPlaylistRepeatingMode(EPlaylistRepeatingMode::NoRepeat);
		ChangeRepeatImage(false, EPlaylistRepeatingMode::NoRepeat);
		break;
	case EPlaylistRepeatingMode::NoRepeat:
		SoundManagerLevelRef->SetPlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatPlaylist);
		ChangeRepeatImage(false, EPlaylistRepeatingMode::RepeatPlaylist);
		break;
	default:
		break;
	}
}

void UWCPP_Settings::L_PreviousTrack_Button_OnClick()
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (IsValid(SoundManagerMainMenuRef) && SoundManagerMainMenuRef->IsAnyTrackPlaying())
	{
		SoundManagerMainMenuRef->PauseCurrentTrack();
		ChangePlayPauseImage(true, false);
	}
	SoundManagerLevelRef->StartPlayingPreviousTrack();
	ChangePlayPauseImage(false, SoundManagerLevelRef->IsAnyTrackPlaying());
	L_UpdateTrackNameTextBlock();
}

void UWCPP_Settings::L_NextTrack_Button_OnClick()
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (IsValid(SoundManagerMainMenuRef) && SoundManagerMainMenuRef->IsAnyTrackPlaying())
	{
		SoundManagerMainMenuRef->PauseCurrentTrack();
		ChangePlayPauseImage(true, false);
	}
	SoundManagerLevelRef->StartPlayingNextTrack();
	ChangePlayPauseImage(false, SoundManagerLevelRef->IsAnyTrackPlaying());
	L_UpdateTrackNameTextBlock();
}

void UWCPP_Settings::L_PlayPause_Button_OnClick()
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (SoundManagerLevelRef->GetTrackIsPaused()) // Track was paused before.
	{
		SoundManagerLevelRef->UnpauseCurrentTrack();
		ChangePlayPauseImage(false, true);
		if (IsValid(SoundManagerMainMenuRef) && SoundManagerMainMenuRef->IsAnyTrackPlaying())
		{
			SoundManagerMainMenuRef->PauseCurrentTrack();
			ChangePlayPauseImage(true, false);
		}
	}
	else if (!SoundManagerLevelRef->IsAnyTrackPlaying()) // Track was stopped before.
	{
		SoundManagerLevelRef->StartPlayingCurrentTrack();
		ChangePlayPauseImage(false, true);
		if (IsValid(SoundManagerMainMenuRef) && SoundManagerMainMenuRef->IsAnyTrackPlaying())
		{
			SoundManagerMainMenuRef->PauseCurrentTrack();
			ChangePlayPauseImage(true, false);
		}
	}
	else // Track is playing.
	{
		SoundManagerLevelRef->PauseCurrentTrack();
		ChangePlayPauseImage(false, false);
	}
}

void UWCPP_Settings::L_UpdateTrackNameTextBlock() const
{
	switch (SoundManagerLevelRef->GetCurrentTrackNumber())
	{
	case 1:
		TB_LevelTrackName->SetText(Music_CreativeMinds_Inscription);
		break;
	case 2:
		TB_LevelTrackName->SetText(Music_Elevate_Inscription);
		break;
	case 3:
		TB_LevelTrackName->SetText(Music_GroovyHipHop_Inscription);
		break;
	case 4:
		TB_LevelTrackName->SetText(Music_Punky_Inscription);
		break;
	case 5:
		TB_LevelTrackName->SetText(Music_Rumble_Inscription);
		break;
	default:
		break;
	}
}

void UWCPP_Settings::Music_CreativeMinds_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (bNewState)
	{
		SoundManagerLevelRef->AddTrackToActivePlaylist(1);
	}
	else
	{
		SoundManagerLevelRef->RemoveTrackFromActivePlaylist(1);
	}
}

void UWCPP_Settings::Music_Elevate_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (bNewState)
	{
		SoundManagerLevelRef->AddTrackToActivePlaylist(2);
	}
	else
	{
		SoundManagerLevelRef->RemoveTrackFromActivePlaylist(2);
	}
}

void UWCPP_Settings::Music_GroovyHipHop_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (bNewState)
	{
		SoundManagerLevelRef->AddTrackToActivePlaylist(3);
	}
	else
	{
		SoundManagerLevelRef->RemoveTrackFromActivePlaylist(3);
	}
}

void UWCPP_Settings::Music_Punky_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (bNewState)
	{
		SoundManagerLevelRef->AddTrackToActivePlaylist(4);
	}
	else
	{
		SoundManagerLevelRef->RemoveTrackFromActivePlaylist(4);
	}
}

void UWCPP_Settings::Music_Rumble_OnCheckStateChanged(const bool bNewState)
{
	if (!IsValid(SoundManagerLevelRef))
		return;

	if (bNewState)
	{
		SoundManagerLevelRef->AddTrackToActivePlaylist(5);
	}
	else
	{
		SoundManagerLevelRef->RemoveTrackFromActivePlaylist(5);
	}
}

void UWCPP_Settings::SetCurrentResolutionToTextBlock()
{
	if (TB_CurrentScreenResolution)
	{
		TB_CurrentScreenResolution->SetText(FText::FromString(
			FString::FromInt(CurrentScreenResolution.X) +
			TEXT(" x ") +
			FString::FromInt(CurrentScreenResolution.Y)));
	}
}

void UWCPP_Settings::ApplyChangesButtonOnClick()
{
	switch (ResolutionWidgetSwitcher->GetActiveWidgetIndex())
	{
	case 1:
		GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		break;
	case 2:
		GameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		break;
	case 3:
		GameUserSettings->SetScreenResolution(FIntPoint(2560, 1440));
		break;
	case 4:
		GameUserSettings->SetScreenResolution(FIntPoint(4096, 2160));
		break;
	default:
		break;
	}

	GameUserSettings->SetTextureQuality(TextureQualityWidgetSwitcher->GetActiveWidgetIndex());
	GameUserSettings->SetShadowQuality(ShadowsQualityWidgetSwitcher->GetActiveWidgetIndex());

	GameUserSettings->ApplySettings(false);
}

void UWCPP_Settings::ResolutionLeftButtonOnClick()
{
	TurnWidgetSwitcherToLeft(ResolutionWidgetSwitcher);
}

void UWCPP_Settings::ResolutionRightButtonOnClick()
{
	TurnWidgetSwitcherToRight(ResolutionWidgetSwitcher);
}

void UWCPP_Settings::TextureQualityLeftButtonOnClick()
{
	TurnWidgetSwitcherToLeft(TextureQualityWidgetSwitcher);
}

void UWCPP_Settings::TextureQualityRightButtonOnClick()
{
	TurnWidgetSwitcherToRight(TextureQualityWidgetSwitcher);
}

void UWCPP_Settings::ShadowsQualityLeftButtonOnClick()
{
	TurnWidgetSwitcherToLeft(ShadowsQualityWidgetSwitcher);
}

void UWCPP_Settings::ShadowsQualityRightButtonOnClick()
{
	TurnWidgetSwitcherToRight(ShadowsQualityWidgetSwitcher);
}

void UWCPP_Settings::TurnWidgetSwitcherToLeft(UWidgetSwitcher* WidgetSwitcher)
{
	if (const int32 CurrentIndex = WidgetSwitcher->GetActiveWidgetIndex();
		CurrentIndex == 0)
	{
		WidgetSwitcher->SetActiveWidgetIndex(WidgetSwitcher->GetNumWidgets() - 1);
	}
	else
	{
		WidgetSwitcher->SetActiveWidgetIndex(CurrentIndex - 1);
	}
}

void UWCPP_Settings::TurnWidgetSwitcherToRight(UWidgetSwitcher* WidgetSwitcher)
{
	if (const int32 CurrentIndex = WidgetSwitcher->GetActiveWidgetIndex();
		CurrentIndex == WidgetSwitcher->GetNumWidgets() - 1)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		WidgetSwitcher->SetActiveWidgetIndex(CurrentIndex + 1);
	}
}

void UWCPP_Settings::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (!bIsWidgetActive)
		return;
	
	if (PressedKey == EKeys::Gamepad_LeftShoulder || PressedKey == EKeys::Left) // LB
	{
		LeftArrowOnClick();
	}
	else if (PressedKey == EKeys::Gamepad_RightShoulder || PressedKey == EKeys::Right) // RB
	{
		RightArrowOnClick();
	}
}

void UWCPP_Settings::PrepareWidgetForWorkingWithGamepad()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(1);

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_Settings::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (!bIsWidgetActive)
		return;
	
	if (IsValid(PlayerControllerRef))
	{
		SFX_Volume_Slider->SetUserFocus(PlayerControllerRef);
	}
	else
	{
		SFX_Volume_Slider->SetKeyboardFocus();
	}
}

void UWCPP_Settings::PrepareWidgetForWorkingWithKeyboard()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(0);

	Super::PrepareWidgetForWorkingWithKeyboard();
}

void UWCPP_Settings::GamepadHorizontalScrollCalled(const bool bIsRightStickScroll, const float Rate) const
{
	if (!bIsWidgetActive)
		return;
	
	bool bSFXSliderActive = false;
	bool bMusicSliderActive = false;

	if (IsValid(PlayerControllerRef))
	{
		if (SFX_Volume_Slider->HasUserFocus(PlayerControllerRef))
		{
			bSFXSliderActive = true;
		}
		else if (Music_Volume_Slider->HasUserFocus(PlayerControllerRef))
		{
			bMusicSliderActive = true;
		}
	}
	else
	{
		if (SFX_Volume_Slider->HasKeyboardFocus())
		{
			bSFXSliderActive = true;
		}
		else if (Music_Volume_Slider->HasKeyboardFocus())
		{
			bMusicSliderActive = true;
		}
	}

	if (bSFXSliderActive)
	{
		const float CurrentValue = SFX_Volume_Slider->GetValue();

		if (const float NewValue = CurrentValue + Rate * 0.02f;
			NewValue < 0.0f)
		{
			SFX_Volume_Slider->SetValue(0.0f);
		}
		else if (NewValue >= SFX_Volume_Slider->GetMaxValue())
		{
			SFX_Volume_Slider->SetValue(SFX_Volume_Slider->GetMaxValue());
		}
		else
		{
			SFX_Volume_Slider->SetValue(NewValue);
		}
	}
	else if (bMusicSliderActive)
	{
		const float CurrentValue = Music_Volume_Slider->GetValue();

		if (const float NewValue = CurrentValue + Rate * 0.02f;
			NewValue < 0.0f)
		{
			Music_Volume_Slider->SetValue(0.0f);
		}
		else if (NewValue >= Music_Volume_Slider->GetMaxValue())
		{
			Music_Volume_Slider->SetValue(Music_Volume_Slider->GetMaxValue());
		}
		else
		{
			Music_Volume_Slider->SetValue(NewValue);
		}
	}
}
