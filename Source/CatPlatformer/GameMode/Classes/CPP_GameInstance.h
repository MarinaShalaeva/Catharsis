// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
class FOnlineSessionSearch;

#ifndef CPP_STATICLIBRARY_H
#define CPP_STATICLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticLibrary.h"
#endif
enum class EPlayingMode : uint8;

#ifndef CPP_SAVEMANAGER_H
#define CPP_SAVEMANAGER_H
#include "CatPlatformer/SaveGame/Classes/CPP_SaveManager.h"
#endif
class UCPP_SaveManager;

#include "CPP_GameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FChangeActiveSplitscreenType, const int32 /* NewSplitscreenType */);

DECLARE_DELEGATE(FCloseLastOpenedSession);

DECLARE_DELEGATE(FStartLogin);

DECLARE_DELEGATE_TwoParams(FLoginEnded, const bool /* bSuccess */, const FString& /* AccountName */);

DECLARE_DELEGATE_RetVal(bool /* bIsLoggedIn */, FIsPlayerLoggedIn);

DECLARE_DELEGATE_RetVal_OneParam(bool /* bSuccess */, FGetEOSAccountName, FString& /* OutAccountName */);

DECLARE_DELEGATE_FourParams(FCallSessionCreation,
                            const FString& /* PlayerName */,
                            bool /* bPublicServer */,
                            bool /* bIsLAN */,
                            int32 /* MaxPlayersNumber */);

DECLARE_DELEGATE(FSessionCreationFailed);

DECLARE_DELEGATE_TwoParams(FStartFindingSessions,
                           FUniqueNetIdRepl /* UserId */,
                           bool /* bIsLAN */);

DECLARE_MULTICAST_DELEGATE_OneParam(FFindingSessionsCompleted,
                                    const TSharedPtr<FOnlineSessionSearch>& /* FoundSessions */);

DECLARE_DELEGATE(FCancelFindingSessions);

DECLARE_DELEGATE_RetVal_ThreeParams(bool,
                                    FStartJoiningSession,
                                    FUniqueNetIdRepl /* UserId */,
                                    FName /* SessionName */,
                                    const FOnlineSessionSearchResult& /* SearchResult */);

DECLARE_DELEGATE(FJoiningSessionFailed);

DECLARE_DELEGATE_RetVal_ThreeParams(bool,
                                    FStartFindingPrivateSession,
                                    FUniqueNetIdRepl /* LocalUserId */,
                                    const FString& /* RoomNumber */,
                                    const bool /* bIsLAN */);

DECLARE_DELEGATE(FFindingPrivateSessionFailed);

DECLARE_DELEGATE_RetVal_OneParam(bool, FStartEndingSession, const FName& /* SessionName */);

DECLARE_DELEGATE_RetVal_OneParam(bool, FCallDestroyingSession, const FName& /* SessionName */);

/**
 * C++ parent for the Game Instance class.
 */
UCLASS()
class CATPLATFORMER_API UCPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	UCPP_GameInstance();

protected:
	/**
	 * Function for storing logic that should be applied
	 * during the Game Instance initialization.
	 */
	virtual void Init() override;

	/**
	 * Current Playing Mode.
	 * SinglePlayer = 0;
	 * LocalMultiplayer = 1;
	 * OnlineMultiplayerServer = 2;
	 * OnlineMultiplayerClientInPrivateSession = 3;
	 * OnlineMultiplayerClientInPublicSession = 4.
	 */
	EPlayingMode PlayingMode;

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
	int32 CurrentSplitscreenType;

	/** Maximum number of online players in one session. */
	int32 MaxOnlinePlayersNumber;

	/** Number of current level. */
	int32 LevelNumber;

	/** Number of players (is needed for local multiplayer). */
	uint8 PlayersNumber;

	/** Name of the current online session. */
	FString CurrentSessionName;

	/** Variable containing unique online room number. */
	uint32 RoomNumber;

