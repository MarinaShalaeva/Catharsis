// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Level.h"
#include "GameMapsSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_Level::UWCPP_Level(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                        PlayerStateRef(nullptr),
                                                                        GameStateRef(nullptr),
                                                                        bCursorShouldBeVisible(true),
                                                                        LevelCanvasPanel(nullptr),
                                                                        ReadyForGame_WidgetSwitcher(nullptr),
                                                                        ReadyForGameButton(nullptr),
                                                                        NotWaitingForOtherPlayersButton(nullptr),
                                                                        RoomNumberVerticalBox(nullptr),
                                                                        TB_RoomNumber(nullptr),
                                                                        CopyRoomNumber_WidgetSwitcher(nullptr),
                                                                        CopyRoomNumberButton(nullptr),
                                                                        TB_CurrentPlayersNumber_Keyboard(nullptr),
                                                                        TB_CurrentPlayersNumber_Gamepad(nullptr),
                                                                        CurrentScore(0),
                                                                        TB_Score(nullptr),
                                                                        TB_ScoreToAdd(nullptr),
                                                                        BuffsVerticalBox(nullptr),
                                                                        TB_TimerBeforeGameStart(nullptr),
                                                                        TimerBeforeGameStartCounter(3)
{
}

void UWCPP_Level::NativeConstruct()
{
	Super::NativeConstruct();

	UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(TB_TimerBeforeGameStart, ESlateVisibility::Collapsed);

	ReadyForGameButton->OnClicked.AddDynamic(this, &UWCPP_Level::ReadyForGameButtonOnClick);
	NotWaitingForOtherPlayersButton->OnClicked.AddDynamic(this, &UWCPP_Level::NotWaitingForOtherPlayersButtonOnClick);
	CopyRoomNumberButton->OnClicked.AddDynamic(this, &UWCPP_Level::CopyRoomNumberButtonOnClick);

	if (!IsValid(GameInstanceRef))
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}

	if (!IsValid(PlayerControllerRef))
	{
		if (IsValid(GameInstanceRef))
		{
			if (GameInstanceRef->GetPlayingModeAsInt() == 1)
			{
				PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningLocalPlayer()->PlayerController);
			}
			else
			{
				PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
			}
		}
		else
		{
			PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
		}
	}

	if (IsValid(PlayerControllerRef))
	{
		if (IsValid(GameInstanceRef))
		{
			if (GameInstanceRef->GetPlayingModeAsInt() == 1)
			{
				if (PlayerControllerRef->NetPlayerIndex != 0)
				{
					bIsGamepadMode = true;
				}
				else
				{
					bIsGamepadMode = !UGameMapsSettings::GetGameMapsSettings()->GetSkipAssigningGamepadToPlayer1();
				}
			}
			else
			{
				bIsGamepadMode = GameInstanceRef->bIsGamepadModeForPlayer0;
			}
		}
		else
		{
			bIsGamepadMode = PlayerControllerRef->GetIsGamepadMode();
		}

		bCursorShouldBeVisible = !bIsGamepadMode;

		if (bIsGamepadMode)
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}

		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_Level::AnyKeyPressed);
		PlayerControllerRef->CharacterWasPossessedDelegate.AddUObject(this,
		                                                              &UWCPP_Level::NewCharacterWasPossessed);
		if (PlayerControllerRef->GetCharacter())
		{
			NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetCharacter()));
		}
	}

	GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (IsValid(GameInstanceRef) && GameInstanceRef->GetPlayingModeAsInt() == 2)
	{
		if (IsValid(GameStateRef))
		{
			DH_PlayersNumberChanged = GameStateRef->PlayersNumberChangedDelegate.AddUObject(
				this, &UWCPP_Level::PlayersNumberWasChanged);
		}

		TB_RoomNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"),
		                                                         GameInstanceRef->GetCurrentRoomNumber())));
		RoomNumberVerticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CopyRoomNumberButton, true);
	}
	else
	{
		RoomNumberVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CopyRoomNumberButton, false);
	}
}

void UWCPP_Level::NativeDestruct()
{
	ReadyForGameButton->OnClicked.RemoveDynamic(this, &UWCPP_Level::ReadyForGameButtonOnClick);
	NotWaitingForOtherPlayersButton->OnClicked.
	                                 RemoveDynamic(this, &UWCPP_Level::NotWaitingForOtherPlayersButtonOnClick);
	CopyRoomNumberButton->OnClicked.RemoveDynamic(this, &UWCPP_Level::CopyRoomNumberButtonOnClick);

	if (IsValid(GameInstanceRef))
	{
		if (IsValid(GameStateRef))
		{
			if (GameInstanceRef->GetPlayingModeAsInt() == 2)
			{
				GameStateRef->PlayersNumberChangedDelegate.Remove(DH_PlayersNumberChanged);
				DH_PlayersNumberChanged.Reset();
			}
			GameStateRef = nullptr;
		}
		GameInstanceRef = nullptr;
	}

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();

		PlayerControllerRef = nullptr;
	}

	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementNumberOfStartedLevels();
		PlayerStateRef->ScoreChangedDelegate.Unbind();
		PlayerStateRef->ShouldBeginCountdownToStartLevelDelegate.Remove(DH_ShouldBeginCountdownToStartLevel);
		DH_ShouldBeginCountdownToStartLevel.Reset();
	}

	Super::NativeDestruct();
}

