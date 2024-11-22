// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;

class UCanvasPanel;
class UCheckBox;
class UTextBlock;
class UWidgetSwitcher;
class UButton;
class UScrollBox;
class UEditableText;
class UHorizontalBox;
class UComboBoxString;
class USizeBox;
class UImage;
class UDataTable;
class FOnlineSessionSearch;
enum class EPlayingMode : uint8;

#ifndef WCPP_VIRTUALKEYBOARD_H
#define WCPP_VIRTUALKEYBOARD_H
#include "CatPlatformer/UI/Classes/WCPP_VirtualKeyboard.h"
#endif
class UWCPP_VirtualKeyboard;

#include "WCPP_ConfigureLevelParams.generated.h"

/**
 * Widget class that contains UI elements for choosing
 * level settings before starting the new level.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_ConfigureLevelParams : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	//================Overriden functions==========================

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to
	 * finalize UObject creation.
	 */
	UWCPP_ConfigureLevelParams(const FObjectInitializer& ObjectInitializer);

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
	TSoftObjectPtr<UDataTable> WidgetBlueprintsDataTable;

	//================General UI elements==========================

	/**
	 * Widget switcher for containing panels with different
	 * level's settings.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsSwitcher;

	/** Number of panels in Widget Switcher. */
	int32 PanelsNumber;

	/**
	 * Widget switcher for buttons located on the bottom
	 * panel (Open Previous Panel Button, Open Previous
	 * Panel Button). Is needed for changing images for
	 * gamepad and keyboard & mouse modes.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* BottomPanel_WidgetSwitcher;

	/**
	 * Button for opening the previous panel in the general
	 * panels switcher.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenPreviousPanelButton;
	/**
	 * Image for LB gamepad's button, which is needed to open
	 * the previous panel in the general panels switcher.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LB_Image;

	/**
	 * Button for opening the next panel in the general
	 * panels switcher.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenNextPanelButton;
	/**
	 * Image for RB gamepad's button, which is needed to open
	 * the next panel in the general panels switcher.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RB_Image;

	//===========Functions for general UI elements=================
private:
	/** Function for opening the next panel. */
	UFUNCTION()
	void RightArrowOnClick();

	/** Function for opening the previous panel. */
	UFUNCTION()
	void LeftArrowOnClick();


	//=========First panel (EnterPlayerName_CanvasPanel)=========
protected:
	/**
	 * Canvas panel for creating and save new player's
	 * nickname.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* EnterPlayerName_CanvasPanel;

	/** Text block for entering the player's name. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableText* InputName_EditableText;

	/**
	 * Horizontal box for storing inscription about opening
	 * or closing virtual keyboard on entering name panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* OnOffVirtualKeyboard_EnterName_HorizontalBox;

	/** Size box for storing virtual keyboard. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* VirtualKeyboardSizeBox;

	/**
	 * Text block for inscription about opening or closing
	 * virtual keyboard on entering name panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* OnOffVirtualKeyboard_EnterName_TB;

	/** Inscription for opening virtual keyboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inscriptions")
	FText OnVirtualKeyboardInscription;

	/** Inscription for closing virtual keyboard. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inscriptions")
	FText OffVirtualKeyboardInscription;

	/** Reference to the virtual keyboard. */
	UPROPERTY()
	UWCPP_VirtualKeyboard* VirtualKeyboardRef;

	/**
	 * Delegate handle for storing reply on Type Symbol
	 * event.
	 */
	FDelegateHandle DH_TypeSymbol;
	/**
	 * Delegate handle for storing reply on Remove Symbol
	 * event.
	 */
	FDelegateHandle DH_RemoveSymbol;
	/**
	 * Delegate handle for storing reply on Enter Was Pressed
	 * event.
	 */
	FDelegateHandle DH_EnterWasPressed;
	/**
	 * Delegate handle for storing reply on Closing Keyboard
	 * event.
	 */
	FDelegateHandle DH_ClosingKeyboard;

	/**
	 * Inscription for notifying about nickname entering
	 * rules.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText EnterNicknameRulesInscription;

	//============Functions for the first panel====================
private:
	/** Function for creating the virtual keyboard. */
	UFUNCTION()
	void InitVirtualKeyboard();

	/** Function for destroying the virtual keyboard. */
	UFUNCTION()
	void DestroyVirtualKeyboard();

	/**
	 * Function for replying on typing any symbol using
	 * virtual keyboard.
	 */
	void SymbolWasTypedByVirtualKeyboard(const FText& Symbol);

	/**
	 * Function for replying on removing any symbol using
	 * virtual keyboard.
	 */
	void SymbolWasRemovedByVirtualKeyboard();

	/**
	 * Function for replying on pressing Enter key using
	 * virtual keyboard.
	 */
	void EnterWasPressedByVirtualKeyboard() const;

	/**
	 * Function for checking player's name correctness.
	 * @param Str String to check.
	 * @return Is name correct?
	 */
	bool CheckNameCorrectness(FString Str);

