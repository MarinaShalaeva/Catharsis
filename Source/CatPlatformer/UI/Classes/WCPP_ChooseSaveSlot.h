// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class APlayerController;
class UCanvasPanel;
class UWidgetSwitcher;
class UButton;
class UTextBlock;
class UVerticalBox;
class UImage;

#include "WCPP_ChooseSaveSlot.generated.h"

DECLARE_DELEGATE(FShouldOpenMainMenu)

/**
 * Widget class that contains UI elements for choosing
 * save slot.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_ChooseSaveSlot : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	//================Overriden functions==========================

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to
	 * finalize UObject creation.
	 */
	UWCPP_ChooseSaveSlot(const FObjectInitializer& ObjectInitializer);

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

	//================General UI elements==========================
protected:
	/**
	 * Canvas panel that contains UI elements for choosing
	 * local players number and split screen type.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ChooseSaveSlot_CanvasPanel;

	/** Normal button's color. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Button Style")
	FLinearColor ButtonNormalBackgroundColor;
	/** Color or button with chosen by user save slot. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Button Style")
	FLinearColor ButtonChosenBackgroundColor;

	/** Number of chosen save slot. */
	uint8 ChosenSaveSlot;
	/** Boolean array that shows existence of save slots. */
	UPROPERTY()
	TArray<bool> SaveSlotsExistence;

	/** Inscription for the empty save slot. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inscriptions")
	FText EmptySlotInscription;

	/** Button for choosing the 1st save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseSaveSlot1_Button;
	/** Text block for the username from the 1st save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_PlayerName1;
	/**
	 * Vertical box containing date and time of the 1st save
	 * slot's creation.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox_DateCreation_Slot1;
	/** Text block for the date of the 1st save slot's creation. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CreationDate_Slot1;
	/** Text block for the time of the 1st save slot's creation. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CreationTime_Slot1;

	/** Button for choosing the 2nd save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseSaveSlot2_Button;
	/** Text block for the username from the 2nd save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_PlayerName2;
	/**
	 * Vertical box containing date and time of the 1st save
	 * slot's creation.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox_DateCreation_Slot2;
	/** Text block for the date of the 1st save slot's creation. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CreationDate_Slot2;
	/** Text block for the time of the 1st save slot's creation. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CreationTime_Slot2;

	/** Button for choosing the 3rd save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseSaveSlot3_Button;
	/** Text block for the username from the 3rd save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_PlayerName3;
	/**
	 * Vertical box containing date and time of the 1st save
	 * slot's creation.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* VerticalBox_DateCreation_Slot3;
	/** Text block for the date of the 1st save slot's creation. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CreationDate_Slot3;
	/** Text block for the time of the 1st save slot's creation. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CreationTime_Slot3;

	/**
	 * Widget switcher for buttons located on the bottom
	 * panel. Is needed for changing images for gamepad
	 * and keyboard & mouse modes.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* BottomPanel_WidgetSwitcher;

	/** Flag indicating if bottom panel is visible and enable. */
	bool bBottomPanelIsVisible;

	/** Button for loading chosen save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LoadSaveButton;
	/**
	 * Button for indicating that chosen save slot should
	 * be used for storing data.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChooseCurrentEmptySaveSlotButton;
	/** Button for deleting current save slot. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* DeleteSaveButton;

	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	/**
	 * Image with X XBOX key for showing another way to
	 * load the game slot (by pressing X key on gamepad).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* X_Key_LoadSlot_Image;
	/**
	 * Image with B XBOX key for showing another way to
	 * delete the game slot (by pressing B key on gamepad).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* B_Key_DeleteSlot_Image;
	/**
	 * Image with X XBOX key for showing another way to
	 * choose the save slot for creating the new game (by
	 * pressing X key on gamepad).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* X_Key_ChooseSlot_Image;
	/**
	 * Image with Y XBOX key for showing another way to
	 * exit game (by pressing Y key on gamepad).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Y_Key_ExitButton_Image;

	/**
	 * Inscription for notifying that it is useful to read
	 * the game's rules.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText ReadRulesBeforePlayingInscription;

	/**
	 * Function for changing bottom panel's visibility and
	 * enabling.
	 */
	void ChangeBottomPanelVisibility(const bool bShouldBeVisible);

	/** Function for choosing the 1st save slot. */
	UFUNCTION()
	void ChooseSaveSlot1_ButtonOnClick();
	/** Function for choosing the 2nd save slot. */
	UFUNCTION()
	void ChooseSaveSlot2_ButtonOnClick();
	/** Function for choosing the 3rd save slot. */
	UFUNCTION()
	void ChooseSaveSlot3_ButtonOnClick();

	/**
	 * Timer for checking the gamepad's focus on the save
	 * slot buttons.
	 */
	FTimerHandle TH_CheckGamepadFocus;

	/**
	 * Function for checking the gamepad's focus on the save
	 * slot buttons.
	 */
	UFUNCTION()
	void CheckGamepadFocus();

	//================Gamepad & keyboard===========================
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
	 * Function for setting the certain background color to
	 * the button.
	 * @param Button Reference to button.
	 * @param bNormalColor If true, function will set normal
	 * background color; otherwise function will set color
	 * for chosen save slot.
	 */
	UFUNCTION()
	void ChangeButtonColor(UButton* Button, const bool bNormalColor) const;
	
	/**
	 * Function for creating a new save file if the slot is
	 * empty and for loading the save slot if it exists.
	 */
	UFUNCTION()
	void CreateOrLoadSlot();

	/** Function for deleting the existing save slot. */
	UFUNCTION()
	void DeleteSlotInfo();

public:
	/**
	 * Delegate for notifying HUD class that main menu widget
	 * should be opened.
	 */
	FShouldOpenMainMenu ShouldOpenMainMenuDelegate;

private:
	/**
	 * Function for destroying current widget and for opening
	 * the main menu one.
	 */
	UFUNCTION()
	void OpenMainMenu();

	/**
	 * Function intended to exit the game.
	 * Function is called after clicking on Exit Button.
	 */
	UFUNCTION()
	void ExitButtonOnClick();
};
