// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_MainMenu::UWCPP_MainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              WidgetBlueprintsDataTable(nullptr),
                                                                              LoginToEOS_HorizontalBox(nullptr),
                                                                              LoginToEOS_Button(nullptr),
                                                                              TB_EOS_AccountName(nullptr),
                                                                              Gamepad_LoginKey_Image(nullptr),
                                                                              ConfigureLevelParams_Widget(nullptr),
                                                                              CharacterAppearance_Widget(nullptr),
                                                                              Rules_Widget(nullptr),
                                                                              Settings_Widget(nullptr),
                                                                              Statistics_Widget(nullptr),
                                                                              GameInfo_Widget(nullptr),
                                                                              TB_UpInscription(nullptr),
                                                                              PanelsWidgetSwitcher(nullptr),
                                                                              TB_UserName(nullptr),
                                                                              OpenConfigureLevelParamsButton(nullptr),
                                                                              RulesButton(nullptr),
                                                                              CharacterAppearanceButton(nullptr),
                                                                              SettingsButton(nullptr),
                                                                              StatisticsButton(nullptr),
                                                                              InfoButton(nullptr),
                                                                              ChangeSaveSlotButton(nullptr),
                                                                              ExitButton(nullptr),
                                                                              GoBackButton(nullptr),
                                                                              BottomPanel_WidgetSwitcher(nullptr),
                                                                              ConfigureLevelParamsSizeBox(nullptr),
                                                                              RulesSizeBox(nullptr),
                                                                              CharacterAppearanceSizeBox(nullptr),
                                                                              SettingsSizeBox(nullptr),
                                                                              StatisticsSizeBox(nullptr),
                                                                              GameInfoSizeBox(nullptr)
{
}

void UWCPP_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!MainMenuInscription.IsEmpty())
	{
		ChangeUpInscriptionText(MainMenuInscription);
	}

	ChangeGoBackButtonVisibility(false);

	LoginToEOS_Button->OnClicked.AddDynamic(this, &UWCPP_MainMenu::LoginToEOS_Button_OnClick);
	OpenConfigureLevelParamsButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel);
	RulesButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::SwitchToRulesPanel);
	CharacterAppearanceButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::SwitchToCharacterAppearancePanel);
	SettingsButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::SwitchToSettingsPanel);
	StatisticsButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::SwitchToStatisticsPanel);
	InfoButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::SwitchToInfoPanel);
	ChangeSaveSlotButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::ChangeSaveSlotButtonOnClick);
	ExitButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::ExitButtonOnClick);
	GoBackButton->OnClicked.AddDynamic(this, &UWCPP_MainMenu::GoBackButtonOnClick);

	if (!IsValid(GameInstanceRef))
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}
	if (IsValid(GameInstanceRef))
	{
		GetWorld()->GetTimerManager().SetTimer(
			TH_CloseLastOpenedSession,
			[&]()
			{
				if (IsValid(GameInstanceRef) && GameInstanceRef->CloseLastOpenedSessionDelegate.IsBound())
				{
					GameInstanceRef->CloseLastOpenedSessionDelegate.Execute();
				}
				if (IsValid(GetWorld()) && GetWorld()->GetTimerManager().TimerExists(TH_CloseLastOpenedSession))
				{
					GetWorld()->GetTimerManager().ClearTimer(TH_CloseLastOpenedSession);
				}
			},
			5.0f,
			false);
		GameInstanceRef->LoginEndedDelegate.BindUObject(this, &UWCPP_MainMenu::LoginToEOS_WasEnded);
		if (GameInstanceRef->GetEOSAccountNameDelegate.IsBound())
		{
			if (FString AccountName;
				GameInstanceRef->GetEOSAccountNameDelegate.Execute(AccountName))
			{
				TB_EOS_AccountName->SetText(FText::FromString(AccountName));
			}
		}
	}

	if (!IsValid(PlayerControllerRef))
	{
		PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
	}
	if (IsValid(PlayerControllerRef))
	{
		bIsGamepadMode = PlayerControllerRef->GetIsGamepadMode();
		if (bIsGamepadMode)
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_MainMenu::AnyKeyPressed);
		if (!PlayerControllerRef->GetCustomUserName().IsEmpty())
		{
			UserName = FText::FromString(PlayerControllerRef->GetCustomUserName());
			TB_UserName->SetText(UserName);
		}
	}
}

