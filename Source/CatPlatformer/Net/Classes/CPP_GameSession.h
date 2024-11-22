// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#ifndef CPP_GAMEMODE_H
#define CPP_GAMEMODE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameMode.h"
#endif
class ACPP_GameMode;

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
class IOnlineSubsystem;
class FOnlineSessionSearch;

#include "CPP_GameSession.generated.h"

UCLASS(config = Game)
class CATPLATFORMER_API ACPP_GameSession : public AGameSession
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_GameSession();

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

	/** Weak pointer to the instance of UCPP_GameInstance class. */
	TWeakObjectPtr<UCPP_GameInstance> GameInstanceRef;

	/** Weak pointer to the instance of ACPP_GameMode class. */
	TWeakObjectPtr<ACPP_GameMode> GameModeRef;

	/** Structure for storing found game sessions. */
	TSharedPtr<FOnlineSessionSearch> SearchSettings;

	/**
	 * Delegate for tracking player's login to Epic Games
	 * account.
	 */
	FOnLoginCompleteDelegate OnLoginCompleteDelegate;
	/**
	 * Delegate Handler related to On Create Session
	 * Complete Delegate.
	 */
	FDelegateHandle DH_OnLoginComplete;

	/** Delegate for tracking session creation. */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/**
	 * Delegate Handler related to On Create Session
	 * Complete Delegate.
	 */
	FDelegateHandle DH_OnStartSessionComplete;

	/** Delegate for tracking the start of the session. */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	/**
	 * Delegate Handler related to On Start Session Complete
	 * Delegate.
	 */
	FDelegateHandle DH_OnCreateSessionComplete;

	/** Delegate for tracking the end of the session search. */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	/**
	 * Delegate Handler related to On Find Sessions Complete
	 * Delegate.
	 */
	FDelegateHandle DH_OnFindSessionsComplete;

	/**
	 * Delegate for tracking the end of the private session
	 * search.
	 */
	FOnFindSessionsCompleteDelegate OnFindPrivateSessionCompleteDelegate;
	/**
	 * Delegate Handler related to On Find Private Session
	 * Complete.
	 */
	FDelegateHandle DH_OnFindPrivateSessionComplete;

	/** Delegate for tracking join to the session. */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	/**
	 * Delegate Handler related to On Join Session Complete
	 * Delegate.
	 */
	FDelegateHandle DH_OnJoinSessionComplete;

	/**
	 * Delegate for tracking session's ending (initiated by
	 * the server).
	 */
	FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;
	/**
	 * Delegate Handler related to On End Session Complete
	 * Delegate.
	 */
	FDelegateHandle DH_OnEndSessionComplete;

	/** Delegate for tracking session destruction. */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	/**
	 * Delegate Handler related to On Destroy Session Complete
	 * Delegate.
	 */
	FDelegateHandle DH_OnDestroySessionComplete;

	/** Delegate for tracking player's unregistering ending. */
	FOnUnregisterPlayersCompleteDelegate OnUnregisterPlayersCompleteDelegate;
	/**
	 * Delegate Handler related to On Unregister Players
	 * Complete Delegate.
	 */
	FDelegateHandle DH_OnUnregisterPlayersComplete;

public:
	/**
	 * Function for checking if the player is logged in.
	 * @return Is player logged in to any Epic Games account?
	 */
	bool IsLoggedInToEOS();
	/**
	 * Function for checking if the player is logged in and
	 * for returning his nickname (or empty string).
	 * @return Is player logged in to any Epic Games account?
	 */
	bool IsLoggedInToEOS(FString& OutUserName);

	/** Function for player's login to Epic Games account. */
	void LoginToEOS();

private:
	/** Function for replying on OnLoginCompleteDelegate. */
	void OnLoginCompleteCustom(int32 InLocalUserNum, bool bWasSuccessful,
	                           const FUniqueNetId& InUserId,
	                           const FString& Error);

	/**
	 * Function for checking if the last opened session
	 * still exists and destroy it if so.
	 */
	void CloseLastOpenedSessionIfItStillExists();

public:
	/** Function for creating the new online session. */
	void CreateSession(const FString& PlayerName,
	                   bool bPublicServer, bool bIsLAN,
	                   int32 MaxPlayersNumber);

private:
	/**
	 * Function for replying on OnCreateSessionCompleteDelegate.
	 * @param InSessionName The name of the session for which
	 * the callback is being called.
	 * @param bWasSuccessful Is True if the asynchronous process
	 * was executed without errors.
	 */
	virtual void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);

	/**
	 * Function for replying on OnStartSessionCompleteDelegate.
	 * @param InSessionName The name of the session for which
	 * the callback is being called.
	 * @param bWasSuccessful Is True if the asynchronous process
	 * was executed without errors.
	 */
	void OnStartOnlineGameComplete(FName InSessionName, bool bWasSuccessful);

	/** Are we looking for the LAN sessions? */
	bool bSearchingForLANSession;
	/** Are we looking for the public session? */
	bool bSearchingForPublicSession;
	/** Code of the session we are looking for. */
	FString SearchingPrivateSessionCode;

