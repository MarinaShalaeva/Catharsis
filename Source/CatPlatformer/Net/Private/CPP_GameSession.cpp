// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_GameSession.h"
#include "Misc/Guid.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"

ACPP_GameSession::ACPP_GameSession() : GameInstanceRef(nullptr),
                                       GameModeRef(nullptr),
                                       bSearchingForLANSession(false),
                                       bSearchingForPublicSession(false)
{
	PrimaryActorTick.bCanEverTick = true;

	MaxPlayers = 4;
	MaxSplitscreensPerConnection = 2;
}

void ACPP_GameSession::BeginPlay()
{
	Super::BeginPlay();

	GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->CloseLastOpenedSessionDelegate.BindUObject(
			this, &ACPP_GameSession::CloseLastOpenedSessionIfItStillExists);
		GameInstanceRef->IsPlayerLoggedInDelegate.BindUObject(
			this, &ACPP_GameSession::IsLoggedInToEOS);
		GameInstanceRef->GetEOSAccountNameDelegate.BindUObject(this, &ACPP_GameSession::IsLoggedInToEOS);
		GameInstanceRef->StartLoginDelegate.BindUObject(this, &ACPP_GameSession::LoginToEOS);
		GameInstanceRef->CallSessionCreationDelegate.BindUObject(this, &ACPP_GameSession::CreateSession);
		GameInstanceRef->StartFindingSessionsDelegate.BindUObject(this, &ACPP_GameSession::FindSessions);
		GameInstanceRef->StartFindingPrivateSessionDelegate.BindUObject(this, &ACPP_GameSession::FindPrivateSession);
		GameInstanceRef->CancelFindingSessionsDelegate.BindUObject(this, &ACPP_GameSession::CancelSessionsFinding);
		GameInstanceRef->StartJoiningSessionDelegate.BindUObject(this, &ACPP_GameSession::JoinSession);
		GameInstanceRef->StartEndingSessionDelegate.BindUObject(this, &ACPP_GameSession::EndSession);
		GameInstanceRef->CallDestroyingSessionDelegate.BindUObject(this, &ACPP_GameSession::DestroySessionCustom);
	}

	GameModeRef = Cast<ACPP_GameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameModeRef))
	{
		GameModeRef->StartTravelDelegate.BindUObject(this, &ACPP_GameSession::StartTravel);
		GameModeRef->CallRegisterAllExistingPlayersDelegate.BindUObject(
			this, &ACPP_GameSession::RegisterExistingPlayers);
		GameModeRef->CallRegisterPlayerDelegate.BindUObject(this, &ACPP_GameSession::RegisterPlayerCustom);
		GameModeRef->CallUnregisterPlayerDelegate.BindUObject(this, &ACPP_GameSession::UnregisterPlayerCustom);
	}
	OnLoginCompleteDelegate = FOnLoginCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnLoginCompleteCustom);
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnStartOnlineGameComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnJoinSessionComplete);
	OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnEndSessionCompleteCustom);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnDestroySessionComplete);
	OnUnregisterPlayersCompleteDelegate = FOnUnregisterPlayersCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnUnregisterPlayerCustomComplete);
}

