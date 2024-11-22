// (c) M. A. Shalaeva, 2023

#include "../Classes/CPP_GameInstance.h"
#include "Kismet/GameplayStatics.h"

#ifndef CPP_GAMEMODE_H
#define CPP_GAMEMODE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameMode.h"
#endif

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerController.h"
#endif

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif

UCPP_GameInstance::UCPP_GameInstance()
{
	PlayingMode = EPlayingMode::SinglePlayer;
	bIsGamepadModeForPlayer0 = false;
	CurrentSplitscreenType = 0;
	MaxOnlinePlayersNumber = 2;
	LevelNumber = 1;
	PlayersNumber = 1;
	RoomNumber = 0;
	CurrentSessionName = FString(TEXT("GameSession"));
	SaveManager = nullptr;
}

void UCPP_GameInstance::Init()
{
	Super::Init();

	SaveManager = NewObject<UCPP_SaveManager>();
}

bool UCPP_GameInstance::DestroySessionOnClientCustom()
{
	UE_LOG(LogTemp, Warning, TEXT("UCPP_GameInstance::DestroySessionOnClientCustom"));

	if (PlayingMode != EPlayingMode::OnlineMultiplayerClientInPublicSession &&
		PlayingMode != EPlayingMode::OnlineMultiplayerClientInPrivateSession)
		return false;

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
	DH_OnDestroySessionOnClientComplete = Sessions->
		AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionOnClientCompleteDelegate);

	bSuccess = Sessions->DestroySession(FName(*CurrentSessionName), OnDestroySessionOnClientCompleteDelegate);
	return bSuccess;
}

void UCPP_GameInstance::OnDestroySessionOnClientComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("UCPP_GameInstance::OnDestroySessionOnClientComplete, Success = %hhd"),
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
	Sessions->ClearOnDestroySessionCompleteDelegate_Handle(DH_OnDestroySessionOnClientComplete);
	DH_OnDestroySessionOnClientComplete.Reset();

	if (bWasSuccessful)
	{
		SetPlayingModeAsInt(0);
		SetCurrentSessionName(FString(TEXT("GameSession")));
	}
}

UClass* UCPP_GameInstance::GetActorClassBySoftReference(const TSoftClassPtr<AActor>& ActorPointer)
{
	if (ActorPointer.IsNull())
		return nullptr;

	const FSoftObjectPath AssetToLoad = ActorPointer.ToSoftObjectPath();
	return GetClassByAssetLoader(AssetToLoad);
}

UClass* UCPP_GameInstance::GetWidgetClassBySoftReference(const TSoftClassPtr<UUserWidget>& WidgetPointer)
{
	if (WidgetPointer.IsNull())
		return nullptr;

	const FSoftObjectPath AssetToLoad = WidgetPointer.ToSoftObjectPath();
	return GetClassByAssetLoader(AssetToLoad);
}

UClass* UCPP_GameInstance::GetClassByAssetLoader(const FSoftObjectPath& AssetToLoad)
{
	AssetLoader.LoadSynchronous(AssetToLoad);
	return Cast<UClass>(StaticLoadObject(UClass::StaticClass(),
	                                     nullptr,
	                                     *AssetToLoad.ToString()));
}

void UCPP_GameInstance::LoadNewLevel(const int32 InLevelNumber,
                                     const int32 InSplitscreenType,
                                     const int32 InPlayersNumber)
{
	DestroyAllLocalPlayers();
	LevelNumber = InLevelNumber;
	SetCurrentSplitscreenType(InSplitscreenType);
	PlayersNumber = InPlayersNumber;

	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("L_BaseLevel")));
}

void UCPP_GameInstance::OnLevelLoaded()
{
	if (ACPP_GameMode* GameMode = Cast<ACPP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		IsValid(GameMode) && !GameMode->bLevelWasGenerated)
	{
		GameMode->CallLevelGeneration(LevelNumber);
	}
	uint8 ColorIndexForMainPlayer = 0;
	if (APlayerController* PlayerController = GetPrimaryPlayerController(false))
	{
		if (ACPP_PlayerController* PC0 = Cast<ACPP_PlayerController>(PlayerController))
		{
			PC0->SetIsGamepadMode(bIsGamepadModeForPlayer0);
			if (ACPP_PlayerState* PS0 = PC0->GetPlayerState<ACPP_PlayerState>())
			{
				PS0->LoadInfoFromSaveFile();
				PS0->UpdateCatsColorUpToSavedIndex();
				ColorIndexForMainPlayer = PS0->GetCatColorIndex();
			}
		}
	}

	// Local multiplayer.
	if (CurrentSplitscreenType != 0)
	{
		FString Error;
		for (int32 i = 0; i < PlayersNumber - 1; i++)
		{
			const ULocalPlayer* Player = CreateLocalPlayer(-1, Error, true);
			FString Name = FString::Printf(TEXT("Player%d"), Player->GetControllerId());
			if (ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(Player->PlayerController))
			{
				PC->SetIsGamepadMode(true);
				PC->bIsFirstPlayer = false;
				if (ACPP_PlayerState* PS = PC->GetPlayerState<ACPP_PlayerState>())
				{
					// Subsequently, the remainder of dividing the number
					// ColorIndexForLocalPlayer by 3 will be used in
					// functions SetCurrentColorVariableByIndex() and
					// SetCurrentColorMaterialByIndex(), so we don't need
					// to perform the % 3 operation now.
					const uint8 ColorIndexForLocalPlayer = ColorIndexForMainPlayer + i + 1;
					PS->SetCatColorIndex(ColorIndexForLocalPlayer);
					PS->SetUserName(Name);
					PS->SetIsFirstPlayer(false);
				}
			}
		}

		ChangeActiveSplitscreenTypeDelegate.Broadcast(CurrentSplitscreenType);
	}
}

void UCPP_GameInstance::DestroyAllLocalPlayers()
{
	for (int8 i = GetNumLocalPlayers() - 1; i >= 1; i--)
	{
		RemoveLocalPlayer(LocalPlayers[i]);
	}
}

EPlayingMode UCPP_GameInstance::GetPlayingModeAsEnum() const
{
	return PlayingMode;
}

void UCPP_GameInstance::SetPlayingModeAsEnum(EPlayingMode NewValue)
{
	PlayingMode = NewValue;
}

int32 UCPP_GameInstance::GetPlayingModeAsInt() const
{
	return static_cast<int32>(PlayingMode);
}

void UCPP_GameInstance::SetPlayingModeAsInt(int32 NewValue)
{
	if (NewValue < 0 || NewValue > 4)
		return;

	PlayingMode = static_cast<EPlayingMode>(NewValue);
}

int32 UCPP_GameInstance::GetCurrentSplitscreenType() const
{
	return CurrentSplitscreenType;
}

void UCPP_GameInstance::SetCurrentSplitscreenType(const int32 NewValue)
{
	if (NewValue < 0 || NewValue > 9)
		return;

	CurrentSplitscreenType = NewValue;
}

int32 UCPP_GameInstance::GetLevelNumber() const
{
	return LevelNumber;
}

void UCPP_GameInstance::SetLevelNumber(const int32 NewValue)
{
	if (NewValue < 1 || NewValue > 6)
		return;

	LevelNumber = NewValue;
}

FString UCPP_GameInstance::GetPrimaryPlayerUniqueIdAsString() const
{
	return GetPrimaryPlayerUniqueIdRepl()->ToString();
}