protected:
	/**
	 * Blueprint function for changing the picture that
	 * indicates if current typed name is correct.
	 * @param bIsCorrect Is current name correct?
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeNameCorrectnessImage(const bool bIsCorrect);

private:
	/**
	 * Function that is called when the value in Input Name
	 * Editable Text field changes.
	 * @param NewText Current text in InputName_EditableText
	 * field.
	 */
	UFUNCTION()
	void InputNameChanged(const FText& NewText);

	/**
	 * Function that is called when the user commits changes
	 * in InputName_EditableText field.
	 * @param NewText Current text in InputName_EditableText
	 * field.
	 * @param CommitMethod How commit was performed.
	 */
	UFUNCTION()
	void InputNameCommitted(const FText& NewText, ETextCommit::Type CommitMethod);


	//========Second panel (ChoosePlayingMode_CanvasPanel)=========
protected:
	/**
	 * Canvas panel that contains UI elements for choosing
	 * playing mode (single play, local multiplayer game,
	 * online multiplayer game).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ChoosePlayingMode_CanvasPanel;

	/** Chosen by checkboxes playing mode. */
	EPlayingMode CurrentPlayingMode;

	/** Check box for the single play mode. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SingleMode_CheckBox;

	/** Check box for the local multiplayer mode. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* LocalMultiplayerMode_CheckBox;

	/** Check box for the online multiplayer mode. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* OnlineMultiplayerMode_CheckBox;

	//===========Functions for the second panel====================

	/**
	 * Function for changing enabling of local multiplayer
	 * check box.
	 * @param bShouldBeEnabled Should we give the option to
	 * choose a local multiplayer mode?
	 */
	void ChangeLocalMultiplayerCheckBoxEnabling(const bool bShouldBeEnabled);

	/**
	 * Function for replying on changing of the
	 * SingleMode_CheckBox's state.
	 */
	UFUNCTION()
	void SingleModeOnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * LocalMultiplayerMode_CheckBox's state.
	 */
	UFUNCTION()
	void LocalMultiplayerModeOnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * OnlineMultiplayerMode_CheckBox's state.
	 */
	UFUNCTION()
	void OnlineMultiplayerModeOnCheckStateChanged(const bool bNewState);


	//=========Third panel (ChooseLocalPlayers_CanvasPanel)========

	/**
	 * Canvas panel that contains UI elements for choosing
	 * local players number and split screen type.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ChooseLocalPlayers_CanvasPanel;

	/**
	 * Widget switcher for choosing local players number and
	 * splitscreen type.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* LocalPlayers_WidgetSwitcher;

	/**
	 * Check box for choosing the first player's input device
	 * (gamepad or keyboard & mouse).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* UseKeyboardForFirstPlayer_CheckBox;

	/**
	 * Widget switcher for buttons intended for changing
	 * local players number. Widget switcher is needed for
	 * changing images for gamepad and keyboard modes.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* ChangeLocalPlayersNumber_WidgetSwitcher;

	/**
	 * Button to decrease the number of local players by
	 * one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* DecrementLocalPlayersNumberButton;

	/**
	 * Button to increase the number of local players by
	 * one.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* IncrementLocalPlayersNumberButton;

	/**
	 * Type of splitscreen:
	 * None = 0;
	 * 2 player horizontal split = 1;
	 * 2 player vertical split = 2;
	 * 3 Player split with 1 player on top and 2 on bottom = 3;
	 * 3 Player split with 1 player on bottom and 2 on top = 4;
	 * 3 Player vertical split = 5;
	 * 3 Player horizontal split = 6;
	 * 4 Player grid split = 7;
	 * 4 Player vertical split = 8;
	 * 4 Player horizontal split = 9.
	 */
	int32 SplitscreenType;

	/** Check box for choosing the split screen type equal to 1. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType1_CheckBox;
	/** Check box for choosing the split screen type equal to 2. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType2_CheckBox;
	/** Check box for choosing the split screen type equal to 3. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType3_CheckBox;
	/** Check box for choosing the split screen type equal to 4. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType4_CheckBox;
	/** Check box for choosing the split screen type equal to 5. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType5_CheckBox;
	/** Check box for choosing the split screen type equal to 6. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType6_CheckBox;
	/** Check box for choosing the split screen type equal to 7. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType7_CheckBox;
	/** Check box for choosing the split screen type equal to 8. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType8_CheckBox;
	/** Check box for choosing the split screen type equal to 9. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SplitType9_CheckBox;

	/** Chosen by user number of players. */
	int32 NumberOfPlayers;

	/**
	 * Inscription for notifying that all gamepads were
	 * disconnected.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText AllGamepadsWereDisconnectedInscription;

	/**
	 * Flag indicating if the first local player should use
	 * keyboard (true) or gamepad (false) in local multiplayer
	 * playing mode.
	 */
	bool bShouldFirstLocalPlayerUseKeyboard;

	//===========Functions for the third panel=====================

	/**
	 * Function for decreasing current local player's
	 * number by one.
	 */
	UFUNCTION()
	void DecrementLocalPlayersNumberButtonOnClick();

	/**
	 * Function for increasing current local player's
	 * number by one.
	 */
	UFUNCTION()
	void IncrementLocalPlayersNumberButtonOnClick();

	/**
	 * Function for updating SplitscreenType variable
	 * depending on current players number and checked
	 * on the panel splitscreen type.
	 * @param CurrentPanelIndex Index of current panel
	 * from ChangeLocalPlayersNumber_WidgetSwitcher.
	 */
	void UpdateCurrentSplitscreenType(const int32 CurrentPanelIndex);

	/**
	 * Function for replying on changing of the
	 * UseKeyboardForFirstPlayer_CheckBox's state.
	 */
	UFUNCTION()
	void UseKeyboardForFirstPlayerOnCheckStateChanged(const bool bNewState);

	/**
	 * Function for replying on changing of the
	 * SplitType1_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType1_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType2_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType2_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType3_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType3_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType4_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType4_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType5_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType5_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType6_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType6_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType7_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType7_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType8_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType8_CheckBox_OnStateChanged(const bool bNewState);
	/**
	 * Function for replying on changing of the
	 * SplitType9_CheckBox's state.
	 */
	UFUNCTION()
	void SplitType9_CheckBox_OnStateChanged(const bool bNewState);


	//===Fourth panel (ChooseOnlineMultiplayerPage_CanvasPanel)====

	/**
	 * Canvas panel that contains UI elements for choosing
	 * needed multiplayer mode (create new session, find
	 * public sessions, connect to private session).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ChooseOnlineMultiplayerPage_CanvasPanel;

	/** Button for opening PublicSessions_CanvasPanel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenPublicSessionsPanelButton;

	/** Button for opening PrivateSession_CanvasPanel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenPrivateSessionPanelButton;

	/** Button for opening CreateSession_CanvasPanel. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenCreateSessionPanelButton;

	//==========Functions for the fourth panel=====================

	/** Function for opening PublicSessions_CanvasPanel. */
	UFUNCTION()
	void OpenPublicSessionsPanelButtonOnClick();

	/** Function for opening PrivateSession_CanvasPanel. */
	UFUNCTION()
	void OpenPrivateSessionPanelButtonOnClick();

	/** Function for opening CreateSession_CanvasPanel. */
	UFUNCTION()
	void OpenCreateSessionPanelButtonOnClick();


	//==========Fifth panel (PublicSessions_CanvasPanel)===========

	/**
	 * Canvas panel that contains UI elements for finding
	 * available public online sessions and for connecting
	 * to one of them.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PublicSessions_CanvasPanel;

	/** Button for finding available public sessions. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* FindPublicSessionsButton;

	/** Scroll box for containing available public sessions. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* PublicSessionsScrollBox;

	/**
	 * Check box for changing LAN connection property for
	 * searching the public sessions.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* FindSessionsOnLAN_CheckBox;

	/**
	 * Size box for spawning a throbber, which intended for
	 * showing the active process of searching online sessions.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* FindingSessionsThrobberSizeBox;

	/** Button for connecting to chosen public session. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConnectToPublicSessionButton;

	/**
	 * Horizontal box containing inscription and a throbber
	 * size box for joining to the public online session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* JoinToPublicSessionHorizontalBox;

	/**
	 * Size box for spawning a throbber, which intended for
	 * showing the active process of joining to the chosen
	 * online session. 
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* JoinToPublicSessionThrobberSizeBox;

	/**
	 * Button for canceling the process of finding the public
	 * online sessions.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CancelFindingPublicSessionsButton;

	/**
	 * Image with X XBOX key for showing another way to
	 * connect to public session (by pressing X key on
	 * gamepad).
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* X_Key_ConnectToPublicSession_Image;

	/**
	 * Text Block for displaying the name of the session
	 * selected by the player to connect.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SessionNameToJoin_TB;

	/**
	 * Delegate handle for storing the response on
	 * FindingSessionsCompletedDelegate event from
	 * Game Instance.
	 */
	FDelegateHandle DH_FindingSessionsCompleted;

	/**
	 * The serial number of the found public online session
	 * selected by the user to connect at the moment.
	 */
	int32 ChosenPublicSessionNumber;

	/**
	 * The ID of the user who initiated the creation of the
	 * public online session selected by the user to connect
	 * at the moment.
	 */
	FUniqueNetIdRepl ChosenPublicSessionUserId;

	/**
	 * The name of the public online session selected by the
	 * user to connect at the moment.
	 */
	FName ChosenPublicSessionName;

	/**
	 * Variable indicating that client is trying to join the
	 * public online session.
	 */
	bool bIsJoiningToPublicServer;

	/**
	 * The FOnlineSessionSearchResult structure, referring
	 * to the public online session selected by the user to
	 * connect at the moment.
	 */
	FOnlineSessionSearchResult ChosenPublicSessionSearchResult;

	/**
	 * Inscription for notifying that finding public sessions
	 * was canceled.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText CancelFindingPublicSessionsInscription;

	/**
	 * Inscription for notifying that joining to public session
	 * was unsuccessful.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText JoiningToPublicSessionFailedInscription;

	/**
	 * Inscription for notifying that user tries to connect
	 * to session while he didn't choose any.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText NoAvailableSessionToConnectInscription;

	/**
	 * Inscription for notifying that there aren't any
	 * sessions to connect.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText NoPublicSessionsFoundInscription;

	/**
	 * Timer handle for clearing created for finding public
	 * sessions UI elements.
	 */
	FTimerHandle TH_ClearFindingPublicSessionUnusedWidgets;

	/**
	 * Timer handle for clearing created for joining to
	 * public session UI elements.
	 */
	FTimerHandle TH_ClearJoiningToPublicSessionUnusedWidgets;

	//===========Functions for the fifth panel=====================

	/**
	 * Function for starting searching for the public online
	 * sessions.
	 */
	UFUNCTION()
	void FindPublicSessionsButtonOnClick();

	/**
	 * Function for canceling the process of finding the
	 * public online sessions.
	 */
	UFUNCTION()
	void CancelFindingPublicSessionsButtonOnClick();

	/**
	 * Function that should be called after completing the
	 * searching for the public online sessions.
	 * @param FoundSessions Information about valid found
	 * sessions.
	 */
	void FindingSessionsCompleted(const TSharedPtr<FOnlineSessionSearch>& FoundSessions);

	/**
	 * Function is needed to clear created for finding public
	 * sessions UI elements.
	 * Function should be called after the TH Clear Finding
	 * Public Session Unused Widgets timer handle ending.
	 */
	UFUNCTION()
	void ClearFindingPublicSessionUnusedWidgets();

	/**
	 * Function for getting information about one public
	 * online session from the instance of WCPP_BuffSlot. 
	 */
	void GetOneSessionInfo(const int32 SessionNumber, const FUniqueNetIdRepl& UserId,
	                       const FName& SessionName, const FOnlineSessionSearchResult& SearchResult);

	/**
	 * Function for starting connection to some public
	 * session.
	 */
	UFUNCTION()
	void ConnectToPublicSessionButtonOnClick();

	/**
	 * Function that should be called if joining to the
	 * online session of any type (public/private) failed.
	 */
	void JoiningToSessionFailed();

	/**
	 * Function that should be called if joining to the
	 * public online session failed.
	 */
	UFUNCTION()
	void JoiningToPublicSessionFailed();

	/**
	 * Function is needed to clear created for joining to
	 * public session UI elements.
	 * Function should be called after the TH Clear Joining
	 * To Public Session Unused Widgets timer handle ending.
	 */
	void ClearJoiningToPublicSessionUnusedWidgets();


	//==========Sixth panel (PrivateSession_CanvasPanel)===========

	/**
	 * Canvas panel that contains UI elements for connecting
	 * to the private session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PrivateSession_CanvasPanel;

	/**
	 * Check box for changing LAN connection property for
	 * searching the private session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* FindPrivateSessionOnLAN_CheckBox;

	/** Text block for entering private session's code. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableText* InputCode_EditableText;

	/** Button for connecting to private session. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConnectToPrivateSessionButton;

	/**
	 * Button for canceling the process of finding the
	 * private online session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CancelFindingPrivateSessionButton;

	/**
	 * Horizontal box containing inscription and a throbber
	 * size box for joining to the private online session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* JoinToPrivateSessionHorizontalBox;

	/**
	 * Size box for spawning a throbber, which intended for
	 * showing the active process of joining to the chosen
	 * online session. 
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* JoinToPrivateSessionThrobberSizeBox;

	/**
	 * Horizontal box for storing inscription about opening
	 * or closing virtual keyboard on finding private session
	 * panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* OnOffVirtualKeyboard_PrivateSession_HorizontalBox;

	/**
	 * Text block for inscription about opening or closing
	 * virtual keyboard on finding private session panel.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* OnOffVirtualKeyboard_PrivateSession_TB;

	/**
	 * Variable indicating that client is trying to join the
	 * private online session.
	 */
	bool bIsJoiningToPrivateSession;

	/**
	 * Delegate handle for storing the response on Finding
	 * Private Session Failed Delegate event from Game
	 * Instance.
	 */
	FDelegateHandle DH_FindingPrivateSessionFailed;

	/**
	 * Inscription for notifying that finding private sessions
	 * was canceled.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText CancelFindingPrivateSessionInscription;

	/**
	 * Inscription for notifying that process of finding private
	 * session failed.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText FindingPrivateSessionFailedInscription;

	/**
	 * Inscription for notifying that joining to private session
	 * was unsuccessful.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText JoiningToPrivateSessionFailedInscription;

	/**
	 * Timer handle for clearing created for finding private
	 * session and joining to it UI elements.
	 */
	FTimerHandle TH_FindingAndJoiningToPrivateSessionUnusedWidgets;

	//===========Functions for the sixth panel=====================

	/**
	 * Function for starting connection to the private
	 * (friend's) online session.
	 */
	UFUNCTION()
	void ConnectToPrivateSessionButtonOnClick();

	/**
	 * Function for canceling the process of finding the
	 * private online session.
	 */
	UFUNCTION()
	void CancelFindingPrivateSessionButtonOnClick();

	/**
	 * Function that should be called if searching for the
	 * online session was unsuccessful.
	 */
	UFUNCTION()
	void OnFindingPrivateSessionFailed();

	/**
	 * Function that should be called if joining to the
	 * private online session failed.
	 */
	UFUNCTION()
	void JoiningToPrivateSessionFailed();

	/**
	 * Function is needed to clear created for finding
	 * private session and joining to it UI elements.
	 * Function should be called after the TH Clear Joining
	 * To Public Session Unused Widgets timer handle ending.
	 */
	void ClearFindingAndJoiningToPrivateSessionUnusedWidgets();

	/**
	 * Function that is called when the user commits changes
	 * in InputCode_EditableText field.
	 * @param NewText Current text in InputCode_EditableText
	 * field.
	 * @param CommitMethod How commit was performed.
	 */
	UFUNCTION()
	void InputCodeCommitted(const FText& NewText, ETextCommit::Type CommitMethod);


	//=========Seventh panel (CreateSession_CanvasPanel)===========

	/**
	 * Canvas panel that contains UI elements for creating
	 * new online session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CreateSession_CanvasPanel;

	/**
	 * Combo Box for choosing maximum players' number on the
	 * server being created.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* MaxPlayersNumOnServer_ComboBox;

	/**
	 * Check box for changing the publicity of the server
	 * being created.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* SessionShouldBePublic_CheckBox;

	/**
	 * Check box for changing LAN connection property for
	 * searching the public sessions.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* CreateSessionForLANConnectionOnly_CheckBox;

	/**
	 * Inscription for notifying that session's creation was
	 * unsuccessful.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText SessionCreationFailedInscription;

	//==========Functions for the seventh panel====================

	/**
	 * Function for calling the new session's creation.
	 * @param LevelNumber Number of level to generate.
	 */
	UFUNCTION()
	void CallServerCreation(const int32 LevelNumber) const;

	/** Function for replying on session's creation failing. */
	UFUNCTION()
	void SessionCreationFailed() const;


	//============Eighth panel (ChooseLevel_CanvasPanel)===========

	/**
	 * Canvas panel that contains UI elements for choosing
	 * level.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ChooseLevel_CanvasPanel;

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
	 * The number of the level that the player wants to
	 * start.
	 */
	int32 ChosenLevelNumber;

	/**
	 * Inscription for notifying that number of connected
	 * devices is lower than chosen number of players.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText SmallConnectedDevicesAmountInscription;

	/**
	 * An inscription indicating that the level the player
	 * is trying to start is not available.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Notification Inscriptions")
	FText LevelIsUnavailableInscription;

	//===========Functions for the eighth panel====================

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


	//====================BP Functions=============================

	/**
	 * Blueprint function for changing the picture of a closed
	 * lock, related to the level with the number LevelNumber,
	 * to the image with an opened lock.
	 * @param LevelNumber Number of the level to open. Random
	 * level has the 6th number.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLevelImageToOpened(const int32 LevelNumber);

	/**
	 * Function for changing images showing the input device
	 * selected for the Player 1.
	 * @param bChangeToKeyboard If true, should change to
	 * images with keyboard. If false, should change to
	 * images with gamepad.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeInputDevicesImages(const bool bChangeToKeyboard);

	//===================C++ Functions=============================

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
	 * Delegate handle for storing the response on moving
	 * gamepad's stick event.
	 */
	FDelegateHandle DH_GamepadScroll;

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
	 * Function for scrolling vertical panels.
	 * @param bIsRightStickScroll Is scrolling done using
	 * the right joystick?
	 * @param Rate Scrolling scale [-1.0f; 1.0].
	 */
	UFUNCTION()
	void GamepadScrollCalled(const bool bIsRightStickScroll, const float Rate) const;

	/**
	 * Function for decrement number of local players by
	 * gamepad's LT button.
	 */
	UFUNCTION()
	void LT_Pressed();

	/**
	 * Function for increment number of local players by
	 * gamepad's RT button.
	 */
	UFUNCTION()
	void RT_Pressed();

	/**
	 * Function for checking if level is available.
	 * @param LevelNumber Level number to check.
	 * @return Is the level available?
	 */
	bool CheckLevelAvailability(const int32 LevelNumber) const;

	/**
	 * Function for checking if the player can start the
	 * level.
	 * @param LevelNumber Level number player wants to start.
	 * @return Can the player start this level?
	 */
	UFUNCTION()
	bool CanStartGame(const int32 LevelNumber);

	/**
	 * Function to start the new level.
	 * @param LevelNumber Level number player wants to start.
	 */
	UFUNCTION()
	void StartGame(const int32 LevelNumber);
};