void ACPP_GameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->CloseLastOpenedSessionDelegate.Unbind();
		GameInstanceRef->GetEOSAccountNameDelegate.Unbind();
		GameInstanceRef->StartLoginDelegate.Unbind();
		GameInstanceRef->CallSessionCreationDelegate.Unbind();
		GameInstanceRef->StartFindingSessionsDelegate.Unbind();
		GameInstanceRef->StartFindingPrivateSessionDelegate.Unbind();
		GameInstanceRef->CancelFindingSessionsDelegate.Unbind();
		GameInstanceRef->StartJoiningSessionDelegate.Unbind();
		GameInstanceRef->StartEndingSessionDelegate.Unbind();
		GameInstanceRef->CallDestroyingSessionDelegate.Unbind();
		GameInstanceRef = nullptr;
	}

	DH_OnLoginComplete.Reset();
	DH_OnCreateSessionComplete.Reset();
	DH_OnDestroySessionComplete.Reset();
	DH_OnFindSessionsComplete.Reset();
	DH_OnFindPrivateSessionComplete.Reset();
	DH_OnJoinSessionComplete.Reset();

	if (OnLoginCompleteDelegate.IsBound())
	{
		OnLoginCompleteDelegate.Unbind();
	}
	if (OnCreateSessionCompleteDelegate.IsBound())
	{
		OnCreateSessionCompleteDelegate.Unbind();
	}
	if (OnStartSessionCompleteDelegate.IsBound())
	{
		OnStartSessionCompleteDelegate.Unbind();
	}
	if (OnFindSessionsCompleteDelegate.IsBound())
	{
		OnFindSessionsCompleteDelegate.Unbind();
	}
	if (OnFindPrivateSessionCompleteDelegate.IsBound())
	{
		OnFindPrivateSessionCompleteDelegate.Unbind();
	}
	if (OnJoinSessionCompleteDelegate.IsBound())
	{
		OnJoinSessionCompleteDelegate.Unbind();
	}
	if (OnEndSessionCompleteDelegate.IsBound())
	{
		OnEndSessionCompleteDelegate.Unbind();
	}
	if (OnDestroySessionCompleteDelegate.IsBound())
	{
		OnDestroySessionCompleteDelegate.Unbind();
	}
	if (OnUnregisterPlayersCompleteDelegate.IsBound())
	{
		OnUnregisterPlayersCompleteDelegate.Unbind();
	}

	if (IsValid(GameModeRef))
	{
		GameModeRef->StartTravelDelegate.Unbind();
		GameModeRef->CallRegisterAllExistingPlayersDelegate.Unbind();
		GameModeRef->CallRegisterPlayerDelegate.Unbind();
		GameModeRef->CallUnregisterPlayerDelegate.Unbind();
		GameModeRef = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

bool ACPP_GameSession::IsLoggedInToEOS()
{
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return false;

	// Getting the identity interface.
	const IOnlineIdentityPtr IdentityPtr = OnlineSub->GetIdentityInterface();
	if (!IdentityPtr.IsValid())
		return false;

	return IdentityPtr->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}


bool ACPP_GameSession::IsLoggedInToEOS(FString& OutUserName)
{
	bool bIsLoggedIn = false;

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return bIsLoggedIn;

	// Getting the identity interface.
	const IOnlineIdentityPtr IdentityPtr = OnlineSub->GetIdentityInterface();
	if (!IdentityPtr.IsValid())
		return bIsLoggedIn;

	bIsLoggedIn = IdentityPtr->GetLoginStatus(0) == ELoginStatus::LoggedIn;
	if (bIsLoggedIn)
	{
		OutUserName = IdentityPtr->GetPlayerNickname(0);
	}

	return bIsLoggedIn;
}

void ACPP_GameSession::LoginToEOS()
{
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the identity interface.
	const IOnlineIdentityPtr IdentityPtr = OnlineSub->GetIdentityInterface();
	if (!IdentityPtr.IsValid())
		return;

	FOnlineAccountCredentials Credentials;
	Credentials.Id = FString(TEXT(""));
	Credentials.Token = FString(TEXT(""));
	Credentials.Type = FString(TEXT("accountportal"));

	// Adding the delegate handle.
	DH_OnLoginComplete = IdentityPtr->AddOnLoginCompleteDelegate_Handle(0, OnLoginCompleteDelegate);
	IdentityPtr->Login(0, Credentials);
}

void ACPP_GameSession::OnLoginCompleteCustom(int32 InLocalUserNum, bool bWasSuccessful, const FUniqueNetId& InUserId,
                                             const FString& Error)
{
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the identity interface.
	const IOnlineIdentityPtr IdentityPtr = OnlineSub->GetIdentityInterface();
	if (!IdentityPtr.IsValid())
		return;

	// Clearing the delegate handle.
	IdentityPtr->ClearOnLoginCompleteDelegate_Handle(0, DH_OnLoginComplete);
	DH_OnLoginComplete.Reset();

	if (IsValid(GameInstanceRef) && GameInstanceRef->LoginEndedDelegate.IsBound())
	{
		if (bWasSuccessful)
		{
			GameInstanceRef->LoginEndedDelegate.Execute(true, IdentityPtr->GetPlayerNickname(0));
		}
		else
		{
			GameInstanceRef->LoginEndedDelegate.Execute(false, FString());
		}
	}
}

void ACPP_GameSession::CloseLastOpenedSessionIfItStillExists()
{
	if (!IsValid(GameInstanceRef))
		return;

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	if (!GameInstanceRef->GetCurrentSessionName().IsEmpty() &&
		GameInstanceRef->GetCurrentSessionName() != FString(TEXT("GameSession")) &&
		Sessions->GetSessionState(FName(*GameInstanceRef->GetCurrentSessionName())) != EOnlineSessionState::NoSession)
	{
		DestroySessionCustom(FName(*GameInstanceRef->GetCurrentSessionName()));
	}
}

void ACPP_GameSession::CreateSession(const FString& PlayerName, bool bPublicServer, bool bIsLAN, int32 MaxPlayersNumber)
{
	if (!IsLoggedInToEOS())
	{
		LoginToEOS();
		return;
	}
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Create Session"));

	MaxPlayers = MaxPlayersNumber;
	// FOnlineSessionSettings Settings;
	TSharedPtr<class FOnlineSessionSettings> Settings = MakeShareable(new FOnlineSessionSettings());

	Settings->NumPublicConnections = MaxPlayersNumber > 1 ? MaxPlayersNumber - 1 : 4;
	Settings->NumPrivateConnections = MaxPlayersNumber > 1 ? MaxPlayersNumber - 1 : 4;
	Settings->bShouldAdvertise = true;
	Settings->bAllowJoinInProgress = true;
	Settings->bIsDedicated = false;
	Settings->bUsesPresence = true;
	Settings->bAllowJoinViaPresence = true;
	Settings->bIsLANMatch = bIsLAN;
	Settings->bUseLobbiesIfAvailable = false;

	// Adding the delegate handle.
	DH_OnCreateSessionComplete = Sessions->AddOnCreateSessionCompleteDelegate_Handle(
		OnCreateSessionCompleteDelegate);

	FString BaseSessionNameStr = FString::Printf(TEXT("%s_Session"), *PlayerName);
	const FName UserSessionName = FName(BaseSessionNameStr);
	const struct FGuid GUID = FGuid::NewGuid();

	Settings->Settings.Empty();

	Settings->Set(SEARCH_KEYWORDS, FOnlineSessionSetting(FString(TEXT("CAT_PLATFORMER_GAME")),
	                                                     EOnlineDataAdvertisementType::ViaOnlineService));
	Settings->Set(SEARCH_PRESENCE, FOnlineSessionSetting(true,
	                                                     EOnlineDataAdvertisementType::ViaOnlineService));
	Settings->Set(SEARCH_LOBBIES, FOnlineSessionSetting(false,
	                                                    EOnlineDataAdvertisementType::ViaOnlineService));

	Settings->Set(FName(TEXT("SESSION_NAME")),
	              FOnlineSessionSetting(BaseSessionNameStr,
	                                    EOnlineDataAdvertisementType::ViaOnlineService));

	Settings->Set(FName(TEXT("ROOM_NUMBER")),
	              FOnlineSessionSetting(FString::Printf(TEXT("%u"), GUID.A),
	                                    EOnlineDataAdvertisementType::ViaOnlineService));

	if (bPublicServer)
	{
		Settings->Set(SEARCH_KEYWORDS, FOnlineSessionSetting(FString(TEXT("PUBLIC_SESSION")),
		                                                     EOnlineDataAdvertisementType::ViaOnlineService));
		// 0 - Is public
		// 1 - Is private
		Settings->Set(FName(TEXT("SESSION_PRIVACY")),
		              FOnlineSessionSetting(0, EOnlineDataAdvertisementType::ViaOnlineService));
	}
	else
	{
		// 0 - Is public
		// 1 - Is private
		Settings->Set(FName(TEXT("SESSION_PRIVACY")),
		              FOnlineSessionSetting(1, EOnlineDataAdvertisementType::ViaOnlineService));
	}

	SessionName = UserSessionName;
	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->SetCurrentSessionName(SessionName.ToString());
		GameInstanceRef->SetCurrentRoomNumber(GUID.A);
	}
	Sessions->CreateSession(0, UserSessionName, *Settings);
}

