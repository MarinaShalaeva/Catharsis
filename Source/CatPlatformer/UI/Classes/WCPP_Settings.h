// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;

#ifndef CPP_SOUNDMANAGERMAINMENU_H
#define CPP_SOUNDMANAGERMAINMENU_H
#include "CatPlatformer/Sound/Classes/CPP_SoundManagerMainMenu.h"
#endif
class ACPP_SoundManagerMainMenu;

#ifndef CPP_SOUNDMANAGERLEVEL_H
#define CPP_SOUNDMANAGERLEVEL_H
#include "CatPlatformer/Sound/Classes/CPP_SoundManagerLevel.h"
#endif
class ACPP_SoundManagerLevel;

class USlider;
class UWidgetSwitcher;
class UCheckBox;
class UTextBlock;
class UButton;
class UDataTable;
enum class EPlaylistRepeatingMode : uint8;

#include "WCPP_Settings.generated.h"

/**
 * Widget class for changing the game's settings.
 */
UCLASS()
class CATPLATFORMER_API UWCPP_Settings : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Settings(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

	//=====================All Panels==============================
protected:
	/**
	 * Widget switcher for containing panels with different
	 * types of settings.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsSwitcher;

	/** Number of panels in Widget Switcher. */
	int32 PanelsNumber;

	/**
	 * Data Table with the list of soft references to all
	 * sound manager child classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	UDataTable* SoundManagersDataTable;

	/**
	 * Reference to the instance of the child ACPP_SoundManager
	 * class for working with the main menu tracks.
	 */
	UPROPERTY()
	ACPP_SoundManagerMainMenu* SoundManagerMainMenuRef;

	/**
	 * Delegate handle for storing replying on
	 * SoundManagerMainMenuRef's Track Was Switched event.
	 */
	FDelegateHandle DH_MM_TrackWasSwitched;

	/**
	 * Reference to the instance of the child ACPP_SoundManager
	 * class for working with the level tracks.
	 */
	UPROPERTY()
	ACPP_SoundManagerLevel* SoundManagerLevelRef;

	/**
	 * Delegate handle for storing replying on
	 * SoundManagerLevelRef's Track Was Switched event.
	 */
	FDelegateHandle DH_L_TrackWasSwitched;

	/**
	 * Reference to the instance of ACPP_PlayerState
	 * class associated with current Player Controller.
	 */
	UPROPERTY()
	ACPP_PlayerState* PlayerStateRef;

public:
	/**
	 * Flag indicating where this settings panel was created:
	 * in the main menu (true) or in the pause menu, that is
	 * during level (false).
	 */
	bool bIsMainMenu;

	//===================Bottom Panel==============================
protected:
	/**
	 * Widget switcher for buttons located on the bottom
	 * panel (Open Previous Panel Button, Open Previous
	 * Panel Button). Is needed for changing images for
	 * gamepad and keyboard & mouse modes.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* BottomPanel_WidgetSwitcher;

	/** Button for opening the previous panel in the switcher. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenPreviousPanelButton;

	/** Button for opening the next panel in the switcher. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenNextPanelButton;

private:
	/** Function for opening the next panel. */
	UFUNCTION()
	void RightArrowOnClick();

	/** Function for opening the previous panel. */
	UFUNCTION()
	void LeftArrowOnClick();

	//====================Audio Panel==============================

	/**
	 * Function for updating UI elements related to the main
	 * menu tracks up to current Sound Manager's state.
	 */
	UFUNCTION()
	void InitializeMainMenuTracksPanel();

	/**
	 * Function for updating UI elements related to the
	 * level tracks up to current Sound Manager's state.
	 */
	UFUNCTION()
	void InitializeLevelTracksPanel();

	//==================Volumes=======================
protected:
	/** Slider for changing SFX volume. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* SFX_Volume_Slider;

	/** Slider for changing music volume. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* Music_Volume_Slider;

private:
	/**
	 * Function for replying on SFX_Volume_Slider value
	 * changing.
	 */
	UFUNCTION()
	void SFX_Volume_Slider_OnValueChanged(const float Value);

	/**
	 * Function for replying on Music_Volume_Slider value
	 * changing.
	 */
	UFUNCTION()
	void Music_Volume_Slider_OnValueChanged(const float Value);

	//=============Main Menu Tracks===================

	/** Repeating mode for the main menu tracks. */
	EPlaylistRepeatingMode MM_RepeatingMode;

protected:
	/**
	 * Button for changing the main menu playlist's repeating
	 * mode.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MM_Repeat_Button;

	/**
	 * Button for changing the current main menu track to the
	 * previous one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MM_PreviousTrack_Button;

	/**
	 * Button for changing the current main menu track to the
	 * next one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MM_NextTrack_Button;

	/**
	 * Button for stopping and starting the current main menu
	 * track playing.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MM_PlayPause_Button;

	/** Text block for showing current main menu track. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_MainMenuTrackName;

	/**
	 * Check box for adding the track with the name «Echo Of
	 * Sadness» to the main menu playlist (and removing this
	 * track from the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_EchoOfSadness_CheckBox;

	/** Variable that contains «Echo Of Sadness» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_EchoOfSadness_Inscription;

	/**
	 * Check box for adding the track with the name «Memories»
	 * to the main menu playlist (and removing this track from
	 * the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_Memories_CheckBox;

	/** Variable that contains «Memories» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_Memories_Inscription;

	/**
	 * Check box for adding the track with the name
	 * «Tenderness» to the main menu playlist (and removing
	 * this track from the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_Tenderness_CheckBox;

	/** Variable that contains «Tenderness» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_Tenderness_Inscription;

private:
	/**
	 * Function for changing the main menu playlist's
	 * repeating mode.
	 */
	UFUNCTION()
	void MM_Repeat_Button_OnClick();

	/**
	 * Function for changing the current main menu track to
	 * the previous one.
	 */
	UFUNCTION()
	void MM_PreviousTrack_Button_OnClick();

	/**
	 * Function for changing the current main menu track to
	 * the next one.
	 */
	UFUNCTION()
	void MM_NextTrack_Button_OnClick();

	/**
	 * Function for stopping and starting the current track
	 * playing.
	 */
	UFUNCTION()
	void MM_PlayPause_Button_OnClick();

	/**
	 * Function for updating the name of the current main
	 * menu track.
	 */
	UFUNCTION()
	void MM_UpdateTrackNameTextBlock();

	/**
	 * Function for replying on changing of the
	 * Music_EchoOfSadness_CheckBox's state.
	 */
	UFUNCTION()
	void Music_EchoOfSadness_OnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * Music_Memories_CheckBox's state.
	 */
	UFUNCTION()
	void Music_Memories_OnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * Music_Tenderness_CheckBox's state.
	 */
	UFUNCTION()
	void Music_Tenderness_OnCheckStateChanged(const bool bNewState);

	//===============Level Tracks===================

	/** Repeating mode for the level tracks. */
	EPlaylistRepeatingMode L_RepeatingMode;

protected:
	/**
	 * Button for changing the level playlist's repeating
	 * mode.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* L_Repeat_Button;

	/**
	 * Button for changing the current level track to the
	 * previous one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* L_PreviousTrack_Button;

	/**
	 * Button for changing the current level track to the
	 * next one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* L_NextTrack_Button;

	/**
	 * Button for stopping and starting the current level
	 * track playing.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* L_PlayPause_Button;

	/** Text block for showing current level track. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_LevelTrackName;

	/**
	 * Check box for adding the track with the name «Creative
	 * Minds» to the level playlist (and removing this track
	 * from the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_CreativeMinds_CheckBox;

	/** Variable that contains «Creative Minds» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_CreativeMinds_Inscription;

	/**
	 * Check box for adding the track with the name «Elevate»
	 * to the level playlist (and removing this track from
	 * the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_Elevate_CheckBox;

	/** Variable that contains «Elevate» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_Elevate_Inscription;

	/**
	 * Check box for adding the track with the name «Groovy
	 * Hip Hop» to the level playlist (and removing this
	 * track from the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_GroovyHipHop_CheckBox;

	/** Variable that contains «Groovy Hip Hop» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_GroovyHipHop_Inscription;

	/**
	 * Check box for adding the track with the name «Punky»
	 * to the level playlist (and removing this track from
	 * the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_Punky_CheckBox;

	/** Variable that contains «Punky» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_Punky_Inscription;

	/**
	 * Check box for adding the track with the name «Rumble»
	 * to the level playlist (and removing this track from
	 * the playlist).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* Music_Rumble_CheckBox;

	/** Variable that contains «Rumble» track name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "Track Names")
	FText Music_Rumble_Inscription;

private:
	/**
	 * Function for changing the level playlist's repeating
	 * mode.
	 */
	UFUNCTION()
	void L_Repeat_Button_OnClick();

	/**
	 * Function for changing the current level track to the
	 * previous one.
	 */
	UFUNCTION()
	void L_PreviousTrack_Button_OnClick();

	/**
	 * Function for changing the current level track to the
	 * next one.
	 */
	UFUNCTION()
	void L_NextTrack_Button_OnClick();

	/**
	 * Function for stopping and starting the current level
	 * track playing.
	 */
	UFUNCTION()
	void L_PlayPause_Button_OnClick();

	/**
	 * Function for updating the name of the current level
	 * track.
	 */
	UFUNCTION()
	void L_UpdateTrackNameTextBlock() const;

	/**
	 * Function for replying on changing of the
	 * Music_CreativeMinds_CheckBox's state.
	 */
	UFUNCTION()
	void Music_CreativeMinds_OnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * Music_Elevate_CheckBox's state.
	 */
	UFUNCTION()
	void Music_Elevate_OnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * Music_GroovyHipHop_CheckBox's state.
	 */
	UFUNCTION()
	void Music_GroovyHipHop_OnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * Music_Punky_CheckBox's state.
	 */
	UFUNCTION()
	void Music_Punky_OnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * Music_Rumble_CheckBox's state.
	 */
	UFUNCTION()
	void Music_Rumble_OnCheckStateChanged(const bool bNewState);

protected:
	/**
	 * Funtion for changing repeat image up to current
	 * repeating mode.
	 * @param bForMainMenu Should we change image for the
	 * main menu playlist?
	 * @param NewRepeatingMode New repeating mode.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeRepeatImage(const bool bForMainMenu, const EPlaylistRepeatingMode NewRepeatingMode);

	/**
	 * Funtion for changing play/pause image up to current
	 * track state (is it playing at the moment?).
	 * @param bForMainMenu Should we change image for the
	 * main menu track?
	 * @param bShouldSetPauseImage Should we change image to
	 * the pause one?
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ChangePlayPauseImage(const bool bForMainMenu, const bool bShouldSetPauseImage);

	//=================Video Quality Panel=========================

private:
	/** Variable for game user settings object reference. */
	UPROPERTY()
	UGameUserSettings* GameUserSettings;

protected:
	/** Text block for showing current screen resolution. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CurrentScreenResolution;

	/**
	 * Button for applying changes to game user settings
	 * object reference.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ApplyChangesButton;

	/** Button for switching resolution to the left one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResolutionLeftButton;

	/** Widget switcher for choosing screen resolution. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* ResolutionWidgetSwitcher;

	/** Button for switching resolution to the right one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResolutionRightButton;

	/** Button for switching texture quality to the left one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* TextureQualityLeftButton;

	/** Widget switcher for choosing texture quality. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* TextureQualityWidgetSwitcher;

	/** Button for switching texture quality to the right one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* TextureQualityRightButton;

	/** Button for switching shadows quality to the left one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ShadowsQualityLeftButton;

	/** Widget switcher for choosing shadows quality. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* ShadowsQualityWidgetSwitcher;

	/** Button for switching shadows quality to the right one. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ShadowsQualityRightButton;

private:
	/**
	 * Variable that contains current user's screen
	 * resolution.
	 */
	FIntPoint CurrentScreenResolution;

	/**
	 * Function for filling the Current Screen Resolution
	 * Text Block with related value.
	 */
	void SetCurrentResolutionToTextBlock();

	/**
	 * Function for applying changes to game user settings
	 * object reference.
	 */
	UFUNCTION()
	void ApplyChangesButtonOnClick();

	/** Function for switching resolution to the left one. */
	UFUNCTION()
	void ResolutionLeftButtonOnClick();

	/** Function for switching resolution to the right one. */
	UFUNCTION()
	void ResolutionRightButtonOnClick();

	/**
	 * Function for switching texture quality to the left
	 * one.
	 */
	UFUNCTION()
	void TextureQualityLeftButtonOnClick();

	/**
	 * Function for switching texture quality to the right
	 * one.
	 */
	UFUNCTION()
	void TextureQualityRightButtonOnClick();

	/**
	 * Function for switching shadows quality to the left
	 * one.
	 */
	UFUNCTION()
	void ShadowsQualityLeftButtonOnClick();

	/**
	 * Function for switching shadows quality to the right
	 * one.
	 */
	UFUNCTION()
	void ShadowsQualityRightButtonOnClick();

	/** Button for opening the previous panel in the switcher. */
	UFUNCTION()
	void TurnWidgetSwitcherToLeft(UWidgetSwitcher* WidgetSwitcher);

	/** Button for opening the next panel in the switcher. */
	UFUNCTION()
	void TurnWidgetSwitcherToRight(UWidgetSwitcher* WidgetSwitcher);

	//================Gamepad & Keyboard===========================
	/**
	 * Delegate handle for storing the response on horizontal
	 * moving gamepad's stick event.
	 */
	FDelegateHandle DH_GamepadHorizontalScroll;

	/**
	 * Function for changing current playing mode (gamepad
	 * or keyboard & mouse) by checking the type of the
	 * pressed key.
	 * @param PressedKey Info about the pressed key.
	 */
	virtual void AnyKeyPressed(const FKey& PressedKey) override;

	/** Switch all necessary panels to the gamepad mode. */
	virtual void PrepareWidgetForWorkingWithGamepad() override;

	/**
	 * Function for setting focus on necessary UI element
	 * for correct working with this widget by gamepad.
	 */
	virtual void SetFocusForGamepadMode() override;

	/**
	 * Switch all necessary panels to the keyboard & mouse
	 * mode.
	 */
	virtual void PrepareWidgetForWorkingWithKeyboard() override;

	/**
	 * Function for scrolling horizontal panels.
	 * @param bIsRightStickScroll Is scrolling done using
	 * the right joystick?
	 * @param Rate Scrolling scale [-1.0f; 1.0].
	 */
	UFUNCTION()
	void GamepadHorizontalScrollCalled(const bool bIsRightStickScroll, const float Rate) const;
};
