// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Pause.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_Pause::UWCPP_Pause(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                        WidgetBlueprintsDataTable(nullptr),
                                                                        Settings_Widget(nullptr),
                                                                        TB_UpInscription(nullptr),
                                                                        PanelsWidgetSwitcher(nullptr),
                                                                        TB_UserName(nullptr),
                                                                        BackToGameButton(nullptr),
                                                                        SettingsButton(nullptr),
                                                                        GoToMainMenuButton(nullptr),
                                                                        ExitButton(nullptr),
                                                                        GoBackButton(nullptr),
                                                                        BottomPanel_WidgetSwitcher(nullptr),
                                                                        SettingsSizeBox(nullptr)
{
}

void UWCPP_Pause::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PauseMenuInscription.IsEmpty())
	{
		ChangeUpInscriptionText(PauseMenuInscription);
	}

	ChangeGoBackButtonVisibility(false);
	BackToGameButton->OnClicked.AddDynamic(this, &UWCPP_Pause::BackToGameButtonOnClick);
	SettingsButton->OnClicked.AddDynamic(this, &UWCPP_Pause::SwitchToSettingsPanel);
	GoToMainMenuButton->OnClicked.AddDynamic(this, &UWCPP_Pause::GoToMainMenuButtonOnClick);
	ExitButton->OnClicked.AddDynamic(this, &UWCPP_Pause::ExitButtonOnClick);
	GoBackButton->OnClicked.AddDynamic(this, &UWCPP_Pause::GoBackButtonOnClick);

	if (!GameInstanceRef.IsValid())
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}

	if (!PlayerControllerRef.IsValid())
	{
		if (GameInstanceRef.IsValid() && GameInstanceRef->GetPlayingModeAsInt() == 1 &&
			IsValid(GetOwningLocalPlayer()) && IsValid(GetOwningLocalPlayer()->PlayerController.Get()))
		{
			PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningLocalPlayer()->PlayerController.Get());
		}
		else
		{
			PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
		}
	}
	if (PlayerControllerRef.IsValid())
	{
		bIsGamepadMode = PlayerControllerRef->GetIsGamepadMode();
		if (bIsGamepadMode)
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_Pause::AnyKeyPressed);
		TB_UserName->SetText(FText::FromString(PlayerControllerRef->GetCustomUserName()));
	}
}

void UWCPP_Pause::NativeDestruct()
{
	BackToGameButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::BackToGameButtonOnClick);
	SettingsButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::SwitchToSettingsPanel);
	GoToMainMenuButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::GoToMainMenuButtonOnClick);
	ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::ExitButtonOnClick);
	GoBackButton->OnClicked.RemoveDynamic(this, &UWCPP_Pause::GoBackButtonOnClick);

	DestroySettingsWidget();

	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();
		PlayerControllerRef = nullptr;
	}

	GameInstanceRef = nullptr;

	Super::NativeDestruct();
}

void UWCPP_Pause::BackToGameButtonOnClick()
{
	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->EndPause();
	}
	else if (GetOwningPlayer())
	{
		GetOwningPlayer()->SetPause(false);
	}
	RemoveFromParent();
}

void UWCPP_Pause::SwitchToSettingsPanel()
{
	if (IsValid(SettingsSizeBox))
	{
		InitSettingsWidget();

		if (!SettingsInscription.IsEmpty())
		{
			ChangeUpInscriptionText(SettingsInscription);
		}

		Settings_Widget->bIsWidgetActive = true;
		PanelsWidgetSwitcher->SetActiveWidget(SettingsSizeBox);

		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_Pause::InitSettingsWidget()
{
	if (Settings_Widget == nullptr &&
		PlayerControllerRef.IsValid() &&
		GameInstanceRef.IsValid())
	{
		const FName Row = FName(TEXT("Settings"));
		if (IsValid(WidgetBlueprintsDataTable.LoadSynchronous()))
		{
			if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
				UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
					WidgetBlueprintsDataTable.Get(),
					Row)))
			{
				Settings_Widget = CreateWidget<UWCPP_Settings>(PlayerControllerRef.Get(), Class);
				if (Settings_Widget.IsValid())
				{
					Settings_Widget->SetFlags(RF_StrongRefOnFrame);
					Settings_Widget->bIsMainMenu = false;
					Settings_Widget->SetPlayerControllerRef(PlayerControllerRef.Get());
					Settings_Widget->SetGameInstanceRef(GameInstanceRef.Get());
					SettingsSizeBox->AddChild(Settings_Widget.Get());
				}
			}
		}
	}
}