void ACPP_GameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Success = %hhd"), bWasSuccessful);

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	// Clearing the delegate handle.
	Sessions->ClearOnCreateSessionCompleteDelegate_Handle(DH_OnCreateSessionComplete);
	DH_OnCreateSessionComplete.Reset();

	if (bWasSuccessful)
	{
		DH_OnStartSessionComplete = Sessions->AddOnStartSessionCompleteDelegate_Handle(
			OnStartSessionCompleteDelegate);

		Sessions->StartSession(InSessionName);
		NonSeamlessTravel();
	}
	else
	{
		if (IsValid(GameInstanceRef))
		{
			if (GameInstanceRef->SessionCreationFailedDelegate.IsBound())
			{
				GameInstanceRef->SessionCreationFailedDelegate.Execute();
			}
		}
	}
}

void ACPP_GameSession::OnStartOnlineGameComplete(FName InSessionName, bool bWasSuccessful)
{
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	// Clearing the delegate handle.
	Sessions->ClearOnStartSessionCompleteDelegate_Handle(DH_OnStartSessionComplete);
	DH_OnStartSessionComplete.Reset();

	if (bWasSuccessful)
	{
		if (IsValid(GameModeRef))
		{
			RegisterExistingPlayers(GameModeRef->GetWorld());
		}
		if (IsValid(GameInstanceRef))
		{
			SessionName = InSessionName;
			GameInstanceRef->SetCurrentSessionName(InSessionName.ToString());
			GameInstanceRef->SetPlayingModeAsInt(2);
		}
	}
}

