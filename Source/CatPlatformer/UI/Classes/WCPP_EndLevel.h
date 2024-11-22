// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UWidgetSwitcher;
class UTextBlock;
class UButton;
class UVerticalBox;
class UScrollBox;

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif
class ACPP_PlayerState;

#include "WCPP_EndLevel.generated.h"

/**
 * Widget for displaying information about level's results
 * after its ending.
 */
UCLASS()
class CATPLATFORMER_API UWCPP_EndLevel : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_EndLevel(const FObjectInitializer& ObjectInitializer);

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
	 * Flag indicating if associated with this widget player
	 * won.
	 */
	bool bIsWinner;

	//=====================Inscriptions============================
protected:
	/** Inscription for the winner. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inscriptions")
	FText YouWinInscription;

	/** Inscription for the loser. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inscriptions")
	FText YouLoseInscription;

	/** Inscription for the default player's name. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inscriptions")
	FText DefaultPlayerNameInscription;

	//======================UI Elements============================

	/** Text block for showing the player's username. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_UserName;

	/**
	 * Text block for showing the player's result (won or
	 * lost).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_WinningOrLosing;

	/** Text block for showing the player's score. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Score;

	/** Text block for showing the player's deaths number. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Deaths;

	/**
	 * Text block for showing number of seconds spent by
	 * the player to finish level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_TimeOnLevel;

	/**
	 * Widget switcher for choosing panel that will be
	 * suitable for the associated with this widget player's
	 * role.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PlayerRole_WidgetSwitcher;

	//===================Main Player Panel=========================

	/**
	 * Scroll box containing UI elements for the main player
	 * (first local player / host / single mode player).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* MainPlayerPanel_ScrollBox;

	/** Button for starting the next level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartNextLevelButton;

	/** Button for opening the main menu panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoToMainMenuButton;

	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitButton;

	/** Button for starting the 1st level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartLevel1_Button;
	/** Button for starting the 2nd level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartLevel2_Button;
	/** Button for starting the 3rd level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartLevel3_Button;
	/** Button for starting the 4th level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartLevel4_Button;
	/** Button for starting the 5th level. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartLevel5_Button;
	/**
	 * Button for starting the random level with higher
	 * difficulty than at the 5th level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartRandomLevel_Button;

	/**
	 * Inscription for notifying that level started its
	 * loading.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText LevelIsLoadingInscription;

	/**
	 * Delegate handle for storing the response on Score
	 * Changed Delegate from Player State class.
	 */
	FDelegateHandle DH_ScoreChanged;

	/**
	 * Weak pointer to the instance of ACPP_PlayerState class
	 * associated with current widget.
	 */
	TWeakObjectPtr<ACPP_PlayerState> PlayerStateRef;

	/**
	 * Function that stores logic that should be applied when
	 * the new character is possessed to the player controller.
	 * @param NewCharacter Possessed Character.
	 */
	UFUNCTION()
	void NewCharacterWasPossessed(ACPP_Character* NewCharacter);
	
	/**
	 * Function that stores logic that should be applied when
	 * the current player state was changed to the new one.
	 * @param NewPlayerState Reference to the new player
	 * state.
	 */
	UFUNCTION()
	void PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState);

	/**
	 * Function for changing score text block's value.
	 * @param ScoreToAdd Score to add (or remove). 
	 */
	void ChangeScore(const int32 ScoreToAdd);
	
	/** Function for starting the next level. */
	UFUNCTION()
	void StartNextLevelButtonOnClick();

	/**
	 * Function for opening the main menu called from the
	 * main player panel.
	 */
	UFUNCTION()
	void MainPlayerGoToMainMenuButtonOnClick();

	/**
	 * Function for exiting the game called from the main
	 * player panel.
	 */
	UFUNCTION()
	void MainPlayerExitButtonOnClick();

	/**
	 * Function for calling the online session end process
	 * from host side.
	 */
	UFUNCTION()
	void EndHostOnlineSession() const;

	/** Function for starting the 1st level. */
	UFUNCTION()
	void StartLevel1_ButtonOnClick();
	/** Function for starting the 2nd level. */
	UFUNCTION()
	void StartLevel2_ButtonOnClick();
	/** Function for starting the 3rd level. */
	UFUNCTION()
	void StartLevel3_ButtonOnClick();
	/** Function for starting the 4th level. */
	UFUNCTION()
	void StartLevel4_ButtonOnClick();
	/** Function for starting the 5th level. */
	UFUNCTION()
	void StartLevel5_ButtonOnClick();
	/**
	 * Function for starting the random level with higher
	 * difficulty than at the 5th level.
	 */
	UFUNCTION()
	void StartRandomLevel_ButtonOnClick();
	/**
	 * Function to start the new level.
	 * @param LevelNumber Level number player wants to start.
	 */
	UFUNCTION()
	void StartGame(const int32 LevelNumber) const;

	//=====================Client Panel============================

	/** Vertical box containing UI elements for the client. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* ClientPanel_VerticalBox;

	/** Button for opening the main menu panel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ClientGoToMainMenuButton;

	/** Button to exit the game. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ClientExitButton;

	/**
	 * Function for opening the main menu panel called from
	 * the client panel.
	 */
	UFUNCTION()
	void ClientGoToMainMenuButtonOnClick();

	/**
	 * Function for exiting the game called from the client
	 * panel.
	 */
	UFUNCTION()
	void ClientExitButtonOnClick();

	/**
	 * Function for calling the online session end process
	 * from client side.
	 */
	UFUNCTION()
	void EndClientOnlineSession() const;

	//==================Local Players Panel========================

	/** Vertical box containing UI elements for the client. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* LocalPlayersPanel_VerticalBox;

private:
	/** Function for opening the main menu panel. */
	UFUNCTION()
	void GoToMainMenu();

	/** Function for exiting the game. */
	UFUNCTION()
	void Exit();

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

public:
	/**
	 * Getter for the bIsWinner variable.
	 * @return bIsWinner variable.
	 */
	FORCEINLINE bool GetIsWinner() const { return bIsWinner; }

	/**
	 * Setter for the bIsWinner variable.
	 * @param bNewValue Value to set to the bIsWinner
	 * variable.
	 */
	FORCEINLINE void SetIsWinner(const bool bNewValue) { bIsWinner = bNewValue; }
};