public:
	/**
	 * Variable for storing information about the gamepad
	 * mode for the first local player during switching
	 * levels.
	 */
	bool bIsGamepadModeForPlayer0;

	/**
	 * Delegate for changing layout of screens in local
	 * multiplayer.
	 */
	FChangeActiveSplitscreenType ChangeActiveSplitscreenTypeDelegate;

	/**
	 * Delegate for closing the last opened session if it
	 * still exists.
	 */
	FCloseLastOpenedSession CloseLastOpenedSessionDelegate;

	/**
	 * Delegate for starting the login to the Epic Games
	 * account.
	 */
	FStartLogin StartLoginDelegate;

	/**
	 * Delegate for passing the name of the player that
	 * finished the logging in.
	 */
	FLoginEnded LoginEndedDelegate;

	/**
	 * Delegate for getting information about the player's
	 * authorization status to his Epic Games account.
	 */
	FIsPlayerLoggedIn IsPlayerLoggedInDelegate;
	
	/**
	 * Delegate for getting the player's name from his Epic
	 * Games account.
	 */
	FGetEOSAccountName GetEOSAccountNameDelegate;

	/** Delegate for starting the new session creation. */
	FCallSessionCreation CallSessionCreationDelegate;

	/**
	 * Delegate for notifying that session's creation was
	 * unsuccessful.
	 */
	FSessionCreationFailed SessionCreationFailedDelegate;

	/** Delegate for starting searching online sessions. */
	FStartFindingSessions StartFindingSessionsDelegate;

	/**
	 * Delegate for canceling the process of finding the
	 * online session (or sessions).
	 */
	FCancelFindingSessions CancelFindingSessionsDelegate;

	/**
	 * Delegate for passing found online sessions to other
	 * class.
	 */
	FFindingSessionsCompleted FindingSessionsCompletedDelegate;

	/** Delegate for starting joining to the online session. */
	FStartJoiningSession StartJoiningSessionDelegate;

	/**
	 * Delegate for notifying that joining to session was
	 * unsuccessful.
	 */
	FJoiningSessionFailed JoiningSessionFailedDelegate;

	/**
	 * Delegate for starting searching private (friend's)
	 * session.
	 */
	FStartFindingPrivateSession StartFindingPrivateSessionDelegate;

	/**
	 * Delegate for indicating that private session's finding
	 * was unsuccessful.
	 */
	FFindingPrivateSessionFailed FindingPrivateSessionFailedDelegate;

	/** Delegate for calling ending of the online session. */
	FStartEndingSession StartEndingSessionDelegate;

	/** Delegate for starting destroying the online session. */
	FCallDestroyingSession CallDestroyingSessionDelegate;
	
	/** Delegate for tracking session destruction. */
	FOnDestroySessionCompleteDelegate OnDestroySessionOnClientCompleteDelegate;
	/**
	 * Delegate Handler related to On Destroy Session Complete
	 * Delegate.
	 */
	FDelegateHandle DH_OnDestroySessionOnClientComplete;
	
	/**
	 * Function for destroying the online session if this
	 * Game Instance is owned by the client.
	 */
	UFUNCTION()
	bool DestroySessionOnClientCustom();

private:
	/**
	 * Function for replying on On Destroy Session Complete
	 * Delegate.
	 * @param InSessionName The name of the session for which
	 * the callback is being called.
	 * @param bWasSuccessful Is True if the asynchronous
	 * process was executed without errors.
	 */
	void OnDestroySessionOnClientComplete(FName InSessionName, bool bWasSuccessful);

protected:
	/** The asset loader for loading assets asynchronously. */
	FStreamableManager AssetLoader;

private:
	UPROPERTY()
	UCPP_SaveManager* SaveManager;

public:
	/**
	 * Function for synchronous receiving class from the
	 * Actor's soft reference.
	 * @param ActorPointer Soft reference to the Actor.
	 * @return Class derived from the soft reference.
	 */
	UClass* GetActorClassBySoftReference(const TSoftClassPtr<AActor>& ActorPointer);

	/**
	 * Function for synchronous receiving class from the
	 * widget blueprint's soft reference.
	 * @param WidgetPointer Soft reference to widget
	 * blueprint.
	 * @return Class derived from the soft reference.
	 */
	UClass* GetWidgetClassBySoftReference(const TSoftClassPtr<UUserWidget>& WidgetPointer);