void UWCPP_MainMenu::NativeDestruct()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CloseLastOpenedSession))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CloseLastOpenedSession);
	}

	LoginToEOS_Button->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::LoginToEOS_Button_OnClick);
	OpenConfigureLevelParamsButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel);
	RulesButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToRulesPanel);
	CharacterAppearanceButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToCharacterAppearancePanel);
	SettingsButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToSettingsPanel);
	StatisticsButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToStatisticsPanel);
	InfoButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::SwitchToInfoPanel);
	ChangeSaveSlotButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::ChangeSaveSlotButtonOnClick);
	ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::ExitButtonOnClick);
	GoBackButton->OnClicked.RemoveDynamic(this, &UWCPP_MainMenu::GoBackButtonOnClick);

	DestroyConfigureLevelParamsWidget();
	DestroyRulesWidget();
	DestroyCharacterAppearanceWidget();
	DestroySettingsWidget();
	DestroyStatisticsWidget();
	DestroyInfoWidget();

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();
		PlayerControllerRef = nullptr;
	}
	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->LoginEndedDelegate.Unbind();
		GameInstanceRef = nullptr;
	}

	Super::NativeDestruct();
}

void UWCPP_MainMenu::LoginToEOS_Button_OnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->StartLoginDelegate.IsBound())
	{
		GameInstanceRef->StartLoginDelegate.Execute();
	}
}

void UWCPP_MainMenu::LoginToEOS_WasEnded(const bool bSuccess, const FString& AccountName) const
{
	if (bSuccess)
	{
		TB_EOS_AccountName->SetText(FText::FromString(AccountName));
	}
	else
	{
		TB_EOS_AccountName->SetText(FText());
	}
}

void UWCPP_MainMenu::SwitchToConfigureLevelParamsPanel()
{
	if (IsValid(ConfigureLevelParamsSizeBox))
	{
		if (IsValid(ConfigureLevelParams_Widget))
		{
			if (bIsGamepadMode)
			{
				ConfigureLevelParams_Widget->SetFocusForGamepadMode();
			}
		}
		else
		{
			InitConfigureLevelParamsWidget();
		}

		if (!ConfigureLevelParamsInscription.IsEmpty())
		{
			ChangeUpInscriptionText(ConfigureLevelParamsInscription);
		}

		ConfigureLevelParams_Widget->bIsWidgetActive = true;
		PanelsWidgetSwitcher->SetActiveWidget(ConfigureLevelParamsSizeBox);

		PanelWasSwitched(false);
	}
}

