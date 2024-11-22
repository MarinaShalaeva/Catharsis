// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Slate/SlateBrushAsset.h"
class USlateBrushAsset;

#include "Engine/DataTable.h"
#ifndef TABLEROWS_H
#define TABLEROWS_H
#include "CatPlatformer/AssetPointer/Classes/TableRows.h"
#endif
class UDataTable;

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

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;

#ifndef WCPP_CONTAINER_H
#define WCPP_CONTAINER_H
#include "CatPlatformer/UI/Classes/WCPP_Container.h"
#endif
class UWCPP_Container;

#ifndef WCPP_CHOOSESAVESLOT_H
#define WCPP_CHOOSESAVESLOT_H
#include "CatPlatformer/UI/Classes/WCPP_ChooseSaveSlot.h"
#endif
class UWCPP_ChooseSaveSlot;

#ifndef WCPP_MAINMENU_H
#define WCPP_MAINMENU_H
#include "CatPlatformer/UI/Classes/WCPP_MainMenu.h"
#endif
class UWCPP_MainMenu;

#ifndef WCPP_LEVEL_H
#define WCPP_LEVEL_H
#include "CatPlatformer/UI/Classes/WCPP_Level.h"
#endif
class UWCPP_Level;

#ifndef WCPP_ENDLEVEL_H
#define WCPP_ENDLEVEL_H
#include "CatPlatformer/UI/Classes/WCPP_EndLevel.h"
#endif
class UWCPP_EndLevel;

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticLibrary.h"
#endif

#include "CPP_HUD.generated.h"

/**
 * Heads-up Display for working with widgets.
 */
UCLASS()
class CATPLATFORMER_API ACPP_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_HUD();

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

	/**
	 * Function that stores logic that should be applied when
	 * the new character is possessed to the player controller.
	 * @param NewCharacter Possessed Character.
	 */
	UFUNCTION(Client, Reliable)
	void NewCharacterWasPossessed(ACPP_Character* NewCharacter);
	void NewCharacterWasPossessed_Implementation(ACPP_Character* NewCharacter);

	/**
	 * Function that stores logic that should be applied when
	 * the current player state was changed to the new one.
	 * @param NewPlayerState Reference to the new player
	 * state.
	 */
	void PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState);

private:
	/**
	 * Weak pointer to the instance of UCPP_GameInstance
	 * class.
	 */
	TWeakObjectPtr<UCPP_GameInstance> GameInstanceRef;

	/**
	 * Weak pointer to the instance of ACPP_PlayerController
	 * class.
	 */
	TWeakObjectPtr<ACPP_PlayerController> PlayerControllerRef;

	/**
	 * Weak pointer to the instance of ACPP_PlayerState
	 * class.
	 */
	TWeakObjectPtr<ACPP_PlayerState> PlayerStateRef;

	/**
	 * Delegate handle for storing the response on Level Was
	 * Ended Delegate from Player State class.
	 */
	FDelegateHandle DH_LevelWasEnded;
	/**
	 * Delegate handle for storing the response on Destroy
	 * Loading Screen Delegate from Player State class.
	 */
	FDelegateHandle DH_DestroyLoadingScreen;

