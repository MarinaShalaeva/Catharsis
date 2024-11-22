// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class USlateBrushAsset;
class UCanvasPanel;
class UVerticalBox;
class UTextBlock;
class USizeBox;
class UButton;
class UWidgetSwitcher;

#ifndef WCPP_BUFFSLOT_H
#define WCPP_BUFFSLOT_H
#include "CatPlatformer/UI/Classes/WCPP_BuffSlot.h"
#endif
class UWCPP_BuffSlot;

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif
class ACPP_PlayerState;

#include "WCPP_Level.generated.h"

/**
 * Widget for displaying information about current level and
 * the current character's state.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_Level : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Level(const FObjectInitializer& ObjectInitializer);

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
	 * Weak pointer to the instance of ACPP_PlayerState class
	 * associated with current widget.
	 */
	TWeakObjectPtr<ACPP_PlayerState> PlayerStateRef;

	/** Weak pointer to the instance of ACPP_GameState class. */
	TWeakObjectPtr<ACPP_GameState> GameStateRef;

	/**
	 * Delegate handle for storing the response on Players
	 * Number Changed Delegate from Game State class.
	 */
	FDelegateHandle DH_PlayersNumberChanged;

public:
	/**
	 * Flag indicating whether the player needs to use the
	 * mouse cursor.
	 */
	bool bCursorShouldBeVisible;

protected:
	//===================Widgets instances=========================

	/** General canvas panel for storing other widgets. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* LevelCanvasPanel;

	/**
	 * Widget Switcher for switching between panels with UI
	 * elements for gamepad and keyboard & mouse modes.
	 * Elements are needed for indicating that the player is
	 * ready to start the game.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* ReadyForGame_WidgetSwitcher;

	/**
	 * Button for indicating that the player is ready to
	 * start the game.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ReadyForGameButton;

	/**
	 * Button for indicating that the host player doesn't
	 * want to wait for other players' joining.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* NotWaitingForOtherPlayersButton;

	/**
	 * Vertical box containing information for the host
	 * player about his online «room».
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* RoomNumberVerticalBox;

	/** Text block containing current online «room» number. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_RoomNumber;

	/**
	 * Widget Switcher for switching between panels with UI
	 * elements for gamepad and keyboard & mouse modes.
	 * Elements are needed for working with information about
	 * the online «room».
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* CopyRoomNumber_WidgetSwitcher;

	/** Button for copying current online «room» number. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CopyRoomNumberButton;

	/**
	 * Text blocks for storing current number of online
	 * players.
	*/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CurrentPlayersNumber_Keyboard;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_CurrentPlayersNumber_Gamepad;

	/** Current player's score. */
	int32 CurrentScore;

	/** Text block for storing current player's score. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Score;

	/**
	 * Text block for storing points that should be added
	 * to the current player's score.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_ScoreToAdd;

	/**
	 * Vertical box for storing current active buffs (array
	 * of UWCPP_BuffSlot widgets).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* BuffsVerticalBox;

	/** Array of references to current buffs' widgets. */
	UPROPERTY(BlueprintReadWrite)
	TArray<UWCPP_BuffSlot*> BuffSlot_Widgets;

	/**
	 * Array of size boxes containing current buff's widgets.
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<USizeBox*> RelatedToBuffsSizeBoxes;

	/**
	 * Text block for the countdown before the start of the
	 * level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_TimerBeforeGameStart;

	/**
	 * Inscription that should be shown after the ending of
	 * countdown.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText StartLevelInscription;

	/**
	 * Inscription for notifying that process of copying
	 * the online room number was successful.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText RoomNumberWasCopiedInscription;
	/**
	 * Inscription for notifying that game cannot be started
	 * with only one player on server.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText OnlyOnePlayerOnServerInscription;

public:
	/**
	 * Function for updating information about existing buff
	 * widget or for creating a new one if it doesn't exist.
	 * @param PC Player controller that owns the widget.
	 * @param BuffTypeId Buff's type ID to create or update.
	 * @param BuffImage Buff's image to set.
	 * @param BuffEffectDuration The duration of buff's
	 * effect.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void CreateOrUpdateBuffWidget(APlayerController* PC,
	                              const int32 BuffTypeId,
	                              USlateBrushAsset* BuffImage,
	                              const float BuffEffectDuration);

	/**
	 * Function for destroying buff's widget.
	 * @param BuffTypeId Buff's type ID to destroy.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyBuffWidget(const int32 BuffTypeId);

	/** Function for destroying all existing buffs' widgets. */
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyAllBuffsWidgets();

protected:
	/** Function for starting animation of countdown digit. */
	UFUNCTION(BlueprintImplementableEvent)
	void CallChangingDigitSizeAnimation();

	/**
	 * Function for starting animation of adding or removing
	 * score.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void CallChangingScoreAnimation();

private:
	/**
	 * Function that stores logic that should be applied when
	 * the new character is possessed to the player controller.
	 * @param NewCharacter Possessed Character.
	 */
	void NewCharacterWasPossessed(ACPP_Character* NewCharacter);

	/**
	 * Function that stores logic that should be applied when
	 * the current player state was changed to the new one.
	 * @param NewPlayerState Reference to the new player
	 * state.
	 */
	void PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState);

	/**
	 * Function for changing score text block's value.
	 * @param ScoreToAdd Score to add (or remove). 
	 */
	void ChangeScore(const int32 ScoreToAdd);

	/**
	 * Function for updating score text block's value up to
	 * CurrentScore variable after the Changing Score Anim
	 * is ended.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCurrentScoreToTextBlock();

	/**
	 * Function that is called when the user presses the
	 * Enter key on the keyboard or A key in the gamepad.
	 */
	UFUNCTION()
	void StartingKeyWasPressed();

	/**
	 * Function for notifying other classes that the player
	 * is ready to start game.
	 */
	UFUNCTION()
	void ReadyForGameButtonOnClick();

protected:
	/**
	 * Function for changing information up to new players
	 * number.
	 * @param NewPlayersNumber New number of players on the
	 * server.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayersNumberWasChanged(const int32 NewPlayersNumber);
	void PlayersNumberWasChanged_Implementation(const int32 NewPlayersNumber);

private:
	/**
	 * Function for indicating that the host player doesn't
	 * want to wait for other players' joining.
	 */
	UFUNCTION()
	void NotWaitingForOtherPlayersButtonOnClick();

	/** Function for copying current online «room» number. */
	UFUNCTION()
	void CopyRoomNumberButtonOnClick();
	
	/**
	 * Delegate handle for storing the response on Score
	 * Changed Delegate from Player State class.
	 */
	FDelegateHandle DH_ScoreChanged;

	/**
	 * Delegate handle for storing the response on Should
	 * Begin Countdown To Start Level Delegate from Player
	 * State class.
	 */
	FDelegateHandle DH_ShouldBeginCountdownToStartLevel;

	/**
	 * Timer handle for switching to the next digit in
	 * countdown before the start of the level.
	 */
	FTimerHandle TH_ChangingDigitSize;

	/** Contains the current digit of countdown. */
	int8 TimerBeforeGameStartCounter;

	/**
	 * Function for starting countdown before the beginning
	 * of the level.
	 */
	UFUNCTION()
	void BeginCountdownToStartLevel();

	/** Function for decreasing the current digit of countdown. */
	UFUNCTION()
	void DecreaseTimerBeforeGameStartCounter();

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