void UWCPP_Level::NewCharacterWasPossessed(ACPP_Character* NewCharacter)
{
	if (IsValid(NewCharacter))
	{
		if (!NewCharacter->PlayerStateChangedDelegate.IsBoundToObject(this))
		{
			NewCharacter->PlayerStateChangedDelegate.AddUObject(this, &UWCPP_Level::PlayerStateWasChanged);
			PlayerStateWasChanged(NewCharacter->GetPlayerState<ACPP_PlayerState>());
		}
	}
}

void UWCPP_Level::PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState)
{
	if (!IsValid(NewPlayerState))
		return;

	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->ScoreChangedDelegate.Unbind();
		PlayerStateRef->ShouldBeginCountdownToStartLevelDelegate.Remove(DH_ShouldBeginCountdownToStartLevel);
		DH_ShouldBeginCountdownToStartLevel.Reset();
	}
	PlayerStateRef = NewPlayerState;
	PlayerStateRef->ScoreChangedDelegate.BindUObject(this, &UWCPP_Level::ChangeScore);
	DH_ShouldBeginCountdownToStartLevel = PlayerStateRef->ShouldBeginCountdownToStartLevelDelegate.
	                                                      AddUObject(this, &UWCPP_Level::BeginCountdownToStartLevel);
}

void UWCPP_Level::ChangeScore(const int32 ScoreToAdd)
{
	if (ScoreToAdd == 0)
		return;

	CurrentScore = CurrentScore + ScoreToAdd < 0 ? 0 : CurrentScore + ScoreToAdd;
	if (ScoreToAdd > 0)
	{
		TB_ScoreToAdd->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ScoreToAdd)));
	}
	else
	{
		TB_ScoreToAdd->SetText(FText::FromString(FString::Printf(TEXT("%d"), ScoreToAdd)));
	}
	CallChangingScoreAnimation();
}

void UWCPP_Level::SetCurrentScoreToTextBlock()
{
	TB_ScoreToAdd->SetText(FText());
	TB_Score->SetText(FText::AsNumber(CurrentScore));
}

void UWCPP_Level::StartingKeyWasPressed()
{
	if (ReadyForGame_WidgetSwitcher->GetVisibility() == ESlateVisibility::Collapsed ||
		ReadyForGame_WidgetSwitcher->GetVisibility() == ESlateVisibility::Hidden ||
		ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 4)
		return;

	if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 0 ||
		ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 1)
	{
		ReadyForGameButtonOnClick();
	}
	else if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 2 ||
		ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 3)
	{
		NotWaitingForOtherPlayersButtonOnClick();
	}
}

void UWCPP_Level::ReadyForGameButtonOnClick()
{
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ReadyForGameButton, false);

	if (IsValid(GameInstanceRef))
	{
		if (GameInstanceRef->GetPlayingModeAsInt() == 2)
		{
			if (!IsValid(GameStateRef))
			{
				GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
			}
			if (IsValid(GameStateRef))
			{
				if (GameStateRef->PlayerArray.Num() < GameInstanceRef->GetMaxOnlinePlayersNumber())
				{
					PlayersNumberWasChanged(GameStateRef->PlayerArray.Num());
					if (bIsGamepadMode)
					{
						ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(3);
					}
					else
					{
						ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(2);
					}
					return;
				}
			}
			if (!bIsGamepadMode)
			{
				bCursorShouldBeVisible = true;
			}
		}
		else
		{
			bCursorShouldBeVisible = false;
		}
	}
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->SetPlayerIsReadyToStartGame(true);
		PlayerControllerRef->ChangeCursorVisibility(bCursorShouldBeVisible);
	}
	ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(4);
}

void UWCPP_Level::PlayersNumberWasChanged_Implementation(const int32 NewPlayersNumber)
{
	TB_CurrentPlayersNumber_Keyboard->SetText(FText::AsNumber(NewPlayersNumber));
	TB_CurrentPlayersNumber_Gamepad->SetText(FText::AsNumber(NewPlayersNumber));

	if (IsValid(GameInstanceRef))
	{
		if ((ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 2 ||
				ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 3) &&
			NewPlayersNumber >= GameInstanceRef->GetMaxOnlinePlayersNumber())
		{
			NotWaitingForOtherPlayersButtonOnClick();
		}
		else if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 4 &&
			NewPlayersNumber < GameInstanceRef->GetMaxOnlinePlayersNumber())
		{
			if (bIsGamepadMode)
			{
				ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(3);
			}
			else
			{
				ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(2);
			}
			PlayerControllerRef->SetPlayerIsReadyToStartGame(false);
			UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(NotWaitingForOtherPlayersButton, true);
		}
	}
}

