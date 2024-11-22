// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

#include "WCPP_WidgetParent.generated.h"

/**
 * Parent class for all custom C++ widget classes.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_WidgetParent : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_WidgetParent(const FObjectInitializer& ObjectInitializer);

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
	 * Current input device used by player (true is gamepad,
	 * false is keyboard & mouse).
	 */
	bool bIsGamepadMode;

	//=================Objects instances===========================

	/** Reference to the instance of UCPP_GameInstance class. */
	TWeakObjectPtr<UCPP_GameInstance> GameInstanceRef;

	/**
	 * Reference to the instance of ACPP_PlayerController
	 * class associated with current widget.
	 */
	TWeakObjectPtr<ACPP_PlayerController> PlayerControllerRef;

	/**
	 * Delegate handle for storing the response on pressing
	 * any key event.
	 */
	FDelegateHandle DH_InputKeyWasPressed;

public:
	/**
	 * Setter for the GameInstanceRef variable.
	 * @param NewGameInstance New reference to the Game
	 * Instance.
	 */
	void SetGameInstanceRef(UCPP_GameInstance* NewGameInstance);

	/**
	 * Setter for the PlayerControllerRef variable.
	 * @param NewPlayerController New reference to the Player
	 * Controller.
	 */
	void SetPlayerControllerRef(ACPP_PlayerController* NewPlayerController);

	/**
	 * Flag indicating if current widget is opened on the
	 * user's screen.
	 */
	bool bIsWidgetActive;

private:
	/**
	 * Delegate handle for replying on All Gamepads Were
	 * Disconnected event from the Player Controller.
	 */
	FDelegateHandle DH_AllGamepadsWereDisconnected;

protected:
	/**
	 * Function for changing current playing mode (gamepad
	 * or keyboard & mouse) by checking the type of the
	 * pressed key.
	 * @param PressedKey Info about the pressed key.
	 */
	virtual void AnyKeyPressed(const FKey& PressedKey);

	/** Switch all necessary panels to the gamepad mode. */
	UFUNCTION()
	virtual void PrepareWidgetForWorkingWithGamepad();

public:
	/**
	 * Function for setting focus on necessary UI element
	 * for correct working with this widget by gamepad.
	 */
	UFUNCTION()
	virtual void SetFocusForGamepadMode();

protected:
	/**
	 * Switch all necessary panels to the keyboard & mouse
	 * mode.
	 */
	UFUNCTION()
	virtual void PrepareWidgetForWorkingWithKeyboard();
};
