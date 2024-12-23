﻿// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_EndLevel.h"

#include "CatPlatformer/UI/Classes/WCPP_Level.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_EndLevel::UWCPP_EndLevel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              bIsWinner(false),
                                                                              TB_UserName(nullptr),
                                                                              TB_WinningOrLosing(nullptr),
                                                                              TB_Score(nullptr),
                                                                              TB_Deaths(nullptr),
                                                                              TB_TimeOnLevel(nullptr),
                                                                              PlayerRole_WidgetSwitcher(nullptr),
                                                                              MainPlayerPanel_ScrollBox(nullptr),
                                                                              StartNextLevelButton(nullptr),
                                                                              GoToMainMenuButton(nullptr),
                                                                              ExitButton(nullptr),
                                                                              StartLevel1_Button(nullptr),
                                                                              StartLevel2_Button(nullptr),
                                                                              StartLevel3_Button(nullptr),
                                                                              StartLevel4_Button(nullptr),
                                                                              StartLevel5_Button(nullptr),
                                                                              StartRandomLevel_Button(nullptr),
                                                                              PlayerStateRef(nullptr),
                                                                              ClientPanel_VerticalBox(nullptr),
                                                                              ClientGoToMainMenuButton(nullptr),
                                                                              ClientExitButton(nullptr),
                                                                              LocalPlayersPanel_VerticalBox(nullptr)
{
}