public:
	/**
	 * Function for working with external online subsystems
	 * for finding other players.
	 * @param UserId The user initiating the search.
	 * @param bIsLAN Are we looking on the local network?
	 */
	void FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN);

private:
	/**
	 * Function for replying on On Find Sessions Complete
	 * Delegate.
	 * @param bWasSuccessful Is True if the asynchronous
	 * process was executed without errors.
	 */
	void OnFindSessionsComplete(bool bWasSuccessful);

public:
	/**
	 * Function for finding a friend's online session.
	 * @param LocalUserId The local player wanting to join.
	 * @param RoomNumber The online room number.
	 * @param bIsLAN Are we looking on the local network?
	 * @return True if the async call worked, false otherwise.
	 */
	bool FindPrivateSession(FUniqueNetIdRepl LocalUserId, const FString& RoomNumber, const bool bIsLAN);

private:
	/**
	 * Function for replying on On Find Private Session
	 * Complete Delegate.
	 * @param bWasSuccessful The session was found and is
	 * joinable, false otherwise.
	 */
	void OnFindPrivateSessionComplete(bool bWasSuccessful);

public:
	/**
	 * Function for canceling the process of finding the
	 * online session (or sessions).
	 */
	void CancelSessionsFinding();

	/**
	 * Function for joining the online session.
	 * @param InUserId The id of the player searching for a
	 * match.
	 * @param InSessionName The name of the session to join.
	 * @param InSearchResult The desired session to join.
	 * @return Was joining successful?
	 */
	bool JoinSession(FUniqueNetIdRepl InUserId,
	                 FName InSessionName,
	                 const FOnlineSessionSearchResult& InSearchResult);

private:
	/**
	 * Function for replying on On Join Session Complete
	 * Delegate.
	 * @param InSessionName The name of the session for
	 * which the callback is being called.
	 * @param Result Joining result.
	 */
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);

public:
	/**
	 * Function for ending the online session.
	 * @param InSessionName The name of the session to end.
	 */
	bool EndSession(const FName& InSessionName);

private:
	/**
	 * Function for replying on On End Session Complete
	 * Delegate.
	 * @param InSessionName The name of the session for which
	 * the callback is being called.
	 * @param bWasSuccessful Is True if the asynchronous
	 * process was executed without errors.
	 */
	void OnEndSessionCompleteCustom(FName InSessionName, bool bWasSuccessful);

public:
	/**
	 * Function for destroying the online session.
	 * @param InSessionName The name of the session to
	 * destroy.
	 */
	UFUNCTION()
	bool DestroySessionCustom(const FName& InSessionName);

private:
	/**
	 * Function for replying on On Destroy Session Complete
	 * Delegate.
	 * @param InSessionName The name of the session for which
	 * the callback is being called.
	 * @param bWasSuccessful Is True if the asynchronous
	 * process was executed without errors.
	 */
	void OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful);

public:
	/**
	 * Function for registering all existing at the moment
	 * players.
	 * @param World World associated with current level.
	 */
	void RegisterExistingPlayers(const UWorld* World);

	/**
	 * Function for registering player in multiplayer game.
	 * @param InPlayerController Player Controller related
	 * to the player that should be registered.
	 * @return Unique Net ID of the registered player.
	 */
	FUniqueNetIdRepl RegisterPlayerCustom(const APlayerController* InPlayerController);

	/**
	 * Function for unregistering player in multiplayer game.
	 * @param InPlayerController Player Controller related
	 * to the player that should be registered.
	 * @param UniqueNetIdRepl Unique Net ID of the registered player.
	 */
	void UnregisterPlayerCustom(const APlayerController* InPlayerController, const FUniqueNetIdRepl& UniqueNetIdRepl);

	/**
	 * Function for replying on Add On Unregister Players
	 * Complete Delegate.
	 * @param InSessionName Session in which the player
	 * lived.
	 * @param PlayerId The player's Unique ID.
	 * @param bWasSuccessful Was the unregister process
	 * successful? 
	 */
	void OnUnregisterPlayerCustomComplete(FName InSessionName, const TArray<FUniqueNetIdRef>& PlayerId,
	                                      bool bWasSuccessful);

	/**
	 * Travelling to another map.
	 * @param bIsSeamless Should the seamless travel be
	 * performed?
	 * @return Success.
	 */
	bool StartTravel(const bool bIsSeamless);

private:
	/**
	 * Performing the seamless travel (usually used for
	 * transitions between levels in online multiplayer).
	 * @return Success.
	 */
	bool SeamlessTravel();

	/**
	 * Performing the non-seamless travel (all players will
	 * disconnect and than re-connect to the same server).
	 * @return Success.
	 */
	bool NonSeamlessTravel();
};