void UWCPP_Level::NotWaitingForOtherPlayersButtonOnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->GetPlayingModeAsInt() == 2 &&
		IsValid(GameStateRef) && GameStateRef->PlayerArray.Num() <= 1 &&
		IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(OnlyOnePlayerOnServerInscription, 4.0f);
		return;
	}

	ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(4);
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(NotWaitingForOtherPlayersButton, false);
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->SetPlayerIsReadyToStartGame(true);
	}
}

void UWCPP_Level::CopyRoomNumberButtonOnClick()
{
	FPlatformApplicationMisc::ClipboardCopy(*TB_RoomNumber->GetText().ToString());
	if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(RoomNumberWasCopiedInscription, 3.5f);
	}
}

void UWCPP_Level::BeginCountdownToStartLevel()
{
	bCursorShouldBeVisible = false;
	PlayerControllerRef->ChangeCursorVisibility(false);
	ReadyForGame_WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	RoomNumberVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	CopyRoomNumberButton->SetIsEnabled(false);
	UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(TB_TimerBeforeGameStart,
	                                                     ESlateVisibility::SelfHitTestInvisible);

	GetWorld()->GetTimerManager().SetTimer(
		TH_ChangingDigitSize,
		this,
		&UWCPP_Level::DecreaseTimerBeforeGameStartCounter,
		1.0f,
		false);
	CallChangingDigitSizeAnimation();
}

void UWCPP_Level::DecreaseTimerBeforeGameStartCounter()
{
	if (TimerBeforeGameStartCounter > 0)
	{
		TimerBeforeGameStartCounter--;
		FText TextToSet;
		if (TimerBeforeGameStartCounter > 0)
		{
			TextToSet = FText::AsNumber(TimerBeforeGameStartCounter);
		}
		else
		{
			TextToSet = StartLevelInscription;
		}
		TB_TimerBeforeGameStart->SetText(TextToSet);
		if (GetWorld()->GetTimerManager().TimerExists(TH_ChangingDigitSize))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_ChangingDigitSize);
		}
		GetWorld()->GetTimerManager().SetTimer(
			TH_ChangingDigitSize,
			this,
			&UWCPP_Level::DecreaseTimerBeforeGameStartCounter,
			1.0f,
			false);
		CallChangingDigitSizeAnimation();
		if (TimerBeforeGameStartCounter == 0 && IsValid(PlayerControllerRef))
		{
			PlayerControllerRef->bWaitingForReadinessToStartLevel = false;
		}
	}
	else
	{
		TimerBeforeGameStartCounter = 3;
		if (GetWorld()->GetTimerManager().TimerExists(TH_ChangingDigitSize))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_ChangingDigitSize);
		}
		UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(TB_TimerBeforeGameStart,
		                                                     ESlateVisibility::Collapsed);
	}
}

void UWCPP_Level::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (!bIsWidgetActive)
		return;

	if (bIsGamepadMode && ReadyForGame_WidgetSwitcher->GetVisibility() != ESlateVisibility::Collapsed)
	{
		bCursorShouldBeVisible = true;
	}

	if (IsValid(PlayerControllerRef) && PlayerControllerRef->IsPaused())
		return;

	if (PressedKey == EKeys::Gamepad_FaceButton_Bottom || // A button.
		PressedKey == EKeys::Enter)
	{
		StartingKeyWasPressed();
	}
	else if (PressedKey == EKeys::Gamepad_FaceButton_Left && // X button.
		RoomNumberVerticalBox->GetVisibility() != ESlateVisibility::Collapsed)
	{
		CopyRoomNumberButtonOnClick();
	}
}

void UWCPP_Level::PrepareWidgetForWorkingWithGamepad()
{
	bCursorShouldBeVisible = false;

	if (RoomNumberVerticalBox->GetVisibility() != ESlateVisibility::Collapsed)
	{
		CopyRoomNumber_WidgetSwitcher->SetActiveWidgetIndex(1);
	}

	if (ReadyForGame_WidgetSwitcher->GetVisibility() != ESlateVisibility::Collapsed)
	{
		if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 0)
		{
			ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(1);
		}
		else if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 2)
		{
			ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(3);
		}
	}

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_Level::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();
}

void UWCPP_Level::PrepareWidgetForWorkingWithKeyboard()
{
	if (ReadyForGame_WidgetSwitcher->GetVisibility() != ESlateVisibility::Collapsed)
	{
		if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() != 4)
		{
			bCursorShouldBeVisible = true;
		}
		if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 1)
		{
			ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(0);
		}
		else if (ReadyForGame_WidgetSwitcher->GetActiveWidgetIndex() == 3)
		{
			ReadyForGame_WidgetSwitcher->SetActiveWidgetIndex(2);
		}
	}

	if (RoomNumberVerticalBox->GetVisibility() != ESlateVisibility::Collapsed)
	{
		bCursorShouldBeVisible = true;
		CopyRoomNumber_WidgetSwitcher->SetActiveWidgetIndex(0);
	}

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->ChangeCursorVisibility(bCursorShouldBeVisible);
	}

	Super::PrepareWidgetForWorkingWithKeyboard();
}