void UWCPP_EndLevel::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GameInstanceRef.IsValid())
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}

	if (!PlayerControllerRef.IsValid())
	{
		PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
	}
	if (PlayerControllerRef.IsValid())
	{
		bIsGamepadMode = PlayerControllerRef->GetIsGamepadMode();
		if (bIsGamepadMode)
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_EndLevel::AnyKeyPressed);
		PlayerControllerRef->CharacterWasPossessedDelegate.
		                     AddUObject(this, &UWCPP_EndLevel::NewCharacterWasPossessed);
		if (PlayerControllerRef->GetCharacter())
		{
			NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetCharacter()));
		}

		FText Name = FText::FromString(PlayerControllerRef->GetCustomUserName());
		if (Name.IsEmptyOrWhitespace())
		{
			Name = DefaultPlayerNameInscription;
		}
		TB_UserName->SetText(Name);
		TB_Score->SetText(FText::FromString(FString::Printf(TEXT("%d"),
		                                                    PlayerControllerRef->GetCustomUserScore())));

		FTimerHandle TH_UpdateScore;
		GetWorld()->GetTimerManager().SetTimer(
			TH_UpdateScore,
			[&]() { if (IsValid(this)) ChangeScore(-1); },
			0.75f,
			false);

		TB_Deaths->SetText(FText::FromString(FString::Printf(TEXT("%u"),
		                                                     PlayerControllerRef->
		                                                     GetCustomCurrentLevelDeathsNumber())));
		TB_TimeOnLevel->SetText(UCPP_StaticLibrary::
			GetTextFromSeconds(PlayerControllerRef->GetCustomTimeOnLevel()));

		if (bIsWinner)
		{
			TB_WinningOrLosing->SetText(YouWinInscription);
		}
		else
		{
			TB_WinningOrLosing->SetText(YouLoseInscription);
		}
		switch (PlayerControllerRef->GetMaxOpenedLevelNumber())
		{
		case 1:
			{
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel2_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel3_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel4_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel5_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartRandomLevel_Button, false, ESlateVisibility::Collapsed);
				break;
			}
		case 2:
			{
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel3_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel4_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel5_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartRandomLevel_Button, false, ESlateVisibility::Collapsed);
				break;
			}
		case 3:
			{
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel4_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel5_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartRandomLevel_Button, false, ESlateVisibility::Collapsed);
				break;
			}
		case 4:
			{
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartLevel5_Button, false, ESlateVisibility::Collapsed);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartRandomLevel_Button, false, ESlateVisibility::Collapsed);
				break;
			}
		case 5:
			{
				UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(
					StartRandomLevel_Button, false, ESlateVisibility::Collapsed);
				break;
			}
		default: break;
		}
	}
	if (GameInstanceRef.IsValid())
	{
		switch (GameInstanceRef->GetPlayingModeAsInt())
		{
		case 1:
			{
				if (PlayerControllerRef.IsValid())
				{
					if (PlayerControllerRef->bIsFirstPlayer)
					{
						PlayerRole_WidgetSwitcher->SetActiveWidgetIndex(0);
					}
					else
					{
						PlayerRole_WidgetSwitcher->SetActiveWidgetIndex(2);
					}
				}
				else
				{
					PlayerRole_WidgetSwitcher->SetActiveWidgetIndex(0);
				}
				break;
			}
		case 3:
		case 4:
			{
				PlayerRole_WidgetSwitcher->SetActiveWidgetIndex(1);
				break;
			}
		default:
			{
				PlayerRole_WidgetSwitcher->SetActiveWidgetIndex(0);
				break;
			}
		}
	}

	switch (PlayerRole_WidgetSwitcher->GetActiveWidgetIndex())
	{
	case 0:
		{
			StartNextLevelButton->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartNextLevelButtonOnClick);
			GoToMainMenuButton->OnClicked.AddDynamic(this, &UWCPP_EndLevel::MainPlayerGoToMainMenuButtonOnClick);
			ExitButton->OnClicked.AddDynamic(this, &UWCPP_EndLevel::MainPlayerExitButtonOnClick);

			StartLevel1_Button->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartLevel1_ButtonOnClick);
			if (StartLevel2_Button->GetIsEnabled())
			{
				StartLevel2_Button->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartLevel2_ButtonOnClick);
			}
			if (StartLevel3_Button->GetIsEnabled())
			{
				StartLevel3_Button->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartLevel3_ButtonOnClick);
			}
			if (StartLevel4_Button->GetIsEnabled())
			{
				StartLevel4_Button->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartLevel4_ButtonOnClick);
			}
			if (StartLevel5_Button->GetIsEnabled())
			{
				StartLevel5_Button->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartLevel5_ButtonOnClick);
			}
			if (StartRandomLevel_Button->GetIsEnabled())
			{
				StartRandomLevel_Button->OnClicked.AddDynamic(this, &UWCPP_EndLevel::StartRandomLevel_ButtonOnClick);
			}
			break;
		}
	case 1:
		{
			ClientGoToMainMenuButton->OnClicked.AddDynamic(this, &UWCPP_EndLevel::ClientGoToMainMenuButtonOnClick);
			ClientExitButton->OnClicked.AddDynamic(this, &UWCPP_EndLevel::ClientExitButtonOnClick);
			break;
		}
	default: break;
	}
}

void UWCPP_EndLevel::NativeDestruct()
{
	switch (PlayerRole_WidgetSwitcher->GetActiveWidgetIndex())
	{
	case 0:
		{
			StartNextLevelButton->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartNextLevelButtonOnClick);
			GoToMainMenuButton->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::MainPlayerGoToMainMenuButtonOnClick);
			ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::MainPlayerExitButtonOnClick);

			StartLevel1_Button->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartLevel1_ButtonOnClick);
			if (StartLevel2_Button->GetIsEnabled())
			{
				StartLevel2_Button->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartLevel2_ButtonOnClick);
			}
			if (StartLevel3_Button->GetIsEnabled())
			{
				StartLevel3_Button->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartLevel3_ButtonOnClick);
			}
			if (StartLevel4_Button->GetIsEnabled())
			{
				StartLevel4_Button->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartLevel4_ButtonOnClick);
			}
			if (StartLevel5_Button->GetIsEnabled())
			{
				StartLevel5_Button->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartLevel5_ButtonOnClick);
			}
			if (StartRandomLevel_Button->GetIsEnabled())
			{
				StartRandomLevel_Button->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::StartRandomLevel_ButtonOnClick);
			}
			break;
		}
	case 1:
		{
			ClientGoToMainMenuButton->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::ClientGoToMainMenuButtonOnClick);
			ClientExitButton->OnClicked.RemoveDynamic(this, &UWCPP_EndLevel::ClientExitButtonOnClick);

			break;
		}
	default: break;
	}

	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();
	}

	if (PlayerStateRef.IsValid())
	{
		PlayerStateRef->ScoreChangedDelegate.Remove(DH_ScoreChanged);
		DH_ScoreChanged.Reset();
	}

	Super::NativeDestruct();
}

