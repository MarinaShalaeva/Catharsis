// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticLibrary.h"
#endif

class ACPP_Character;
class UCPP_GameInstance;
class ACPP_SoundManager;
class UMaterialInstance;

#include "CPP_PlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterWasPossessed, ACPP_Character*);

DECLARE_MULTICAST_DELEGATE_OneParam(FInputKeyWasPressed, const FKey& /* Pressed Key */);

DECLARE_MULTICAST_DELEGATE(FAllGamepadsWereDisconnected);

DECLARE_DELEGATE_OneParam(FPauseChanged, bool /* bIsPaused */);

DECLARE_MULTICAST_DELEGATE_TwoParams(FGamepadScroll,
                                     const bool /* Is Right Scroll? */,
                                     const float /* Scroll Rate */);

DECLARE_DELEGATE_TwoParams(FCreateNotification,
                           const FText& /* Text of the notification */,
                           const float /* Time to display */);

UCLASS()
class CATPLATFORMER_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_PlayerController();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the actor's life (but before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Called to bind functionality to input. */
	virtual void SetupInputComponent() override;

	/**
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Overriden function for cases when this controller is
	 * asked to possess a pawn.
	 * @param InPawn The Pawn to be possessed.
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * Function for broadcasting the character's possessing
	 * event.
	 * @param InCharacter Possessed character reference.
	 */
	UFUNCTION(Client, Reliable)
	void BroadcastCharacterWasPossessedDelegate(ACPP_Character* InCharacter);
	void BroadcastCharacterWasPossessedDelegate_Implementation(ACPP_Character* InCharacter);

	//==================Input Bindings============================

protected:
	/** Mapping Context. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jumping Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Pausing and unpausing Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	/** Sprinting Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Attacking Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Tracking any key pressing Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AnyKeyAction;

	/** Gamepad LB key pressing Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LB_Action;

	/** Gamepad RB key pressing Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RB_Action;

	/** Gamepad LT key pressing Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LT_Action;

	/** Gamepad RT key pressing Input Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RT_Action;

	/** Moving Input 2D-Axis Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Looking Input 2D-Axis Action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/**
	 * Input 1D-Axis Action for gamepad's left stick moving
	 * along the horizontal axis.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftHorizontalScrollAction;

	/**
	 * Input 1D-Axis Action for gamepad's right stick moving
	 * along the horizontal axis.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightHorizontalScrollAction;

	/**
	 * Input 1D-Axis Action for gamepad's left stick moving
	 * along the vertical axis.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftVerticalScrollAction;

	/**
	 * Input 1D-Axis Action for gamepad's right stick moving
	 * along the vertical axis.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightVerticalScrollAction;

private:
	/** Function called for movement input. */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/**
	 * Checking the speed of the current character to update
	 * information about his pose (can he start the waiting
	 * phase or is he moving right now?)
	 */
	UFUNCTION()
	void CheckVelocity();

	/**
	 * Function for applying a waiting animation if the
	 * character is in idle mode for a long period time.
	 */
	UFUNCTION()
	void StartCharacterWaiting();

	/** Function for ending a waiting animation. */
	UFUNCTION()
	void EndCharacterWaiting();

	/** Function called for looking input. */
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	/** 
	 * Called via input to perform horizontal scroll by left
	 * gamepad's stick.
	 */
	void LeftHorizontalScrollByGamepad(const FInputActionValue& Value);

	/** 
	 * Called via input to perform horizontal scroll by right
	 * gamepad's stick.
	 */
	void RightHorizontalScrollByGamepad(const FInputActionValue& Value);

	/** 
	 * Called via input to perform vertical scroll by left
	 * gamepad's stick.
	 */
	void LeftVerticalScrollByGamepad(const FInputActionValue& Value);

	/** 
	 * Called via input to perform vertical scroll by right
	 * gamepad's stick.
	 */
	void RightVerticalScrollByGamepad(const FInputActionValue& Value);

	/** Binding the input of the start jumping. */
	UFUNCTION()
	void StartJump();

	/** Binding the input of the end jumping. */
	UFUNCTION()
	void StopJump();

	/**
	 * Binding the input of the start of the sprint.
	 * Increases the speed of the character.
	 */
	UFUNCTION()
	void StartSprint();

	/**
	 * Binding the input of the end of the sprint.
	 * Returns the starting speed value of the character.
	 */
	UFUNCTION()
	void StopSprint();

	/** Binding of the attack input. */
	UFUNCTION()
	void StartAttack();

	/** Binding of any key input. */
	void AnyKeyPressed(FKey PressedKey);

	/** Binding of the Pause Key input. */
	UFUNCTION()
	void PauseKeyPressed();

	//======================Pause=================================