void ACPP_GameSession::FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Sessions"));

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
	{
		if (IsValid(GameInstanceRef) && GameInstanceRef->FindingSessionsCompletedDelegate.IsBound())
		{
			GameInstanceRef->FindingSessionsCompletedDelegate.Broadcast(nullptr);
		}
		return;
	}

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
	{
		if (IsValid(GameInstanceRef) && GameInstanceRef->FindingSessionsCompletedDelegate.IsBound())
		{
			GameInstanceRef->FindingSessionsCompletedDelegate.Broadcast(nullptr);
		}
		return;
	}

	if (OnFindPrivateSessionCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindPrivateSessionComplete);
		DH_OnFindPrivateSessionComplete.Reset();
		Sessions->CancelFindSessions();
		OnFindPrivateSessionCompleteDelegate.Unbind();
	}
	if (OnFindSessionsCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindSessionsComplete);
		DH_OnFindSessionsComplete.Reset();
		Sessions->CancelFindSessions();
		OnFindSessionsCompleteDelegate.Unbind();
	}

	SearchSettings.Reset();
	SearchSettings = MakeShareable(new FOnlineSessionSearch());

	SearchSettings->QuerySettings.SearchParams.Empty();

	SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString(TEXT("CAT_PLATFORMER_GAME")),
	                                  EOnlineComparisonOp::Equals);
	SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, false, EOnlineComparisonOp::Equals);

	// 0 - Is public
	// 1 - Is private
	SearchSettings->QuerySettings.Set(FName(TEXT("SESSION_PRIVACY")), 0,
	                                  EOnlineComparisonOp::Equals);

	SearchSettings->bIsLanQuery = bIsLAN;
	SearchSettings->MaxSearchResults = 100;

	bSearchingForLANSession = bIsLAN;
	bSearchingForPublicSession = true;
	SearchingPrivateSessionCode = FString();

	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnFindSessionsComplete);
	// Adding the delegate handle.
	DH_OnFindSessionsComplete = Sessions->AddOnFindSessionsCompleteDelegate_Handle(
		OnFindSessionsCompleteDelegate);

	Sessions->FindSessions(0, SearchSettings.ToSharedRef());
}