void UWCPP_EndLevel::NewCharacterWasPossessed(ACPP_Character* NewCharacter)
{
	if (IsValid(NewCharacter))
	{
		if (!NewCharacter->PlayerStateChangedDelegate.IsBoundToObject(this))
		{
			NewCharacter->PlayerStateChangedDelegate.AddUObject(this, &UWCPP_EndLevel::PlayerStateWasChanged);
			PlayerStateWasChanged(NewCharacter->GetPlayerState<ACPP_PlayerState>());
		}
	}
}

void UWCPP_EndLevel::PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState)
{
	if (!IsValid(NewPlayerState))
		return;

	if (PlayerStateRef.IsValid())
	{
		PlayerStateRef->ScoreChangedDelegate.Remove(DH_ScoreChanged);
		DH_ScoreChanged.Reset();
	}
	PlayerStateRef = NewPlayerState;
	DH_ScoreChanged = PlayerStateRef->ScoreChangedDelegate.AddUObject(this, &UWCPP_EndLevel::ChangeScore);
}

void UWCPP_EndLevel::ChangeScore(const int32 ScoreToAdd)
{
	if (PlayerControllerRef.IsValid())
	{
		TB_Score->SetText(FText::FromString(FString::Printf(TEXT("%d"),
		                                                    PlayerControllerRef->GetCustomUserScore())));
	}
}

void UWCPP_EndLevel::StartNextLevelButtonOnClick()
{
	if (const ACPP_GameState* GS = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
		IsValid(GS))
	{
		StartGame(GS->GetLevelNumber() + 1 > 6 ? 6 : GS->GetLevelNumber() + 1);
	}
	else
	{
		StartGame(1);
	}
}

void UWCPP_EndLevel::MainPlayerGoToMainMenuButtonOnClick()
{
	EndHostOnlineSession();
	if (GameInstanceRef.IsValid())
	{
		if (GameInstanceRef->GetPlayingModeAsInt() == 1)
		{
			GameInstanceRef->DestroyAllLocalPlayers();
			GameInstanceRef->SetCurrentSplitscreenType(0);
			GameInstanceRef->ChangeActiveSplitscreenTypeDelegate.Broadcast(0);
		}
	}
	GoToMainMenu();
}

void UWCPP_EndLevel::MainPlayerExitButtonOnClick()
{
	EndHostOnlineSession();
	Exit();
}

void UWCPP_EndLevel::EndHostOnlineSession() const
{
	if (GameInstanceRef.IsValid())
	{
		if (GameInstanceRef->GetPlayingModeAsInt() == 2 &&
			GameInstanceRef->StartEndingSessionDelegate.IsBound())
		{
			bool bSuccess = GameInstanceRef->StartEndingSessionDelegate.Execute(
				FName(GameInstanceRef->GetCurrentSessionName()));
		}
	}
}

void UWCPP_EndLevel::StartLevel1_ButtonOnClick()
{
	StartGame(1);
}

void UWCPP_EndLevel::StartLevel2_ButtonOnClick()
{
	StartGame(2);
}

void UWCPP_EndLevel::StartLevel3_ButtonOnClick()
{
	StartGame(3);
}

void UWCPP_EndLevel::StartLevel4_ButtonOnClick()
{
	StartGame(4);
}

void UWCPP_EndLevel::StartLevel5_ButtonOnClick()
{
	StartGame(5);
}

