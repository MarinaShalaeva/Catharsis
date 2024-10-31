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
class UDataTable;

#include "WCPP_Pause.generated.h"

/** Parent widget class for the pause menu. */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_Pause : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Pause(const FObjectInitializer& ObjectInitializer);

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

protected:
	/**
	 * Data Table with the list of soft references to all
	 * widgets' classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	UDataTable* WidgetBlueprintsDataTable;

	//===================Widgets instances=========================
private:
	/** Reference to the current Settings widget blueprint. */
	UPROPERTY()
	UWCPP_Settings* Settings_Widget;

	//=====================Inscriptions============================
protected:
	/**
	 * Text block for the caption at the top of the widget,
	 * containing information about current opened panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_UpInscription;

	/** Variable containing the pause menu inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText PauseMenuInscription;

	/** Variable containing the settings inscription. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inscriptions")
	FText SettingsInscription;

	//======================UI Elements============================

	/**
	 * Widget switcher containing all necessary for the pause
	 * menu panels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsWidgetSwitcher;

	/** Text block for showing the player's username. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_UserName;

	/**
	 * Button for unpausing the game and for returning from
	 * pause menu to the level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackToGameButton;

	/** Button for switching to Settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	/** Button for opening the main menu panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoToMainMenuButton;

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

	/** Size box for Settings panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SettingsSizeBox;

	//=======================Functions=============================

	/**
	 * Function intended to unpause the game and to return
	 * from pause menu to the level.
	 * Function is called after clicking on Back To Game
	 * Button.
	 */
	UFUNCTION()
	void BackToGameButtonOnClick();

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

	/** Function for opening the main menu. */
	UFUNCTION()
	void GoToMainMenuButtonOnClick();

	/**
	 * Function intended to exit the game.
	 * Function is called after clicking on Exit Button.
	 */
	UFUNCTION()
	void ExitButtonOnClick();

	/** Function for calling the online session end process. */
	UFUNCTION()
	void EndOnlineSession() const;

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