void ACPP_GameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Success = %hhd"), bWasSuccessful);

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
	{
		GameInstanceRef->FindingSessionsCompletedDelegate.Broadcast(nullptr);
		return;
	}

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
	{
		GameInstanceRef->FindingSessionsCompletedDelegate.Broadcast(nullptr);
		return;
	}

	// Clearing the delegate handle.
	if (OnFindSessionsCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindSessionsComplete);
		DH_OnFindSessionsComplete.Reset();
		OnFindSessionsCompleteDelegate.Unbind();
	}

	// If the number of sessions found is not zero.
	if (bWasSuccessful && SearchSettings->SearchResults.Num() > 0)
	{
		if (bSearchingForLANSession)
		{
			for (int32 SearchIdx = 0; SearchIdx < SearchSettings->SearchResults.Num(); SearchIdx++)
			{
				if (bSearchingForPublicSession &&
					SearchSettings->SearchResults[SearchIdx].Session.SessionSettings.Settings.FindRef(
						FName(TEXT("SESSION_PRIVACY"))).Data == 1)
				{
					SearchSettings->SearchResults.RemoveAt(SearchIdx);
					SearchIdx--;
				}
			}
		}
		GameInstanceRef->FindingSessionsCompletedDelegate.Broadcast(SearchSettings);
		return;
	}

	GameInstanceRef->FindingSessionsCompletedDelegate.Broadcast(nullptr);
}

bool ACPP_GameSession::FindPrivateSession(FUniqueNetIdRepl LocalUserId, const FString& RoomNumber, const bool bIsLAN)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Private Sessions"));

	bool bSuccess = false;

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
	{
		if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
		{
			GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
		}
		return bSuccess;
	}

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
	{
		if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
		{
			GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
		}
		return bSuccess;
	}

	if (OnFindPrivateSessionCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindPrivateSessionComplete);
		DH_OnFindPrivateSessionComplete.Reset();
		Sessions->CancelFindSessions();
		OnFindPrivateSessionCompleteDelegate.Unbind();
	}
	if (OnFindSessionsCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindSessionsComplete);
		DH_OnFindSessionsComplete.Reset();
		Sessions->CancelFindSessions();
		OnFindSessionsCompleteDelegate.Unbind();
	}
	OnFindPrivateSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ACPP_GameSession::OnFindPrivateSessionComplete);

	// Adding the delegate handle.
	DH_OnFindPrivateSessionComplete = Sessions->AddOnFindSessionsCompleteDelegate_Handle(
		OnFindPrivateSessionCompleteDelegate);

	SearchSettings.Reset();
	SearchSettings = MakeShareable(new FOnlineSessionSearch());

	SearchSettings->QuerySettings.SearchParams.Empty();

	SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString(TEXT("CAT_PLATFORMER_GAME")),
	                                  EOnlineComparisonOp::Equals);
	SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true,
	                                  EOnlineComparisonOp::Equals);
	SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, false,
	                                  EOnlineComparisonOp::Equals);
	SearchSettings->QuerySettings.Set(FName(TEXT("ROOM_NUMBER")), RoomNumber,
	                                  EOnlineComparisonOp::Equals);
	// 0 - Is public
	// 1 - Is private
	SearchSettings->QuerySettings.Set(FName(TEXT("SESSION_PRIVACY")), 1,
	                                  EOnlineComparisonOp::LessThanEquals);

	SearchSettings->bIsLanQuery = bIsLAN;
	SearchSettings->MaxSearchResults = 100;

	bSearchingForLANSession = bIsLAN;
	bSearchingForPublicSession = false;
	SearchingPrivateSessionCode = RoomNumber;

	bSuccess = Sessions->FindSessions(0, SearchSettings.ToSharedRef());

	return bSuccess;
}

