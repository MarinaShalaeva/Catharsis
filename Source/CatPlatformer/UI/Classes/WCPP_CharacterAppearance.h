// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UCheckBox;

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif

#include "WCPP_CharacterAppearance.generated.h"

/**
 * Parent widget class for changing and saving the character's
 * appearance.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_CharacterAppearance : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_CharacterAppearance(const FObjectInitializer& ObjectInitializer);

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
	 * Reference to the instance of ACPP_PlayerState class
	 * associated with current widget.
	 */
	UPROPERTY()
	ACPP_PlayerState* PlayerStateRef;

	/**
	 * Delegate handle for storing the response on possessing
	 * the new instance on the ACPP_Character class.
	 */	
	FDelegateHandle DH_CharacterWasPossessed;
	
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
	
protected:	
	/** Check box for applying the gray cat's color. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* GrayColor_CheckBox;

	/** Check box for applying the orange cat's color. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* OrangeColor_CheckBox;

	/** Check box for applying the black cat's color. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* BlackColor_CheckBox;

	/** The cat's color index. */
	UPROPERTY(BlueprintReadOnly)
	uint8 CatColorIndex;

private:
	/**
	 * Function for replying on changing of the
	 * GrayColor_CheckBox's state.
	 */
	UFUNCTION()
	void GrayColorOnCheckStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * OrangeColor_CheckBox's state.
	 */
	UFUNCTION()
	void OrangeColorOnCheckStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * BlackColor_CheckBox's state.
	 */
	UFUNCTION()
	void BlackColorOnCheckStateChanged(const bool bNewState);

protected:
	/**
	 * Function for updating the UI character's color and
	 * the appropriate variable in the Player State class.
	 * @param ColorIndex Index of color to set.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void ChangeCatsColor(const int32 ColorIndex);
	void ChangeCatsColor_Implementation(const int32 ColorIndex);

private:
	/**
	 * Function for setting focus on necessary UI element
	 * for correct working with this widget by gamepad.
	 */
	virtual void SetFocusForGamepadMode() override;
};
