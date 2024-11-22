// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UScrollBox;
class UTextBlock;

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif

#include "WCPP_Statistics.generated.h"

/**
 * Widget class for showing player's results and statistics.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_Statistics : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Statistics(const FObjectInitializer& ObjectInitializer);

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
	 * Weak pointer to the instance of ACPP_PlayerState
	 * class associated with current widget.
	 */
	TWeakObjectPtr<ACPP_PlayerState> PlayerStateRef;

	/**
	 * Delegate handle for storing the response on possessing
	 * the new instance on the ACPP_Character class.
	 */
	FDelegateHandle DH_CharacterWasPossessed;

protected:
	/**
	 * Scroll box with the player's statistics.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox;

	/** Text block for showing the player's name. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Name;
	/**
	 * Text block for showing the number of started by the
	 * player levels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_StartedLevelsNumber;
	/**
	 * Text block for showing the number of ended by the
	 * player levels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_EndedLevelsNumber;
	/**
	 * Text block for showing the number of the player's
	 * deaths.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_DeathsNumber;
	/**
	 * Text block for showing the number of jumps performed
	 * by the player during all levels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_JumpsNumber;
	/**
	 * Text block for showing the number of buffs and debuffs
	 * collected by the player during all levels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BuffsNumber;
	/**
	 * Text block for showing the number of crows killed by
	 * the player during all levels.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_NPCsKilled;
	/**
	 * Text block for showing the number of the player's
	 * wins in online games.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_OnlineWinsNumber;
	/**
	 * Text block for showing the number of the player's
	 * losses in online games.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_OnlineLossesNumber;
	/** Text block for showing the general score number. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_GeneralScore;
	/**
	 * Text block for showing the number of seconds spent
	 * by the player in game.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_TimeInGame;
	/**
	 * Text block for showing the best time for the player
	 * to complete the 1st level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BestTimeLevel_1;
	/**
	 * Text block for showing the best time for the player
	 * to complete the 2nd level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BestTimeLevel_2;
	/**
	 * Text block for showing the best time for the player
	 * to complete the 3rd level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BestTimeLevel_3;
	/**
	 * Text block for showing the best time for the player
	 * to complete the 4th level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BestTimeLevel_4;
	/**
	 * Text block for showing the best time for the player
	 * to complete the 5th level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BestTimeLevel_5;
	/**
	 * Text block for showing the best time for the player
	 * to complete the random level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_BestTimeLevel_6;

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
	 * Function for filling the text blocks with data from
	 * the save file.
	 */
	void UpdateTextBlocks() const;

	/**
	 * Function for updating the TB_TimeInGame text block
	 * value.
	 * @param NewTime New value to set.
	 */
	void UpdateTimeInGameTextBlock(const uint32 NewTime) const;

	/**
	 * Delegate handle for storing the response on moving
	 * gamepad's stick event.
	 */
	FDelegateHandle DH_GamepadScroll;

	/**
	 * Function for scroll the player's statistics by
	 * gamepad's sticks.
	 * @param bIsRightScroll Is scrolling made by right
	 * gamepad's stick?
	 * @param Rate Scrolling rate.
	 */
	void GamepadScroll(const bool bIsRightScroll, const float Rate) const;

	/**
	 * Function for changing current playing mode (gamepad
	 * or keyboard & mouse) by checking the type of the
	 * pressed key.
	 * @param PressedKey Info about the pressed key.
	 */
	virtual void AnyKeyPressed(const FKey& PressedKey) override;
};