void ACPP_GameSession::OnFindPrivateSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindPrivateSessionComplete, Success = %hhd"), bWasSuccessful);

	if (bWasSuccessful)
	{
		// Getting the online subsystem.
		const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (!OnlineSub)
		{
			if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
			{
				GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
			}
			return;
		}

		// Getting the session interface.
		const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (!Sessions.IsValid())
		{
			if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
			{
				GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
			}
			return;
		}

		// Clearing the delegate handle.
		if (OnFindPrivateSessionCompleteDelegate.IsBoundToObject(this))
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindPrivateSessionComplete);
			DH_OnFindPrivateSessionComplete.Reset();
			OnFindPrivateSessionCompleteDelegate.Unbind();
		}

		DH_OnJoinSessionComplete = Sessions->AddOnJoinSessionCompleteDelegate_Handle(
			OnJoinSessionCompleteDelegate);

		if (SearchSettings->SearchResults.Num() > 0)
		{
			if (bSearchingForLANSession)
			{
				for (int32 SearchIdx = 0; SearchIdx < SearchSettings->SearchResults.Num(); SearchIdx++)
				{
					if (!bSearchingForPublicSession &&
						SearchSettings->SearchResults[SearchIdx].Session.SessionSettings.Settings.FindRef(
							FName(TEXT("ROOM_NUMBER"))).Data != SearchingPrivateSessionCode)
					{
						SearchSettings->SearchResults.RemoveAt(SearchIdx);
						SearchIdx--;
					}
				}
			}
			if (SearchSettings->SearchResults.Num() > 0)
			{
				const FOnlineSessionSearchResult Result = SearchSettings->SearchResults[0];
				if (Result.IsValid())
				{
					SessionName = FName(Result.Session.SessionSettings.Settings.FindRef(FName("SESSION_NAME")).
					                           Data.ToString());
					if (IsValid(GameInstanceRef))
					{
						GameInstanceRef->SetCurrentSessionName(SessionName.ToString());
						GameInstanceRef->SetPlayingModeAsInt(3);
					}
					JoinSession(GetGameInstance()->GetPrimaryPlayerUniqueIdRepl().GetUniqueNetId(), SessionName,
					            Result);
				}
			}
			else
			{
				if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
				{
					GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
				}
			}
		}
		else
		{
			if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
			{
				GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
			}
		}
	}
	else
	{
		if (IsValid(GameInstanceRef) && GameInstanceRef->FindingPrivateSessionFailedDelegate.IsBound())
		{
			GameInstanceRef->FindingPrivateSessionFailedDelegate.Execute();
		}
	}
}

void ACPP_GameSession::CancelSessionsFinding()
{
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	Sessions->CancelFindSessions();

	if (OnFindSessionsCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindSessionsComplete);
		DH_OnFindSessionsComplete.Reset();
		OnFindSessionsCompleteDelegate.Unbind();
	}
	if (OnFindPrivateSessionCompleteDelegate.IsBoundToObject(this))
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DH_OnFindPrivateSessionComplete);
		DH_OnFindPrivateSessionComplete.Reset();
		OnFindPrivateSessionCompleteDelegate.Unbind();
	}
}