void UWCPP_MainMenu::InitConfigureLevelParamsWidget()
{
	if (ConfigureLevelParams_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("LevelParams"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			ConfigureLevelParams_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(ConfigureLevelParams_Widget))
			{
				ConfigureLevelParams_Widget->SetFlags(RF_StrongRefOnFrame);
				ConfigureLevelParams_Widget->SetGameInstanceRef(GameInstanceRef);
				ConfigureLevelParams_Widget->SetPlayerControllerRef(PlayerControllerRef);
				ConfigureLevelParamsSizeBox->AddChild(ConfigureLevelParams_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyConfigureLevelParamsWidget()
{
	if (IsValid(ConfigureLevelParams_Widget))
	{
		ConfigureLevelParams_Widget->RemoveFromParent();
		ConfigureLevelParams_Widget = nullptr;
	}
}

void UWCPP_MainMenu::SwitchToRulesPanel()
{
	if (IsValid(RulesSizeBox))
	{
		InitRulesWidget();

		if (!RulesInscription.IsEmpty())
		{
			ChangeUpInscriptionText(RulesInscription);
		}

		Rules_Widget->bIsWidgetActive = true;
		PanelsWidgetSwitcher->SetActiveWidget(RulesSizeBox);

		PanelWasSwitched(false);
	}
}

void UWCPP_MainMenu::InitRulesWidget()
{
	if (Rules_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("Rules"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			Rules_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(Rules_Widget))
			{
				Rules_Widget->SetFlags(RF_StrongRefOnFrame);
				Rules_Widget->SetGameInstanceRef(GameInstanceRef);
				Rules_Widget->SetPlayerControllerRef(PlayerControllerRef);
				RulesSizeBox->AddChild(Rules_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyRulesWidget()
{
	if (IsValid(Rules_Widget))
	{
		Rules_Widget->RemoveFromParent();
		Rules_Widget = nullptr;
	}
}

void UWCPP_MainMenu::SwitchToCharacterAppearancePanel()
{
	if (IsValid(CharacterAppearanceSizeBox))
	{
		InitCharacterAppearanceWidget();

		if (!CharacterAppearanceInscription.IsEmpty())
		{
			ChangeUpInscriptionText(CharacterAppearanceInscription);
		}

		CharacterAppearance_Widget->bIsWidgetActive = true;
		PanelsWidgetSwitcher->SetActiveWidget(CharacterAppearanceSizeBox);

		PanelWasSwitched(false);
	}
}

void UWCPP_MainMenu::InitCharacterAppearanceWidget()
{
	if (CharacterAppearance_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("CharacterAppearance"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			CharacterAppearance_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(CharacterAppearance_Widget))
			{
				CharacterAppearance_Widget->SetFlags(RF_StrongRefOnFrame);
				CharacterAppearance_Widget->SetPlayerControllerRef(PlayerControllerRef);
				CharacterAppearance_Widget->SetGameInstanceRef(GameInstanceRef);
				CharacterAppearanceSizeBox->AddChild(CharacterAppearance_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyCharacterAppearanceWidget()
{
	if (IsValid(CharacterAppearance_Widget))
	{
		CharacterAppearance_Widget->RemoveFromParent();
		CharacterAppearance_Widget = nullptr;
	}
}

void UWCPP_MainMenu::SwitchToSettingsPanel()
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

		PanelWasSwitched(false);
	}
}

void UWCPP_MainMenu::InitSettingsWidget()
{
	if (Settings_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("Settings"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			Settings_Widget = CreateWidget<UWCPP_Settings>(PlayerControllerRef, Class);
			if (IsValid(Settings_Widget))
			{
				Settings_Widget->SetFlags(RF_StrongRefOnFrame);
				Settings_Widget->SetPlayerControllerRef(PlayerControllerRef);
				Settings_Widget->SetGameInstanceRef(GameInstanceRef);
				Settings_Widget->bIsMainMenu = true;
				SettingsSizeBox->AddChild(Settings_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroySettingsWidget()
{
	if (IsValid(Settings_Widget))
	{
		Settings_Widget->RemoveFromParent();
		Settings_Widget = nullptr;
	}
}

void UWCPP_MainMenu::SwitchToStatisticsPanel()
{
	if (IsValid(StatisticsSizeBox))
	{
		InitStatisticsWidget();

		if (!StatisticsInscription.IsEmpty())
		{
			ChangeUpInscriptionText(StatisticsInscription);
		}

		Statistics_Widget->bIsWidgetActive = true;
		PanelsWidgetSwitcher->SetActiveWidget(StatisticsSizeBox);

		PanelWasSwitched(false);
	}
}

void UWCPP_MainMenu::InitStatisticsWidget()
{
	if (Statistics_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("Statistics"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			Statistics_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(Statistics_Widget))
			{
				Statistics_Widget->SetFlags(RF_StrongRefOnFrame);
				Statistics_Widget->SetPlayerControllerRef(PlayerControllerRef);
				Statistics_Widget->SetGameInstanceRef(GameInstanceRef);
				StatisticsSizeBox->AddChild(Statistics_Widget);
			}
		}
	}
}

void UWCPP_MainMenu::DestroyStatisticsWidget()
{
	if (IsValid(Statistics_Widget))
	{
		Statistics_Widget->RemoveFromParent();
		Statistics_Widget = nullptr;
	}
}

void UWCPP_MainMenu::SwitchToInfoPanel()
{
	if (IsValid(GameInfoSizeBox))
	{
		InitInfoWidget();

		if (!GameInfoInscription.IsEmpty())
		{
			ChangeUpInscriptionText(GameInfoInscription);
		}

		GameInfo_Widget->bIsWidgetActive = true;
		PanelsWidgetSwitcher->SetActiveWidget(GameInfoSizeBox);

		PanelWasSwitched(false);
	}
}

void UWCPP_MainMenu::InitInfoWidget()
{
	if (GameInfo_Widget == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("GameInfo"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			GameInfo_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(GameInfo_Widget))
			{
				GameInfo_Widget->SetFlags(RF_StrongRefOnFrame);
				GameInfoSizeBox->AddChild(GameInfo_Widget);
			}
		}
	}
	else if (IsValid(GameInfo_Widget) && IsValid(PlayerControllerRef) && PlayerControllerRef->GetIsGamepadMode())
	{
		GameInfo_Widget->SetFocusForGamepadMode();
	}
}

void UWCPP_MainMenu::DestroyInfoWidget()
{
	if (IsValid(GameInfo_Widget))
	{
		GameInfo_Widget->RemoveFromParent();
		GameInfo_Widget = nullptr;
	}
}

void UWCPP_MainMenu::PanelWasSwitched(const bool bCurrentPanelIsMain)
{
	if (bCurrentPanelIsMain)
	{
		//LoginToEOS_HorizontalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChangeGoBackButtonVisibility(false);
	}
	else
	{
		//LoginToEOS_HorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
		ChangeGoBackButtonVisibility(true);
	}
}

void UWCPP_MainMenu::ChangeSaveSlotButtonOnClick()
{
	if (ShouldOpenSaveSlotsPanelDelegate.IsBound())
	{
		ShouldOpenSaveSlotsPanelDelegate.Execute();
	}
}

void UWCPP_MainMenu::ExitButtonOnClick()
{
	const UWorld* World = GetWorld();
	if (PlayerControllerRef)
	{
		if (IsValid(PlayerControllerRef))
		{
			PlayerControllerRef->CallCollectingInfoForSavingItToFile();
		}
		UKismetSystemLibrary::QuitGame(World,
		                               PlayerControllerRef,
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

void UWCPP_MainMenu::GoBackButtonOnClick()
{
	switch (PanelsWidgetSwitcher->GetActiveWidgetIndex())
	{
	case 0: // Main Menu
		return;
	case 1: // Configure Level Params
		{
			if (IsValid(ConfigureLevelParams_Widget))
			{
				ConfigureLevelParams_Widget->bIsWidgetActive = false;
			}
			if (bIsGamepadMode)
			{
				OpenConfigureLevelParamsButton->SetKeyboardFocus();
			}
			break;
		}
	case 2: // Rules
		{
			if (IsValid(Rules_Widget))
			{
				Rules_Widget->bIsWidgetActive = false;
			}
			if (bIsGamepadMode)
			{
				RulesButton->SetKeyboardFocus();
			}
			break;
		}
	case 3: // Character Appearance
		{
			if (IsValid(CharacterAppearance_Widget))
			{
				CharacterAppearance_Widget->bIsWidgetActive = false;
			}
			if (bIsGamepadMode)
			{
				CharacterAppearanceButton->SetKeyboardFocus();
			}
			DestroyCharacterAppearanceWidget();
			break;
		}
	case 4: // Settings
		{
			if (bIsGamepadMode)
			{
				SettingsButton->SetKeyboardFocus();
			}
			DestroySettingsWidget();
			break;
		}
	case 5: // Statistics
		{
			if (bIsGamepadMode)
			{
				StatisticsButton->SetKeyboardFocus();
			}
			DestroyStatisticsWidget();
			break;
		}
	case 6: // Game Info
		{
			if (IsValid(GameInfo_Widget))
			{
				GameInfo_Widget->bIsWidgetActive = false;
			}
			if (bIsGamepadMode)
			{
				InfoButton->SetKeyboardFocus();
			}
			break;
		}
	default:
		{
			if (bIsGamepadMode)
			{
				OpenConfigureLevelParamsButton->SetKeyboardFocus();
			}
			break;
		}
	}

	if (UserName.IsEmpty() && IsValid(PlayerControllerRef) && !PlayerControllerRef->GetCustomUserName().IsEmpty())
	{
		UserName = FText::FromString(PlayerControllerRef->GetCustomUserName());
		TB_UserName->SetText(UserName);
	}

	PanelsWidgetSwitcher->SetActiveWidgetIndex(0);

	if (!MainMenuInscription.IsEmpty())
	{
		ChangeUpInscriptionText(MainMenuInscription);
	}

	PanelWasSwitched(true);
}

void UWCPP_MainMenu::ChangeGoBackButtonVisibility(const bool bShouldBeVisible) const
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

void UWCPP_MainMenu::ChangeUpInscriptionText(const FText& NewText) const
{
	TB_UpInscription->SetText(NewText);
}

void UWCPP_MainMenu::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (PressedKey == EKeys::Gamepad_FaceButton_Right)
	{
		GoBackButtonOnClick();
	}
	else if (PressedKey == EKeys::Gamepad_Special_Right)
	{
		LoginToEOS_Button_OnClick();
	}
}

void UWCPP_MainMenu::PrepareWidgetForWorkingWithGamepad()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(1);
	Gamepad_LoginKey_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_MainMenu::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (PanelsWidgetSwitcher->GetActiveWidgetIndex() == 0)
	{
		OpenConfigureLevelParamsButton->SetKeyboardFocus();
	}
}

void UWCPP_MainMenu::PrepareWidgetForWorkingWithKeyboard()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(0);
	Gamepad_LoginKey_Image->SetVisibility(ESlateVisibility::Collapsed);

	Super::PrepareWidgetForWorkingWithKeyboard();
}