private:
	/**
	 * Function for synchronous receiving class from any
	 * given soft asset reference by using the asset loader.
	 * @param AssetToLoad Asset reference.
	 * @return Class derived from the asset reference.
	 */
	UFUNCTION()
	UClass* GetClassByAssetLoader(const FSoftObjectPath& AssetToLoad);

public:
	/**
	 * Function for starting the new level.
	 * @param InLevelNumber Level number to load.
	 * @param InSplitscreenType Type of splitscreen to apply.
	 * @param InPlayersNumber Number of player that should
	 * be on level.
	 */
	UFUNCTION()
	void LoadNewLevel(const int32 InLevelNumber,
	                  const int32 InSplitscreenType,
	                  const int32 InPlayersNumber);

	/**
	 * Function that is called after the ending of the level's
	 * loading.
	 */
	UFUNCTION(BlueprintCallable)
	void OnLevelLoaded();

	/**
	 * Function for destroying all local players except the
	 * first one (with index 0).
	 */
	UFUNCTION()
	void DestroyAllLocalPlayers();

	/** Getter for the PlayingMode variable (as Enum). */
	EPlayingMode GetPlayingModeAsEnum() const;

	/** Setter for the PlayingMode variable (as Enum). */
	void SetPlayingModeAsEnum(EPlayingMode NewValue);

	/**
	 * Getter for the PlayingMode variable (as int).
	 * 0 = SinglePlayer,
	 * 1 = LocalMultiplayer,
	 * 2 = OnlineMultiplayerServer,
	 * 3 = OnlineMultiplayerClientInPrivateSession,
	 * 4 = OnlineMultiplayerClientInPublicSession.
	 */
	int32 GetPlayingModeAsInt() const;

	/**
	 * Setter for the PlayingMode variable (as int).
	 * 0 = SinglePlayer,
	 * 1 = LocalMultiplayer,
	 * 2 = OnlineMultiplayerServer,
	 * 3 = OnlineMultiplayerClientInPrivateSession,
	 * 4 = OnlineMultiplayerClientInPublicSession.
	 */
	void SetPlayingModeAsInt(int32 NewValue);

	/** Getter for the CurrentSplitscreenType variable. */
	int32 GetCurrentSplitscreenType() const;

	/** Setter for the CurrentSplitscreenType variable. */
	void SetCurrentSplitscreenType(const int32 NewValue);

	/** Getter for the MaxOnlinePlayersNumber variable. */
	FORCEINLINE int32 GetMaxOnlinePlayersNumber() const { return MaxOnlinePlayersNumber; }

	/** Setter for the MaxOnlinePlayersNumber variable. */
	FORCEINLINE void SetMaxOnlinePlayersNumber(const int32 NewValue) { MaxOnlinePlayersNumber = NewValue; }

	/** Getter for the PlayersNumber variable. */
	FORCEINLINE uint8 GetPlayersNumber() const { return PlayersNumber; }

	/** Setter for the PlayersNumber variable. */
	FORCEINLINE void SetPlayersNumber(const uint8 NewValue) { PlayersNumber = NewValue; }

	/** Getter for the LevelNumber variable. */
	int32 GetLevelNumber() const;

	/** Setter for the LevelNumber variable. */
	void SetLevelNumber(const int32 NewValue);

	/** Getter for the CurrentSessionName variable. */
	FORCEINLINE FString GetCurrentSessionName() const { return CurrentSessionName; }

	/** Setter for the CurrentSessionName variable. */
	FORCEINLINE void SetCurrentSessionName(FString NewValue) { CurrentSessionName = NewValue; }

	/** Getter for the RoomNumber variable. */
	FORCEINLINE uint32 GetCurrentRoomNumber() const { return RoomNumber; }

	/** Setter for the RoomNumber variable. */
	FORCEINLINE void SetCurrentRoomNumber(uint32 NewValue) { RoomNumber = NewValue; }

	/** Function for getting the player's unique ID as string. */
	UFUNCTION(BlueprintCallable)
	FString GetPrimaryPlayerUniqueIdAsString() const;

	/** Getter for the SaveManager variable. */
	UFUNCTION()
	FORCEINLINE UCPP_SaveManager* GetSaveManager() const { return SaveManager; }
};