bool ACPP_GameSession::JoinSession(FUniqueNetIdRepl InUserId,
                                   FName InSessionName,
                                   const FOnlineSessionSearchResult& InSearchResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Join Session"));

	if (!IsLoggedInToEOS())
	{
		LoginToEOS();
		return false;
	}

	bool bSuccess = false;

	if (!InUserId.IsValid())
		return bSuccess;

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return bSuccess;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return bSuccess;

	// Adding the delegate handle.
	DH_OnJoinSessionComplete = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->SetCurrentSessionName(InSessionName.ToString());
		SessionName = InSessionName;
	}
	bSuccess = Sessions->JoinSession(*InUserId.GetUniqueNetId(), InSessionName, InSearchResult);

	return bSuccess;
}

void ACPP_GameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	/** Result:
	 * 0 Success,
	 * 1 SessionIsFull,
	 * 2 SessionDoesNotExist,
	 * 3 CouldNotRetrieveAddress,
	 * 4 AlreadyInSession,
	 * 5 UnknownError
	 */
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete, Success = %d"), Result);

	if (Result == 0)
	{
		// Getting the online subsystem.
		const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (!OnlineSub)
			return;

		// Getting the session interface.
		const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (!Sessions.IsValid())
			return;

		// Clearing the delegate handle.
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(DH_OnJoinSessionComplete);
		DH_OnJoinSessionComplete.Reset();

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (FString TravelURL;
			IsValid(PlayerController) && Sessions->GetResolvedConnectString(InSessionName, TravelURL))
		{
			UE_LOG(LogTemp, Warning, TEXT("TravelURL = %s"), *TravelURL);
			SessionName = InSessionName;
			PlayerController->ClientTravel(TravelURL, TRAVEL_Absolute);
		}
	}
	else if (Result != 4 && IsValid(GameInstanceRef))
	{
		if (GameInstanceRef->JoiningSessionFailedDelegate.IsBound())
		{
			GameInstanceRef->JoiningSessionFailedDelegate.Execute();
		}
		GameInstanceRef->SetPlayingModeAsInt(0);
	}
}

bool ACPP_GameSession::EndSession(const FName& InSessionName)
{
	UE_LOG(LogTemp, Warning, TEXT("EndSession"));

	bool bSuccess = false;

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return bSuccess;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return bSuccess;

	// Adding the delegate handle.
	DH_OnEndSessionComplete = Sessions->AddOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegate);

	if (IsValid(GameModeRef))
	{
		GameModeRef->CallSessionEnding();
	}
	bSuccess = Sessions->EndSession(InSessionName);
	DestroySessionCustom(InSessionName);

	return bSuccess;
}

void ACPP_GameSession::OnEndSessionCompleteCustom(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndSessionCompleteCustom"));

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	// Clearing the delegate handle.
	Sessions->ClearOnEndSessionCompleteDelegate_Handle(DH_OnEndSessionComplete);
	DH_OnEndSessionComplete.Reset();
}

bool ACPP_GameSession::DestroySessionCustom(const FName& InSessionName)
{
	UE_LOG(LogTemp, Warning, TEXT("DestroySessionCustom"));

	bool bSuccess = false;

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return bSuccess;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return bSuccess;

	// Adding the delegate handle.
	DH_OnDestroySessionComplete = Sessions->
		AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

	bSuccess = Sessions->DestroySession(InSessionName, OnDestroySessionCompleteDelegate);
	return bSuccess;
}

void ACPP_GameSession::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete, bWasSuccessful = %hhd"),
	       bWasSuccessful);

	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	// Clearing the delegate handle.
	Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DH_OnDestroySessionComplete);
	DH_OnDestroySessionComplete.Reset();

	if (bWasSuccessful)
	{
		SessionName = FName(TEXT("GameSession"));
		if (IsValid(GameInstanceRef))
		{
			GameInstanceRef->SetPlayingModeAsInt(0);
			GameInstanceRef->SetCurrentSessionName(FString(TEXT("GameSession")));
		}
	}
}