void UWCPP_EndLevel::StartRandomLevel_ButtonOnClick()
{
	StartGame(6);
}

void UWCPP_EndLevel::StartGame(const int32 LevelNumber) const
{
	if (!GameInstanceRef.IsValid())
		return;

	if (PlayerControllerRef.IsValid() && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(LevelIsLoadingInscription, 4.5f);
	}

	GameInstanceRef->bIsGamepadModeForPlayer0 = bIsGamepadMode;

	switch (GameInstanceRef->GetPlayingModeAsEnum())
	{
	case EPlayingMode::SinglePlayer:
		{
			GameInstanceRef->LoadNewLevel(LevelNumber,
			                              0,
			                              1);

			break;
		}
	case EPlayingMode::LocalMultiplayer:
		{
			GameInstanceRef->LoadNewLevel(LevelNumber,
			                              GameInstanceRef->GetCurrentSplitscreenType(),
			                              GameInstanceRef->GetPlayersNumber());
			break;
		}
	case EPlayingMode::OnlineMultiplayerServer:
		{
			GameInstanceRef->SetLevelNumber(LevelNumber);
			ACPP_GameState* GS = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
			GS->StartNonSeamlessTravel();
			break;
		}
	default: break;
	}
}


void UWCPP_EndLevel::ClientGoToMainMenuButtonOnClick()
{
	EndClientOnlineSession();
	GoToMainMenu();
}

void UWCPP_EndLevel::ClientExitButtonOnClick()
{
	EndClientOnlineSession();
	Exit();
}

void UWCPP_EndLevel::EndClientOnlineSession() const
{
	if (GameInstanceRef.IsValid())
	{
		if (GameInstanceRef->GetPlayingModeAsInt() == 3 ||
			GameInstanceRef->GetPlayingModeAsInt() == 4)
		{
			if (PlayerControllerRef.IsValid())
			{
				PlayerControllerRef->ClientDestroyOnlineSession();
			}
		}
	}
}

void UWCPP_EndLevel::GoToMainMenu()
{
	if (GameInstanceRef.IsValid())
	{
		GameInstanceRef->SetPlayingModeAsInt(0);
	}
	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->CallCollectingInfoForSavingItToFile();
	}
	else
	{
		if (ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(
			UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PC->CallCollectingInfoForSavingItToFile();
		}
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/CatPlatformer/Maps/L_MainMenu")));
}

void UWCPP_EndLevel::Exit()
{
	const UWorld* World = GetWorld();
	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->CallCollectingInfoForSavingItToFile();
		UKismetSystemLibrary::QuitGame(World,
		                               PlayerControllerRef.Get(),
		                               EQuitPreference::Quit,
		                               false);
	}
	else
	{
		UKismetSystemLibrary::QuitGame(World,
		                               UGameplayStatics::GetPlayerController(World, 0),
		                               EQuitPreference::Quit,
		                               false);
	}
}

void UWCPP_EndLevel::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);
}

void UWCPP_EndLevel::PrepareWidgetForWorkingWithGamepad()
{
	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_EndLevel::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	switch (PlayerRole_WidgetSwitcher->GetActiveWidgetIndex())
	{
	case 0:
		{
			if (PlayerControllerRef.IsValid())
			{
				StartNextLevelButton->SetUserFocus(PlayerControllerRef.Get());
			}
			else
			{
				StartNextLevelButton->SetKeyboardFocus();
			}
			break;
		}
	case 1:
		{
			if (PlayerControllerRef.IsValid())
			{
				ClientGoToMainMenuButton->SetUserFocus(PlayerControllerRef.Get());
			}
			else
			{
				ClientGoToMainMenuButton->SetKeyboardFocus();
			}
			break;
		}
	default: break;
	}
}

void UWCPP_EndLevel::PrepareWidgetForWorkingWithKeyboard()
{
	Super::PrepareWidgetForWorkingWithKeyboard();
}