public:
	/** Can the player pause the game? */
	bool bCanBePaused;

	/** Did the player open the pause menu? */
	bool bIsPaused;

	/** Function for pausing the game. */
	void StartPause();

	/** Function that takes the game off pause. */
	void EndPause();

	//=====================Cursor=================================

	/**
	 * Changing cursor's visibility and switching on (or off)
	 * some mouse events, such as Click or Mouse Over events.
	 * @param bShouldBeVisible Should we make cursor visible
	 * and active or not?
	 */
	UFUNCTION()
	void ChangeCursorVisibility(const bool bShouldBeVisible);

	/** Function for starting loading data from the save file. */
	UFUNCTION()
	void CallLoadingInfoFromSaveFile();

	/**
	 * Function for starting collecting of data from the
	 * to save it to file.
	 */
	UFUNCTION()
	void CallCollectingInfoForSavingItToFile();

	//======================Online================================

private:
	/** Unique Net ID for the online session. */
	UPROPERTY(Replicated)
	FUniqueNetIdRepl PlayerNetId;

public:
	/** Getter for the PlayerNetId variable. */
	FORCEINLINE FUniqueNetIdRepl GetPlayerNetId() const { return PlayerNetId; }

	/** Setter for the PlayerNetId variable. */
	FORCEINLINE void SetPlayerNetId(const FUniqueNetIdRepl NewValue) { PlayerNetId = NewValue; }

	/** 
	 * Handles cleaning up this Player Controller when killing
	 * the connection.
	 * @param Connection the connection associated with this
	 * actor.
	 */
	virtual void OnNetCleanup(UNetConnection* Connection) override;

	/**
	 * Function for calling the destroying of the online
	 * session.
	 * Is only relevant for clients.
	 */
	UFUNCTION(Client, Reliable)
	void ClientDestroyOnlineSession();

private:
	void ClientDestroyOnlineSession_Implementation();

	//====================Variables===============================

	/** Flag indicating if the mouse cursor is visible. */
	bool bCursorIsVisible;

	/**
	 * Flag indicating if the player is using gamepad (false
	 * if using keyboard & mouse).
	 */
	bool bIsGamepadMode;

	/**
	 * Flag indicating if the main menu map is opened at the
	 * moment.
	 */
	bool bIsMainMenu;

public:
	/**
	 * Flag indicating if this player controller owns the
	 * main player. Will be false for local players with
	 * index > 0 in local multiplayer mode; will be true
	 * otherwise.
	 */
	bool bIsFirstPlayer;

	//================Object References===========================
private:
	/** Reference to the instance of ACPP_Character class. */
	UPROPERTY(Replicated)
	ACPP_Character* CharacterRef;

	/** Weak pointer to the instance of UCPP_GameInstance class. */
	TWeakObjectPtr<UCPP_GameInstance> GameInstanceRef;

	/** Reference to the instance of ACPP_SoundManager class. */
	UPROPERTY()
	ACPP_SoundManager* SoundManagerRef;

	//============================================================

	/**
	 * Delegate handle for storing replying on SoundManagerRef's
	 * Track Was Switched event.
	 */
	FDelegateHandle DH_TrackWasSwitched;