void ACPP_GameSession::RegisterExistingPlayers(const UWorld* World)
{
	if (!IsValid(World))
		return;

	for (auto It = World->GetPlayerControllerIterator(); It; --It)
	{
		RegisterPlayerCustom(It->Get());
	}
}

FUniqueNetIdRepl ACPP_GameSession::RegisterPlayerCustom(const APlayerController* InPlayerController)
{
	if (!IsValid(InPlayerController))
		return FUniqueNetIdRepl();

	if (InPlayerController->GetNetDriverName().IsEqual(NAME_DemoNetDriver))
		return FUniqueNetIdRepl();

	FUniqueNetIdRepl UniqueNetIdRepl;
	if (InPlayerController->IsLocalController())
	{
		const ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
		if (IsValid(LocalPlayer))
		{
			UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
		}
		else
		{
			const UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
			if (!IsValid(RemoteNetConnection))
				return FUniqueNetIdRepl();
			UniqueNetIdRepl = RemoteNetConnection->PlayerId;
		}
	}
	else
	{
		const UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
		if (!IsValid(RemoteNetConnection))
			return FUniqueNetIdRepl();
		UniqueNetIdRepl = RemoteNetConnection->PlayerId;
	}

	const TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
	if (!UniqueNetId.IsValid())
		return FUniqueNetIdRepl();

	const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(InPlayerController->GetWorld());
	if (!OnlineSub)
		return FUniqueNetIdRepl();

	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return FUniqueNetIdRepl();

	Sessions->RegisterPlayer(SessionName, *UniqueNetId, false);

	return UniqueNetIdRepl;
}

void ACPP_GameSession::UnregisterPlayerCustom(const APlayerController* InPlayerController,
                                              const FUniqueNetIdRepl& UniqueNetIdRepl)
{
	if (!IsValid(InPlayerController))
		return;

	// Get the unique player ID.
	const TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
	if (!UniqueNetId.IsValid())
		return;

	const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(InPlayerController->GetWorld());
	if (!OnlineSub)
		return;

	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	// Adding the delegate handle.
	DH_OnUnregisterPlayersComplete = Sessions->
		AddOnUnregisterPlayersCompleteDelegate_Handle(OnUnregisterPlayersCompleteDelegate);

	Sessions->UnregisterPlayer(SessionName, *UniqueNetId);
}

void ACPP_GameSession::OnUnregisterPlayerCustomComplete(FName InSessionName, const TArray<FUniqueNetIdRef>& PlayerId,
                                                        bool bWasSuccessful)
{
	// Getting the online subsystem.
	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
		return;

	// Getting the session interface.
	const IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
		return;

	// Clearing the delegate handle.
	Sessions->ClearOnUnregisterPlayersCompleteDelegate_Handle(DH_OnUnregisterPlayersComplete);
	DH_OnUnregisterPlayersComplete.Reset();
}

bool ACPP_GameSession::StartTravel(const bool bIsSeamless)
{
	if (bIsSeamless)
	{
		return SeamlessTravel();
	}
	return NonSeamlessTravel();
}

bool ACPP_GameSession::SeamlessTravel()
{
	if (!IsValid(GameModeRef))
		return false;

	GameModeRef->bUseSeamlessTravel = true;
	return GameModeRef->GetWorld()->ServerTravel(FString(TEXT("/Game/CatPlatformer/Maps/L_BaseLevel?listen")),
	                                             true);
}

bool ACPP_GameSession::NonSeamlessTravel()
{
	if (!IsValid(GameModeRef))
		return false;

	GameModeRef->bUseSeamlessTravel = false;
	return GameModeRef->GetWorld()->ServerTravel(FString(TEXT("/Game/CatPlatformer/Maps/L_BaseLevel?listen")),
	                                             true);
}
