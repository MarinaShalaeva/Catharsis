// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;

#ifndef WCPP_SETTINGS_H
#define WCPP_SETTINGS_H
#include "CatPlatformer/UI/Classes/WCPP_Settings.h"
#endif
class UWCPP_Settings;
class UTextBlock;
class UWidgetSwitcher;
class USizeBox;
class UHorizontalBox;
class UImage;
class UDataTable;

#include "WCPP_MainMenu.generated.h"

DECLARE_DELEGATE(FShouldOpenSaveSlotsPanel)

/**
 * Parent widget class for the main menu panel.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_MainMenu : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_MainMenu(const FObjectInitializer& ObjectInitializer);

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

	/**
	 * Timer handle for closing last opened session in
	 * several seconds after loading the main menu.
	 */
	FTimerHandle TH_CloseLastOpenedSession;

	/** Current username. */
	FText UserName;

protected:
	/**
	 * Data Table with the list of soft references to all
	 * widgets' classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	TSoftObjectPtr<UDataTable> WidgetBlueprintsDataTable;

	//========================Login================================

	/**
	 * Horizontal box containing UI elements for the
	 * player's login to the Epic Games account.
	 */
	UPROPERTY()
	UHorizontalBox* LoginToEOS_HorizontalBox;

	/** Button for starting the login to the EOS account. */
	UPROPERTY()
	UButton* LoginToEOS_Button;

	/**
	 * Text block for showing current user's name from EOS
	 * account.
	 */
	UPROPERTY()
	UTextBlock* TB_EOS_AccountName;

	/**
	 * Image for the XBOX key that should be pressed to start
	 * the logging in process.
	 */
	UPROPERTY()
	UImage* Gamepad_LoginKey_Image;

	/** Function for starting the login to the EOS account. */
	UFUNCTION()
	void LoginToEOS_Button_OnClick();

	/**
	 * Function for replying on ending the login to the EOS
	 * account.
	 */
	UFUNCTION()
	void LoginToEOS_WasEnded(const bool bSuccess, const FString& AccountName) const;

	//===================Widgets instances=========================

	/**
	 * Weak pointer to the current Configure Level Params
	 * widget blueprint.
	 */
	TWeakObjectPtr<UWCPP_WidgetParent> ConfigureLevelParams_Widget;

	/**
	 * Weak pointer to the current Character Appearance
	 * widget blueprint.
	 */
	TWeakObjectPtr<UWCPP_WidgetParent> CharacterAppearance_Widget;

	/**
	 * Weak pointer to the current Rules widget blueprint.
	 */
	TWeakObjectPtr<UWCPP_WidgetParent> Rules_Widget;

	/** 
	 * Weak pointer to the current Settings widget blueprint.
	 */
	TWeakObjectPtr<UWCPP_Settings> Settings_Widget;

	/** 
	 * Weak pointer to the current Statistics widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_WidgetParent> Statistics_Widget;

	/** 
	 * Weak pointer to the current Game Info widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_WidgetParent> GameInfo_Widget;

	//=====================Inscriptions============================

	/**
	 * Text block for the caption at the top of the widget,
	 * containing information about current opened panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_UpInscription;

	/** Variable containing the main menu inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText MainMenuInscription;

	/** Variable containing the level's settings inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText ConfigureLevelParamsInscription;

	/**
	 * Variable containing the character appearance
	 * inscription.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText CharacterAppearanceInscription;

	/** Variable containing the game rules inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText RulesInscription;

	/** Variable containing the settings inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText SettingsInscription;

	/** Variable containing the statistics inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText StatisticsInscription;

	/**
	 * Variable containing the information about the game
	 * inscription.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText GameInfoInscription;

	//======================UI Elements============================

	/**
	 * Widget switcher containing all necessary for the main
	 * menu panels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsWidgetSwitcher;

	/** Text block for showing the player's username. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_UserName;

	/** Button for opening Configure Level Params widget. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenConfigureLevelParamsButton;

	/** Button for switching to Rules panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RulesButton;

	/** Button for switching to Character Appearance panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CharacterAppearanceButton;

	/** Button for switching to Settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	/** Button for switching to Statistics panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StatisticsButton;

	/**
	 * Button for switching to the panel with information
	 * about the game.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InfoButton;

	/** Button for opening the save slot panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeSaveSlotButton;

	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	/**
	 * Button for returning to the main panel, containing
	 * the list of available buttons.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoBackButton;

	/**
	 * Widget Switcher with UI elements for gamepad and
	 * keyboard & mouse to switch between different menu
	 * panels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* BottomPanel_WidgetSwitcher;

	//=======================Size boxes============================

	/**  Size box for choosing level's settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* ConfigureLevelParamsSizeBox;

	/** Size box for Rules panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* RulesSizeBox;

	/** Size box for Character Appearance panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* CharacterAppearanceSizeBox;

	/** Size box for Settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SettingsSizeBox;

	/** Size box for Statistics panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* StatisticsSizeBox;

	/** Size box for information about the game panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* GameInfoSizeBox;

	//=======================Functions=============================
private:
	/**
	 * Function for changing current opened panel to the
	 * level's Settings panel.
	 */
	UFUNCTION()
	void SwitchToConfigureLevelParamsPanel();

	/** Function for creating the level's Settings panel. */
	UFUNCTION()
	void InitConfigureLevelParamsWidget();

	/** Function for destroying the level's Settings panel. */
	UFUNCTION()
	void DestroyConfigureLevelParamsWidget();

	/**
	 * Function for changing current opened panel to the
	 * Rules panel.
	 */
	UFUNCTION()
	void SwitchToRulesPanel();

	/** Function for creating the game's Rules panel. */
	UFUNCTION()
	void InitRulesWidget();

	/** Function for destroying the game's Rules panel. */
	UFUNCTION()
	void DestroyRulesWidget();

	/**
	 * Function for changing current opened panel to the
	 * Character Appearance panel.
	 */
	UFUNCTION()
	void SwitchToCharacterAppearancePanel();

	/**
	 * Function for creating the game's Character Appearance
	 * panel.
	 */
	UFUNCTION()
	void InitCharacterAppearanceWidget();

	/**
	 * Function for destroying the game's Character
	 * Appearance panel.
	 */
	UFUNCTION()
	void DestroyCharacterAppearanceWidget();

	/**
	 * Function for changing current opened panel to the
	 * settings panel.
	 */
	UFUNCTION()
	void SwitchToSettingsPanel();

	/** Function for creating the settings panel. */
	UFUNCTION()
	void InitSettingsWidget();

	/** Function for destroying the settings panel. */
	UFUNCTION()
	void DestroySettingsWidget();

	/**
	 * Function for changing current opened panel to the
	 * Statistics panel.
	 */
	UFUNCTION()
	void SwitchToStatisticsPanel();

	/** Function for creating the game's Statistics panel. */
	UFUNCTION()
	void InitStatisticsWidget();

	/** Function for destroying the game's Statistics panel. */
	UFUNCTION()
	void DestroyStatisticsWidget();

	/**
	 * Function for changing current opened panel to the
	 * information about the game panel.
	 */
	UFUNCTION()
	void SwitchToInfoPanel();

	/** Function for creating the game's information panel. */
	UFUNCTION()
	void InitInfoWidget();

	/** Function for destroying the game's information panel. */
	UFUNCTION()
	void DestroyInfoWidget();

	/**
	 * Function for changing UI elements enabling and
	 * visibility depend on which type of panel was opened.
	 * @param bCurrentPanelIsMain Is opened panel is the
	 * main one?
	 */
	void PanelWasSwitched(const bool bCurrentPanelIsMain);

public:
	/**
	 * Delegate for notifying HUD class that choosing save
	 * slot widget should be opened.
	 */
	FShouldOpenSaveSlotsPanel ShouldOpenSaveSlotsPanelDelegate;

private:
	/**
	 * Function for opening the save slot panel.
	 * Function is called after clicking on Change Save Slot
	 * Button.
	 */
	UFUNCTION()
	void ChangeSaveSlotButtonOnClick();

	/**
	 * Function intended to exit the game.
	 * Function is called after clicking on Exit Button.
	 */
	UFUNCTION()
	void ExitButtonOnClick();

	/**
	 * Function for returning to the main panel, containing
	 * the list of available buttons.
	 * Function is called after clicking on Go Back Button.
	 */
	UFUNCTION()
	void GoBackButtonOnClick();

	/**
	 * Function changing go back button's visibility and
	 * enabling.
	 * @param bShouldBeVisible Should the go back button be
	 * visible and enabled?
	 */
	UFUNCTION()
	void ChangeGoBackButtonVisibility(const bool bShouldBeVisible) const;

	/**
	 * Function for changing caption at the top of the widget.
	 * @param NewText Inscription to set.
	 */
	UFUNCTION()
	void ChangeUpInscriptionText(const FText& NewText) const;

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
};