protected:
	/**
	 * Data Table with the list of soft references to all
	 * widgets' classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	TSoftObjectPtr<UDataTable> WidgetBlueprintsDataTable;

	/**
	 * Data Table with the list of soft references to all
	 * sound manager child classes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
	TSoftObjectPtr<UDataTable> SoundManagersDataTable;

public:
	/** 
	 * Weak pointer to the current Container widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_Container> Container_Widget;

	/**
	 * Weak pointer to the current Choose Save Slot widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_ChooseSaveSlot> ChooseSaveSlot_Widget;

	/**
	 * Weak pointer to the current Main Menu widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_MainMenu> MainMenu_Widget;

	/** Weak pointer to the current Pause widget blueprint. */
	TWeakObjectPtr<UWCPP_WidgetParent> Pause_Widget;

	/**
	 * Weak pointer to the current Loading Screen widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_WidgetParent> LoadingScreen_Widget;

	/** Weak pointer to the current Level widget blueprint. */
	TWeakObjectPtr<UWCPP_Level> Level_Widget;

	/** 
	 * Weak pointer to the current End Level widget
	 * blueprint.
	 */
	TWeakObjectPtr<UWCPP_EndLevel> EndLevel_Widget;

	/**
	 * Inscription for notifying that all gamepads were
	 * disconnected.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText AllGamepadsWereDisconnectedInscription;
	/**
	 * Inscription for notifying that one gamepad was
	 * disconnected.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText GamepadWasDisconnectedInscription;
	/**
	 * Inscription for notifying that one gamepad was
	 * connected.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText GamepadWasConnectedInscription;

	/**
	 * Delegate handle for storing the response on
	 * IPlatformInputDeviceMapper::Get().GetOnInput
	 * DeviceConnectionChange() event.
	 */
	FDelegateHandle DH_ControllerConnectionChanging;

	/**
	 * Function for storing the reaction to a change in the
	 * connection of the gamepad to the device.
	 */
	UFUNCTION()
	void OnControllerConnectionChanged(EInputDeviceConnectionState NewConnectionState,
	                                   FPlatformUserId PlatformUserId,
	                                   FInputDeviceId InputDeviceId);

	/**
	 * Function for creating and adding to the player's
	 * screen new notification.
	 * @param NewText Text of the new notification.
	 * @param TimeToDisplay How many seconds should this
	 * notification be shown on the screen.
	 */
	UFUNCTION()
	void CreateNewNotification(const FText& NewText, const float TimeToDisplay) const;

	/**
	 * Function fot creating or destroying the pause widget
	 * depending on new pause mode.
	 * @param bIsPaused New pause mode.
	 */
	UFUNCTION()
	void PauseModeChanged(const bool bIsPaused);

	/**
	 * Function for updating widget data related to the
	 * current buff. Function will be called after the
	 * player has collected the buff.
	 * @param BuffTypeId Identification number of the
	 * collected buff's type.
	 * @param BuffImage Image that represent collected buff.
	 * @param EffectDuration The number of seconds during
	 * which the buff is active.
	 */
	void BuffWasCollected(const int32 BuffTypeId, USlateBrushAsset* BuffImage,
	                      const float EffectDuration) const;

	/**
	 * Function for updating widget data related to a buff
	 * that has expired. Function will be called after the
	 * buff expires.
	 * @param BuffTypeId Identification number of the
	 * collected buff's type.
	 */
	void ShouldResetBuffEffect(const int32 BuffTypeId) const;

	/** Function for clearing all buffs' widgets. */
	void ResetAllBuffsEffects() const;

	/** Initializing of the Container widget. */
	void InitializeContainerWidget();

	/** Destroying the Container widget. */
	void DestroyContainerWidget();

	/** Initializing of the Main Menu widget. */
	UFUNCTION()
	void InitializeMainMenuWidget();

	/** Destroying the Main Menu widget. */
	void DestroyMainMenuWidget();

	/** Initializing of the Choose Save Slot widget. */
	UFUNCTION()
	void InitializeChooseSaveSlotWidget();

	/** Destroying the Choose Save Slot widget. */
	void DestroyChooseSaveSlotWidget();

	/** Initializing of the Pause widget. */
	void InitializePauseWidget();

	/** Destroying the Pause widget. */
	UFUNCTION()
	void DestroyPauseWidget();

	/** Initializing of the Loading Screen widget. */
	UFUNCTION()
	void InitializeLoadingScreenWidget();

	/**
	 * Timer handle for destroying the loading screen on
	 * clients.
	 */
	FTimerHandle TH_DestroyLoadingScreen;

	/** Destroying the Loading Screen widget. */
	UFUNCTION()
	void DestroyLoadingScreenWidget(const bool bShouldHideBackgroundImage);

	/** Initializing of the Level widget. */
	UFUNCTION()
	void InitializeLevelWidget();

	/** Destroying the Level widget. */
	UFUNCTION()
	void DestroyLevelWidget();

	/** Function for replying on ending the level. */
	UFUNCTION()
	void LevelWasEnded(bool bIsWinner);

	/** Initializing of the End Level widget. */
	void InitializeEndLevelWidget(const bool bIsWinner);

	/** Destroying the End Level widget. */
	void DestroyEndLevelWidget();

	/**
	 * Function for creating Pause Widget and for calling
	 * the Player Controller's «Start Pause» function.
	 */
	UFUNCTION()
	void StartPause();

	/**
	 * Function for destroying Pause Widget and for calling
	 * the Player Controller's «End Pause» function.
	 */
	UFUNCTION()
	void EndPause();
};