void UWCPP_Pause::DestroySettingsWidget()
{
	if (Settings_Widget.IsValid())
	{
		Settings_Widget->RemoveFromParent();
		Settings_Widget = nullptr;
	}
}

void UWCPP_Pause::GoToMainMenuButtonOnClick()
{
	EndOnlineSession();
	if (GameInstanceRef.IsValid())
	{
		if (GameInstanceRef->GetPlayingModeAsInt() == 1)
		{
			GameInstanceRef->DestroyAllLocalPlayers();
			GameInstanceRef->SetCurrentSplitscreenType(0);
			GameInstanceRef->ChangeActiveSplitscreenTypeDelegate.Broadcast(0);
		}
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

void UWCPP_Pause::ExitButtonOnClick()
{
	EndOnlineSession();
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

void UWCPP_Pause::EndOnlineSession() const
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
		else if (GameInstanceRef->GetPlayingModeAsInt() == 2 &&
			GameInstanceRef->StartEndingSessionDelegate.IsBound())
		{
			bool bSuccess = GameInstanceRef->StartEndingSessionDelegate.Execute(
				FName(GameInstanceRef->GetCurrentSessionName()));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UWCPP_Pause::EndOnlineSession(), GameInstanceRef is not Valid"));
	}
}

void UWCPP_Pause::GoBackButtonOnClick()
{
	switch (PanelsWidgetSwitcher->GetActiveWidgetIndex())
	{
	case 0: // Main Menu
		return;
	case 1: // Settings
		{
			if (bIsGamepadMode)
			{
				SettingsButton->SetKeyboardFocus();
			}
			DestroySettingsWidget();
			break;
		}
	default:
		if (bIsGamepadMode)
		{
			BackToGameButton->SetKeyboardFocus();
		}
		break;
	}

	PanelsWidgetSwitcher->SetActiveWidgetIndex(0);

	if (!PauseMenuInscription.IsEmpty())
	{
		ChangeUpInscriptionText(PauseMenuInscription);
	}

	ChangeGoBackButtonVisibility(false);
}

void UWCPP_Pause::ChangeGoBackButtonVisibility(const bool bShouldBeVisible) const
{
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(GoBackButton, bShouldBeVisible);

	if (bShouldBeVisible)
	{
		BottomPanel_WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BottomPanel_WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWCPP_Pause::ChangeUpInscriptionText(const FText& NewText) const
{
	TB_UpInscription->SetText(NewText);
}

void UWCPP_Pause::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (PressedKey == EKeys::Gamepad_FaceButton_Right) // B button.
	{
		GoBackButtonOnClick();
	}
}

void UWCPP_Pause::PrepareWidgetForWorkingWithGamepad()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(1);

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_Pause::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (PanelsWidgetSwitcher->GetActiveWidgetIndex() == 0)
	{
		if (PlayerControllerRef.IsValid())
		{
			if (GameInstanceRef.IsValid() && GameInstanceRef->GetPlayingModeAsInt() == 1)
			{
				BackToGameButton->SetKeyboardFocus();
			}
			BackToGameButton->SetUserFocus(PlayerControllerRef.Get());
		}
		else
		{
			BackToGameButton->SetFocus();
		}
	}
}

void UWCPP_Pause::PrepareWidgetForWorkingWithKeyboard()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(0);

	Super::PrepareWidgetForWorkingWithKeyboard();
}