public:
	/**
	 * Variable indicating whether we are waiting for the
	 * game to start (we are waiting for other players to
	 * join or not all players have clicked the «Ready»
	 * button).
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bWaitingForReadinessToStartLevel;

	/**
	 * Delegate for notifying about changing of the current
	 * character.
	 */
	FCharacterWasPossessed CharacterWasPossessedDelegate;

	/**
	 * Delegate for notifying that all gamepads were
	 * disconnected.
	 */
	FAllGamepadsWereDisconnected AllGamepadsWereDisconnectedDelegate;

	/**
	 * Delegate for notifying about pressing any mouse,
	 * keyboard or gamepad key. 
	 */
	FInputKeyWasPressed InputKeyWasPressedDelegate;

	/**
	 * Delegate for notifying about vertical gamepad's left
	 * or right scroll.
	 * Parameters:
	 * bool — Is scroll committed by right stick?
	 * float — Scroll Rate.
	 */
	FGamepadScroll GamepadScrollDelegate;

	/**
	 * Delegate for notifying about horizontal gamepad's left
	 * or right scroll.
	 * Parameters:
	 * bool — Is scroll committed by right stick?
	 * float — Scroll Rate.
	 */
	FGamepadScroll GamepadHorizontalScrollDelegate;

	/** Delegate for notifying about pause mode changing. */
	FPauseChanged PauseChangedDelegate;

	/** Delegate for calling the creation of the notification. */
	FCreateNotification CreateNotificationDelegate;

	//============================================================

protected:
	/** Timer for calling the CheckVelocity function. */
	FTimerHandle TH_CheckVelocity;

	/** Is the player idle for a long period of time? */
	bool bCharacterIsIdle;

	/** Timer for applying the waiting pose to the character. */
	FTimerHandle TH_CallWaitingCharacterPose;

	//============================================================

	/**
	 * Function that should be called after switching any
	 * audio track.
	 */
	void TrackWasSwitched();

	//============================================================
public:
	/**
	 * Function for changing possibility of controlling
	 * character.
	 */
	void ChangeInputEnabling(bool bShouldEnable);

	//====================Getters=================================

	/**
	 * Function for getting the highest opened level's number
	 * from the Character.
	 * @return The highest opened level's number (amount of
	 * opened levels).
	 */
	UFUNCTION()
	int32 GetMaxOpenedLevelNumber();

	/**
	 * Function for setting the new player name.
	 * @param NewName The name chosen by the player for
	 * himself.
	 */
	UFUNCTION()
	void SetCustomUserName(const FString& NewName);

	/**
	 * Function for getting the player name from the Player
	 * State.
	 * @return The name chosen by the player for himself.
	 */
	UFUNCTION()
	FString GetCustomUserName() const;
	/**
	 * Function for getting the player's score from the Player
	 * State.
	 * @return Current score.
	 */
	UFUNCTION()
	int32 GetCustomUserScore() const;
	/**
	 * Function for getting the player's deaths number during
	 * current level from the Player State.
	 * @return Current score.
	 */
	UFUNCTION()
	uint32 GetCustomCurrentLevelDeathsNumber() const;
	/**
	 * Function for getting the number of seconds spent by
	 * the player to finish level from the Player State.
	 * @return TimeOnLevel variable from the Player State.
	 */
	UFUNCTION()
	float GetCustomTimeOnLevel() const;
	/**
	 * Function for getting data from the save slot.
	 * @param OutSaveSlot Saved data.
	 * @return Is the save slot valid?
	 */
	bool GetSavedData(FSaveSlot& OutSaveSlot) const;

	/**
	 * Function for changing value of the bIsReadyToStart
	 * variable from the Player State.
	 * @param bIsReady Is player ready to start the level?
	 */
	UFUNCTION()
	void SetPlayerIsReadyToStartGame(const bool bIsReady) const;

	/**
	 * Getter for the CharacterRef variable.
	 * @return Reference to the associated instance of
	 * ACPP_Character class.
	 */
	UFUNCTION()
	FORCEINLINE ACPP_Character* GetCharacterRef() const { return CharacterRef; }

	/**
	 * Setter for the bIsGamepadMode variable.
	 * @param bNewValue True if current mode should be the
	 * gamepad one. False if current mode should be the
	 * keyboard & mouse one.
	 */
	UFUNCTION()
	void SetIsGamepadMode(bool bNewValue);

	/**
	 * Getter for the bIsGamepadMode variable.
	 * @return Flag indicating if the player is using
	 * gamepad.
	 */
	UFUNCTION()
	FORCEINLINE bool GetIsGamepadMode() const { return bIsGamepadMode; }

	/**
	 * Setter for the PlayerControllerRef variable.
	 * @param NewValue New reference to the Sound Manager.
	 */
	void SetSoundManagerRef(ACPP_SoundManager* NewValue);

	/** Getter for the PlayerControllerRef variable. */
	FORCEINLINE ACPP_SoundManager* GetSoundManagerRef() const { return SoundManagerRef; }
};
