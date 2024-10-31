// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_ConfigureLevelParams.h"
#include "Components/CanvasPanel.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/ComboBoxString.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CircularThrobber.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetMathLibrary.h"

#ifndef WCPP_PUBLICSESSIONINFO_H
#define WCPP_PUBLICSESSIONINFO_H
#include "CatPlatformer/UI/Classes/WCPP_PublicSessionInfo.h"
#endif
class UWCPP_PublicSessionInfo;

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_ConfigureLevelParams::UWCPP_ConfigureLevelParams(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	WidgetBlueprintsDataTable(nullptr),
	PanelsSwitcher(nullptr), PanelsNumber(0),
	BottomPanel_WidgetSwitcher(nullptr),
	OpenPreviousPanelButton(nullptr),
	LB_Image(nullptr), OpenNextPanelButton(nullptr), RB_Image(nullptr),
	EnterPlayerName_CanvasPanel(nullptr),
	InputName_EditableText(nullptr),
	OnOffVirtualKeyboard_EnterName_HorizontalBox(nullptr),
	VirtualKeyboardSizeBox(nullptr), OnOffVirtualKeyboard_EnterName_TB(nullptr),
	VirtualKeyboardRef(nullptr),
	ChoosePlayingMode_CanvasPanel(nullptr), CurrentPlayingMode(EPlayingMode::SinglePlayer),
	SingleMode_CheckBox(nullptr), LocalMultiplayerMode_CheckBox(nullptr),
	OnlineMultiplayerMode_CheckBox(nullptr), ChooseLocalPlayers_CanvasPanel(nullptr),
	LocalPlayers_WidgetSwitcher(nullptr), UseKeyboardForFirstPlayer_CheckBox(nullptr),
	ChangeLocalPlayersNumber_WidgetSwitcher(nullptr),
	DecrementLocalPlayersNumberButton(nullptr), IncrementLocalPlayersNumberButton(nullptr),
	SplitscreenType(0),
	SplitType1_CheckBox(nullptr),
	SplitType2_CheckBox(nullptr),
	SplitType3_CheckBox(nullptr),
	SplitType4_CheckBox(nullptr),
	SplitType5_CheckBox(nullptr),
	SplitType6_CheckBox(nullptr),
	SplitType7_CheckBox(nullptr),
	SplitType8_CheckBox(nullptr),
	SplitType9_CheckBox(nullptr),
	NumberOfPlayers(1),
	bShouldFirstLocalPlayerUseKeyboard(true),
	ChooseOnlineMultiplayerPage_CanvasPanel(nullptr),
	OpenPublicSessionsPanelButton(nullptr),
	OpenPrivateSessionPanelButton(nullptr),
	OpenCreateSessionPanelButton(nullptr),
	PublicSessions_CanvasPanel(nullptr),
	FindPublicSessionsButton(nullptr),
	PublicSessionsScrollBox(nullptr),
	FindSessionsOnLAN_CheckBox(nullptr),
	FindingSessionsThrobberSizeBox(nullptr),
	ConnectToPublicSessionButton(nullptr),
	JoinToPublicSessionHorizontalBox(nullptr),
	JoinToPublicSessionThrobberSizeBox(nullptr),
	CancelFindingPublicSessionsButton(nullptr),
	X_Key_ConnectToPublicSession_Image(nullptr),
	SessionNameToJoin_TB(nullptr),
	ChosenPublicSessionNumber(-1),
	bIsJoiningToPublicServer(false),
	PrivateSession_CanvasPanel(nullptr),
	FindPrivateSessionOnLAN_CheckBox(nullptr), InputCode_EditableText(nullptr),
	ConnectToPrivateSessionButton(nullptr),
	CancelFindingPrivateSessionButton(nullptr),
	JoinToPrivateSessionHorizontalBox(nullptr),
	JoinToPrivateSessionThrobberSizeBox(nullptr),
	OnOffVirtualKeyboard_PrivateSession_HorizontalBox(nullptr),
	OnOffVirtualKeyboard_PrivateSession_TB(nullptr),
	bIsJoiningToPrivateSession(false), CreateSession_CanvasPanel(nullptr),
	MaxPlayersNumOnServer_ComboBox(nullptr),
	SessionShouldBePublic_CheckBox(nullptr),
	CreateSessionForLANConnectionOnly_CheckBox(nullptr),
	ChooseLevel_CanvasPanel(nullptr), StartLevel1_Button(nullptr),
	StartLevel2_Button(nullptr),
	StartLevel3_Button(nullptr),
	StartLevel4_Button(nullptr),
	StartLevel5_Button(nullptr),
	StartRandomLevel_Button(nullptr),
	ChosenLevelNumber(0)
{
}

void UWCPP_ConfigureLevelParams::NativeConstruct()
{
	Super::NativeConstruct();

	// For some strange reason, when calling this function
	// for the first time, 0 is always returned, and the
	// correct values begin to return only with consequent
	// calls. Therefore, we call this function once in idle
	// mode when creating a widget.
	UCPP_StaticLibrary::GetConnectedGamepadsNumber();

	PanelsNumber = PanelsSwitcher->GetNumWidgets();

	if (!IsValid(GameInstanceRef))
	{
		GameInstanceRef = GetGameInstance<UCPP_GameInstance>();
	}
	if (!IsValid(PlayerControllerRef))
	{
		PlayerControllerRef = GetOwningPlayer<ACPP_PlayerController>();
	}
	if (IsValid(PlayerControllerRef))
	{
		bIsGamepadMode = PlayerControllerRef->GetIsGamepadMode();

		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_ConfigureLevelParams::AnyKeyPressed);
		DH_GamepadScroll = PlayerControllerRef->GamepadScrollDelegate.
		                                        AddUObject(this, &UWCPP_ConfigureLevelParams::GamepadScrollCalled);

		if (PlayerControllerRef->GetMaxOpenedLevelNumber() > 1)
		{
			for (int32 i = 2; i <= PlayerControllerRef->GetMaxOpenedLevelNumber(); i++)
			{
				ChangeLevelImageToOpened(i);
			}
		}
	}

	DH_ControllerConnectionChanging = IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddUObject(
		this,
		&UWCPP_ConfigureLevelParams::OnControllerConnectionChanged);

	OpenNextPanelButton->OnClicked.AddDynamic(this, &UWCPP_ConfigureLevelParams::RightArrowOnClick);
	OpenPreviousPanelButton->OnClicked.AddDynamic(this, &UWCPP_ConfigureLevelParams::LeftArrowOnClick);

	UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenPreviousPanelButton, ESlateVisibility::Collapsed);
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenPreviousPanelButton, false);
	UCPP_StaticWidgetLibrary::ChangeImageVisibility(LB_Image, ESlateVisibility::Collapsed);

	if (IsValid(PlayerControllerRef) && !PlayerControllerRef->GetCustomUserName().IsEmpty())
	{
		PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);
	}
	else
	{
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(EnterNicknameRulesInscription, 5.5f);
		}
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
	}

	//=========First panel (EnterPlayerName_CanvasPanel)=========

	InputName_EditableText->OnTextChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::InputNameChanged);
	InputName_EditableText->OnTextCommitted.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::InputNameCommitted);
	OnOffVirtualKeyboard_EnterName_TB->SetText(OnVirtualKeyboardInscription);

	//========Second panel (ChoosePlayingMode_CanvasPanel)=========

	SingleMode_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SingleModeOnCheckStateChanged);
	LocalMultiplayerMode_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::LocalMultiplayerModeOnCheckStateChanged);
	OnlineMultiplayerMode_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::OnlineMultiplayerModeOnCheckStateChanged);

	SingleMode_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	SingleModeOnCheckStateChanged(true);

	if (UCPP_StaticLibrary::IsAnyGamepadConnected())
	{
		ChangeLocalMultiplayerCheckBoxEnabling(true);
	}
	else
	{
		ChangeLocalMultiplayerCheckBoxEnabling(false);
	}

	//=========Third panel (ChooseLocalPlayers_CanvasPanel)========

	DecrementLocalPlayersNumberButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::DecrementLocalPlayersNumberButtonOnClick);
	IncrementLocalPlayersNumberButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::IncrementLocalPlayersNumberButtonOnClick);
	UseKeyboardForFirstPlayer_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::UseKeyboardForFirstPlayerOnCheckStateChanged);
	SplitType1_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType1_CheckBox_OnStateChanged);
	SplitType2_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType2_CheckBox_OnStateChanged);
	SplitType3_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType3_CheckBox_OnStateChanged);
	SplitType4_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType4_CheckBox_OnStateChanged);
	SplitType5_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType5_CheckBox_OnStateChanged);
	SplitType6_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType6_CheckBox_OnStateChanged);
	SplitType7_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType7_CheckBox_OnStateChanged);
	SplitType8_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType8_CheckBox_OnStateChanged);
	SplitType9_CheckBox->OnCheckStateChanged.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType9_CheckBox_OnStateChanged);

	//===Fourth panel (ChooseOnlineMultiplayerPage_CanvasPanel)====

	OpenPublicSessionsPanelButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::OpenPublicSessionsPanelButtonOnClick);
	OpenPrivateSessionPanelButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::OpenPrivateSessionPanelButtonOnClick);
	OpenCreateSessionPanelButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::OpenCreateSessionPanelButtonOnClick);

	//==========Fifth panel (PublicSessions_CanvasPanel)===========

	if (IsValid(GameInstanceRef))
	{
		DH_FindingSessionsCompleted = GameInstanceRef->FindingSessionsCompletedDelegate.AddUObject(
			this, &UWCPP_ConfigureLevelParams::FindingSessionsCompleted);

		GameInstanceRef->JoiningSessionFailedDelegate.BindUObject(
			this, &UWCPP_ConfigureLevelParams::JoiningToSessionFailed);
	}
	FindPublicSessionsButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::FindPublicSessionsButtonOnClick);
	ConnectToPublicSessionButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::ConnectToPublicSessionButtonOnClick);
	CancelFindingPublicSessionsButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::CancelFindingPublicSessionsButtonOnClick);

	JoinToPublicSessionHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CancelFindingPublicSessionsButton, false);
	UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(CancelFindingPublicSessionsButton,
	                                                  ESlateVisibility::Collapsed);

	//==========Sixth panel (PrivateSession_CanvasPanel)===========

	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->FindingPrivateSessionFailedDelegate.BindUObject(
			this, &UWCPP_ConfigureLevelParams::OnFindingPrivateSessionFailed);
	}
	ConnectToPrivateSessionButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::ConnectToPrivateSessionButtonOnClick);
	CancelFindingPrivateSessionButton->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::CancelFindingPrivateSessionButtonOnClick);
	JoinToPrivateSessionHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	InputCode_EditableText->OnTextCommitted.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::InputCodeCommitted);
	OnOffVirtualKeyboard_PrivateSession_TB->SetText(OnVirtualKeyboardInscription);

	//=========Seventh panel (CreateSession_CanvasPanel)===========

	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->SessionCreationFailedDelegate.BindUObject(
			this, &UWCPP_ConfigureLevelParams::SessionCreationFailed);
	}

	//============Eighth panel (ChooseLevel_CanvasPanel)===========

	StartLevel1_Button->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel1_ButtonOnClick);
	StartLevel2_Button->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel2_ButtonOnClick);
	StartLevel3_Button->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel3_ButtonOnClick);
	StartLevel4_Button->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel4_ButtonOnClick);
	StartLevel5_Button->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel5_ButtonOnClick);
	StartRandomLevel_Button->OnClicked.AddDynamic(
		this, &UWCPP_ConfigureLevelParams::StartRandomLevel_ButtonOnClick);

	//====================Gamepad Mode=============================

	if (bIsGamepadMode)
	{
		PrepareWidgetForWorkingWithGamepad();
	}
	else
	{
		PrepareWidgetForWorkingWithKeyboard();
	}
}

void UWCPP_ConfigureLevelParams::NativeDestruct()
{
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		PlayerControllerRef->GamepadScrollDelegate.Remove(DH_GamepadScroll);
		PlayerControllerRef = nullptr;
	}

	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().Remove(DH_ControllerConnectionChanging);
	DH_ControllerConnectionChanging.Reset();

	OpenNextPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_ConfigureLevelParams::RightArrowOnClick);
	OpenPreviousPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_ConfigureLevelParams::LeftArrowOnClick);

	//=========First panel (EnterPlayerName_CanvasPanel)=========

	DestroyVirtualKeyboard();
	InputName_EditableText->OnTextChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::InputNameChanged);
	InputName_EditableText->OnTextCommitted.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::InputNameCommitted);

	//========Second panel (ChoosePlayingMode_CanvasPanel)=========

	SingleMode_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SingleModeOnCheckStateChanged);
	LocalMultiplayerMode_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::LocalMultiplayerModeOnCheckStateChanged);
	OnlineMultiplayerMode_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::OnlineMultiplayerModeOnCheckStateChanged);

	//=========Third panel (ChooseLocalPlayers_CanvasPanel)========

	DecrementLocalPlayersNumberButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::DecrementLocalPlayersNumberButtonOnClick);
	IncrementLocalPlayersNumberButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::IncrementLocalPlayersNumberButtonOnClick);
	UseKeyboardForFirstPlayer_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::UseKeyboardForFirstPlayerOnCheckStateChanged);
	SplitType1_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType1_CheckBox_OnStateChanged);
	SplitType2_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType2_CheckBox_OnStateChanged);
	SplitType3_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType3_CheckBox_OnStateChanged);
	SplitType4_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType4_CheckBox_OnStateChanged);
	SplitType5_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType5_CheckBox_OnStateChanged);
	SplitType6_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType6_CheckBox_OnStateChanged);
	SplitType7_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType7_CheckBox_OnStateChanged);
	SplitType8_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType8_CheckBox_OnStateChanged);
	SplitType9_CheckBox->OnCheckStateChanged.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::SplitType9_CheckBox_OnStateChanged);

	//===Fourth panel (ChooseOnlineMultiplayerPage_CanvasPanel)====

	OpenPublicSessionsPanelButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::OpenPublicSessionsPanelButtonOnClick);
	OpenPrivateSessionPanelButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::OpenPrivateSessionPanelButtonOnClick);
	OpenCreateSessionPanelButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::OpenCreateSessionPanelButtonOnClick);

	//==========Fifth panel (PublicSessions_CanvasPanel)===========

	FindPublicSessionsButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::FindPublicSessionsButtonOnClick);
	ConnectToPublicSessionButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::ConnectToPublicSessionButtonOnClick);
	CancelFindingPublicSessionsButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::CancelFindingPublicSessionsButtonOnClick);

	if (GetWorld()->GetTimerManager().TimerExists(TH_ClearFindingPublicSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ClearFindingPublicSessionUnusedWidgets);
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_ClearJoiningToPublicSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ClearJoiningToPublicSessionUnusedWidgets);
	}

	//==========Sixth panel (PrivateSession_CanvasPanel)===========

	ConnectToPrivateSessionButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::ConnectToPrivateSessionButtonOnClick);
	CancelFindingPrivateSessionButton->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::CancelFindingPrivateSessionButtonOnClick);
	InputCode_EditableText->OnTextCommitted.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::InputCodeCommitted);

	if (GetWorld()->GetTimerManager().TimerExists(TH_FindingAndJoiningToPrivateSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets);
	}

	//=========Seventh panel (CreateSession_CanvasPanel)===========

	//============Eighth panel (ChooseLevel_CanvasPanel)===========

	StartLevel1_Button->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel1_ButtonOnClick);
	StartLevel2_Button->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel2_ButtonOnClick);
	StartLevel3_Button->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel3_ButtonOnClick);
	StartLevel4_Button->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel4_ButtonOnClick);
	StartLevel5_Button->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::StartLevel5_ButtonOnClick);
	StartRandomLevel_Button->OnClicked.RemoveDynamic(
		this, &UWCPP_ConfigureLevelParams::StartRandomLevel_ButtonOnClick);

	if (IsValid(GameInstanceRef))
	{
		GameInstanceRef->JoiningSessionFailedDelegate.Unbind();
		GameInstanceRef->SessionCreationFailedDelegate.Unbind();
		GameInstanceRef->FindingSessionsCompletedDelegate.Remove(DH_FindingSessionsCompleted);
		DH_FindingSessionsCompleted.Reset();
		GameInstanceRef->FindingPrivateSessionFailedDelegate.Unbind();
		GameInstanceRef = nullptr;
	}

	Super::NativeDestruct();
}

void UWCPP_ConfigureLevelParams::RightArrowOnClick()
{
	if (PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		FString Name = InputName_EditableText->GetText().ToString();
		if (!CheckNameCorrectness(Name))
		{
			if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
			{
				PlayerControllerRef->CreateNotificationDelegate.Execute(EnterNicknameRulesInscription, 5.5f);
			}
			return;
		}
		if (IsValid(PlayerControllerRef))
		{
			PlayerControllerRef->SetCustomUserName(Name);
			PlayerControllerRef->CallCollectingInfoForSavingItToFile();
		}
		PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);
		DestroyVirtualKeyboard();
	}
	else if (PanelsSwitcher->GetActiveWidget() == ChoosePlayingMode_CanvasPanel)
	{
		if (CurrentPlayingMode == EPlayingMode::SinglePlayer)
		{
			PanelsSwitcher->SetActiveWidget(ChooseLevel_CanvasPanel);
			UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
			UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
			UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
		}
		else if (CurrentPlayingMode == EPlayingMode::LocalMultiplayer)
		{
			PanelsSwitcher->SetActiveWidget(ChooseLocalPlayers_CanvasPanel);
		}
		else
		{
			PanelsSwitcher->SetActiveWidget(ChooseOnlineMultiplayerPage_CanvasPanel);
			UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
			UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
			UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
		}

		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenPreviousPanelButton, ESlateVisibility::Visible);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenPreviousPanelButton, true);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(LB_Image, ESlateVisibility::SelfHitTestInvisible);
	}
	else if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel)
	{
		PanelsSwitcher->SetActiveWidget(ChooseLevel_CanvasPanel);
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == CreateSession_CanvasPanel)
	{
		PanelsSwitcher->SetActiveWidget(ChooseLevel_CanvasPanel);
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
	}

	if (bIsGamepadMode)
	{
		SetFocusForGamepadMode();
	}
}

void UWCPP_ConfigureLevelParams::LeftArrowOnClick()
{
	if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel ||
		PanelsSwitcher->GetActiveWidget() == ChooseOnlineMultiplayerPage_CanvasPanel)
	{
		PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);

		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenPreviousPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenPreviousPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(LB_Image, ESlateVisibility::Collapsed);

		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Visible);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, true);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::SelfHitTestInvisible);
	}
	else if (PanelsSwitcher->GetActiveWidget() == PublicSessions_CanvasPanel ||
		PanelsSwitcher->GetActiveWidget() == CreateSession_CanvasPanel)
	{
		PanelsSwitcher->SetActiveWidget(ChooseOnlineMultiplayerPage_CanvasPanel);

		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
	{
		PanelsSwitcher->SetActiveWidget(ChooseOnlineMultiplayerPage_CanvasPanel);

		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);

		DestroyVirtualKeyboard();
	}
	else if (PanelsSwitcher->GetActiveWidget() == ChooseLevel_CanvasPanel)
	{
		if (CurrentPlayingMode == EPlayingMode::SinglePlayer)
		{
			PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);

			UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenPreviousPanelButton, ESlateVisibility::Collapsed);
			UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenPreviousPanelButton, false);
			UCPP_StaticWidgetLibrary::ChangeImageVisibility(LB_Image, ESlateVisibility::Collapsed);
		}
		else if (CurrentPlayingMode == EPlayingMode::LocalMultiplayer)
		{
			PanelsSwitcher->SetActiveWidget(ChooseLocalPlayers_CanvasPanel);
		}
		else
		{
			PanelsSwitcher->SetActiveWidget(CreateSession_CanvasPanel);
		}
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Visible);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, true);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::SelfHitTestInvisible);
	}

	if (bIsGamepadMode)
	{
		SetFocusForGamepadMode();
	}
}

void UWCPP_ConfigureLevelParams::InitVirtualKeyboard()
{
	if (PanelsSwitcher->GetActiveWidget() != EnterPlayerName_CanvasPanel &&
		PanelsSwitcher->GetActiveWidget() != PrivateSession_CanvasPanel)
		return;

	if (VirtualKeyboardRef == nullptr &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		const FName Row = FName(TEXT("VirtualKeyboard"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			VirtualKeyboardRef = CreateWidget<UWCPP_VirtualKeyboard>(PlayerControllerRef, Class);
			if (IsValid(VirtualKeyboardRef))
			{
				OnOffVirtualKeyboard_EnterName_TB->SetText(OffVirtualKeyboardInscription);
				OnOffVirtualKeyboard_PrivateSession_TB->SetText(OffVirtualKeyboardInscription);

				VirtualKeyboardRef->SetFlags(RF_StrongRefOnFrame);
				VirtualKeyboardRef->SetGameInstanceRef(GameInstanceRef);
				VirtualKeyboardRef->SetPlayerControllerRef(PlayerControllerRef);
				DH_TypeSymbol = VirtualKeyboardRef->TypeSymbolDelegate.AddUObject(
					this, &UWCPP_ConfigureLevelParams::SymbolWasTypedByVirtualKeyboard);
				DH_RemoveSymbol = VirtualKeyboardRef->RemoveSymbolDelegate.AddUObject(
					this, &UWCPP_ConfigureLevelParams::SymbolWasRemovedByVirtualKeyboard);
				DH_EnterWasPressed = VirtualKeyboardRef->EnterWasPressedDelegate.AddUObject(
					this, &UWCPP_ConfigureLevelParams::EnterWasPressedByVirtualKeyboard);
				DH_ClosingKeyboard = VirtualKeyboardRef->ClosingKeyboardDelegate.AddUObject(
					this, &UWCPP_ConfigureLevelParams::DestroyVirtualKeyboard);
				VirtualKeyboardSizeBox->AddChild(VirtualKeyboardRef);
				VirtualKeyboardRef->Btn_Digit_1->SetKeyboardFocus();
			}
		}
	}
}

void UWCPP_ConfigureLevelParams::DestroyVirtualKeyboard()
{
	if (IsValid(VirtualKeyboardRef))
	{
		OnOffVirtualKeyboard_EnterName_TB->SetText(OnVirtualKeyboardInscription);
		OnOffVirtualKeyboard_PrivateSession_TB->SetText(OnVirtualKeyboardInscription);

		VirtualKeyboardRef->TypeSymbolDelegate.Remove(DH_TypeSymbol);
		DH_TypeSymbol.Reset();
		VirtualKeyboardRef->RemoveSymbolDelegate.Remove(DH_RemoveSymbol);
		DH_RemoveSymbol.Reset();
		VirtualKeyboardRef->EnterWasPressedDelegate.Remove(DH_EnterWasPressed);
		DH_EnterWasPressed.Reset();
		VirtualKeyboardRef->ClosingKeyboardDelegate.Remove(DH_ClosingKeyboard);
		DH_ClosingKeyboard.Reset();
		VirtualKeyboardRef->RemoveFromParent();
		VirtualKeyboardRef = nullptr;

		SetFocusForGamepadMode();
	}
}

void UWCPP_ConfigureLevelParams::SymbolWasTypedByVirtualKeyboard(const FText& Symbol)
{
	if (PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		FString Str = InputName_EditableText->GetText().ToString();
		Str += Symbol.ToString();
		const FText FinalText = FText::FromString(Str);
		InputName_EditableText->SetText(FinalText);
		InputNameChanged(FinalText);
	}
	else if (PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
	{
		FString Str = InputCode_EditableText->GetText().ToString();
		Str += Symbol.ToString();
		const FText FinalText = FText::FromString(Str);
		InputCode_EditableText->SetText(FinalText);
	}
}

void UWCPP_ConfigureLevelParams::SymbolWasRemovedByVirtualKeyboard()
{
	if (PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		FString Str = InputName_EditableText->GetText().ToString();
		const int32 StrLen = GetNum(Str);
		if (StrLen == 0)
			return;
		Str.RemoveAt(StrLen - 1);
		const FText FinalText = FText::FromString(Str);
		InputName_EditableText->SetText(FinalText);
		InputNameChanged(FinalText);
	}
	else if (PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
	{
		FString Str = InputCode_EditableText->GetText().ToString();
		const int32 StrLen = GetNum(Str);
		if (StrLen == 0)
			return;
		Str.RemoveAt(StrLen - 1);
		const FText FinalText = FText::FromString(Str);
		InputCode_EditableText->SetText(FinalText);
	}
}

void UWCPP_ConfigureLevelParams::EnterWasPressedByVirtualKeyboard() const
{
	if (PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		InputName_EditableText->OnTextCommitted.Broadcast(InputName_EditableText->GetText(),
		                                                  ETextCommit::OnEnter);
	}
	else if (PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
	{
		InputCode_EditableText->OnTextCommitted.Broadcast(InputCode_EditableText->GetText(),
		                                                  ETextCommit::OnEnter);
	}
}

bool UWCPP_ConfigureLevelParams::CheckNameCorrectness(FString Str)
{
	bool bNameIsValid = true;
	Str.TrimStartAndEndInline();

	if (Str.IsEmpty() || Str.Len() <= 1 || Str.Len() > 13)
	{
		bNameIsValid = false;
	}
	else
	{
		for (int32 i = 0; i < Str.Len(); i++)
		{
			if (const int32 Code = Str[i])
			{
				if (Code >= 48 && Code <= 57 || // 0 - 9
					Code >= 65 && Code <= 90 || // A - Z
					Code >= 97 && Code <= 122 || // a - z
					Code == 1025 || Code == 1105 || // Ё, ё
					Code >= 1040 && Code <= 1103) // А - я
				{
					bNameIsValid = true;
				}
				else
				{
					bNameIsValid = false;
					break;
				}
			}
		}
	}
	ChangeNameCorrectnessImage(bNameIsValid);
	return bNameIsValid;
}

void UWCPP_ConfigureLevelParams::InputNameChanged(const FText& NewText)
{
	if (CheckNameCorrectness(NewText.ToString()))
	{
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, true);
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Visible);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, false);
		UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Collapsed);
		UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::Collapsed);
	}
}

void UWCPP_ConfigureLevelParams::InputNameCommitted(const FText& NewText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		RightArrowOnClick();
	}
}

void UWCPP_ConfigureLevelParams::ChangeLocalMultiplayerCheckBoxEnabling(const bool bShouldBeEnabled)
{
	LocalMultiplayerMode_CheckBox->SetIsEnabled(bShouldBeEnabled);
	if (bShouldBeEnabled)
	{
		LocalMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		if (LocalMultiplayerMode_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			SingleMode_CheckBox->SetCheckedState(ECheckBoxState::Checked);
			SingleModeOnCheckStateChanged(true);
		}
		LocalMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Undetermined);
	}
}

void UWCPP_ConfigureLevelParams::SingleModeOnCheckStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 0;
		NumberOfPlayers = 1;
		CurrentPlayingMode = EPlayingMode::SinglePlayer;
		if (LocalMultiplayerMode_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			LocalMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		}
		OnlineMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SingleMode_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::LocalMultiplayerModeOnCheckStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 1;
		NumberOfPlayers = 2;
		CurrentPlayingMode = EPlayingMode::LocalMultiplayer;
		SingleMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		OnlineMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		LocalMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::OnlineMultiplayerModeOnCheckStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 0;
		NumberOfPlayers = 1;
		CurrentPlayingMode = EPlayingMode::OnlineMultiplayerClientInPublicSession;
		SingleMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		if (LocalMultiplayerMode_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			LocalMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}
	else
	{
		OnlineMultiplayerMode_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::DecrementLocalPlayersNumberButtonOnClick()
{
	int32 AvailablePanelsNumber;
	if (UseKeyboardForFirstPlayer_CheckBox->IsChecked())
	{
		AvailablePanelsNumber = UKismetMathLibrary::Min(3,
		                                                UCPP_StaticLibrary::GetConnectedGamepadsNumber());
	}
	else
	{
		AvailablePanelsNumber = UKismetMathLibrary::Min(3,
		                                                UCPP_StaticLibrary::GetConnectedGamepadsNumber() - 1);
	}
	if (AvailablePanelsNumber < 1)
	{
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(AllGamepadsWereDisconnectedInscription, 3.5f);
		}
		PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);
		return;
	}

	int32 IndexToSet;
	if (LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() < AvailablePanelsNumber - 1 ||
		LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() == 0)
	{
		IndexToSet = AvailablePanelsNumber - 1;
	}
	else
	{
		IndexToSet = LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() - 1;
	}
	LocalPlayers_WidgetSwitcher->SetActiveWidgetIndex(IndexToSet);
	NumberOfPlayers = IndexToSet + 2;

	UpdateCurrentSplitscreenType(IndexToSet);
}

void UWCPP_ConfigureLevelParams::IncrementLocalPlayersNumberButtonOnClick()
{
	int32 AvailablePanelsNumber;
	if (UseKeyboardForFirstPlayer_CheckBox->IsChecked())
	{
		AvailablePanelsNumber = UKismetMathLibrary::Min(3,
		                                                UCPP_StaticLibrary::GetConnectedGamepadsNumber());
	}
	else
	{
		AvailablePanelsNumber = UKismetMathLibrary::Min(3,
		                                                UCPP_StaticLibrary::GetConnectedGamepadsNumber() - 1);
	}
	if (AvailablePanelsNumber < 1)
	{
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(AllGamepadsWereDisconnectedInscription, 3.5f);
		}
		PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);
		return;
	}

	int32 IndexToSet;
	if (LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() + 1 >= AvailablePanelsNumber)
	{
		IndexToSet = 0;
	}
	else
	{
		IndexToSet = LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() + 1;
	}
	LocalPlayers_WidgetSwitcher->SetActiveWidgetIndex(IndexToSet);
	NumberOfPlayers = IndexToSet + 2;

	UpdateCurrentSplitscreenType(IndexToSet);
}

void UWCPP_ConfigureLevelParams::UpdateCurrentSplitscreenType(const int32 CurrentPanelIndex)
{
	switch (CurrentPanelIndex)
	{
	case 0:
		if (SplitType1_CheckBox->IsChecked())
		{
			SplitscreenType = 1;
		}
		else
		{
			SplitscreenType = 2;
		}
		break;
	case 1:
		if (SplitType3_CheckBox->IsChecked())
		{
			SplitscreenType = 3;
		}
		else if (SplitType4_CheckBox->IsChecked())
		{
			SplitscreenType = 4;
		}
		else if (SplitType5_CheckBox->IsChecked())
		{
			SplitscreenType = 5;
		}
		else
		{
			SplitscreenType = 6;
		}
		break;
	case 2:
		if (SplitType7_CheckBox->IsChecked())
		{
			SplitscreenType = 7;
		}
		else if (SplitType8_CheckBox->IsChecked())
		{
			SplitscreenType = 8;
		}
		else
		{
			SplitscreenType = 9;
		}
		break;
	default:
		break;
	}
}

void UWCPP_ConfigureLevelParams::UseKeyboardForFirstPlayerOnCheckStateChanged(const bool bNewState)
{
	const int32 AvailablePanelsNumber = UKismetMathLibrary::Min(3,
	                                                            UCPP_StaticLibrary::GetConnectedGamepadsNumber() - 1);
	if (bNewState || AvailablePanelsNumber < 1)
	{
		UseKeyboardForFirstPlayer_CheckBox->SetCheckedState(ECheckBoxState::Checked);
		UCPP_StaticLibrary::MakeFirstLocalPlayerUseKeyboard();
		bShouldFirstLocalPlayerUseKeyboard = true;
		ChangeInputDevicesImages(true);
	}
	else
	{
		if (LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() > AvailablePanelsNumber - 1)
		{
			LocalPlayers_WidgetSwitcher->SetActiveWidgetIndex(AvailablePanelsNumber - 1);
			LocalPlayers_WidgetSwitcher->SetActiveWidgetIndex(AvailablePanelsNumber - 1);
			NumberOfPlayers = AvailablePanelsNumber + 1;
			UpdateCurrentSplitscreenType(AvailablePanelsNumber - 1);
		}
		UCPP_StaticLibrary::MakeAllLocalPlayersUseGamepads();
		bShouldFirstLocalPlayerUseKeyboard = false;
		ChangeInputDevicesImages(false);
	}
}

void UWCPP_ConfigureLevelParams::SplitType1_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 1;
		SplitType2_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType1_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType2_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 2;
		SplitType1_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType2_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType3_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 3;
		SplitType4_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType5_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType6_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType3_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType4_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 4;
		SplitType3_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType5_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType6_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType4_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType5_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 5;
		SplitType3_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType4_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType6_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType5_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType6_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 6;
		SplitType3_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType4_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType5_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType6_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType7_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 7;
		SplitType8_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType9_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType7_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType8_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 8;
		SplitType7_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType9_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType8_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::SplitType9_CheckBox_OnStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		SplitscreenType = 9;
		SplitType7_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		SplitType8_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		SplitType9_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_ConfigureLevelParams::OpenPublicSessionsPanelButtonOnClick()
{
	CurrentPlayingMode = EPlayingMode::OnlineMultiplayerClientInPublicSession;

	PanelsSwitcher->SetActiveWidget(PublicSessions_CanvasPanel);

	if (bIsGamepadMode)
	{
		SetFocusForGamepadMode();
	}
}

void UWCPP_ConfigureLevelParams::OpenPrivateSessionPanelButtonOnClick()
{
	CurrentPlayingMode = EPlayingMode::OnlineMultiplayerClientInPrivateSession;

	PanelsSwitcher->SetActiveWidget(PrivateSession_CanvasPanel);

	if (bIsGamepadMode)
	{
		SetFocusForGamepadMode();
	}
}

void UWCPP_ConfigureLevelParams::OpenCreateSessionPanelButtonOnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->IsPlayerLoggedInDelegate.IsBound())
	{
		if (!GameInstanceRef->IsPlayerLoggedInDelegate.Execute())
		{
			if (GameInstanceRef->StartLoginDelegate.IsBound())
			{
				GameInstanceRef->StartLoginDelegate.Execute();
			}
		}
		else
		{
			CurrentPlayingMode = EPlayingMode::OnlineMultiplayerServer;

			PanelsSwitcher->SetActiveWidget(CreateSession_CanvasPanel);
			UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenNextPanelButton, ESlateVisibility::Visible);
			UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenNextPanelButton, true);
			UCPP_StaticWidgetLibrary::ChangeImageVisibility(RB_Image, ESlateVisibility::SelfHitTestInvisible);

			if (bIsGamepadMode)
			{
				SetFocusForGamepadMode();
			}
		}
	}
}

void UWCPP_ConfigureLevelParams::FindPublicSessionsButtonOnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->IsPlayerLoggedInDelegate.IsBound())
	{
		if (!GameInstanceRef->IsPlayerLoggedInDelegate.Execute())
		{
			if (GameInstanceRef->StartLoginDelegate.IsBound())
			{
				GameInstanceRef->StartLoginDelegate.Execute();
			}
		}
		else
		{
			if (IsValid(GameInstanceRef) && GameInstanceRef->StartFindingSessionsDelegate.IsBound())
			{
				GameInstanceRef->StartFindingSessionsDelegate.Execute(GameInstanceRef->GetFirstGamePlayer()->
				                                                      GetUniqueNetIdForPlatformUser(),
				                                                      FindSessionsOnLAN_CheckBox->IsChecked());

				if (WidgetTree && !FindingSessionsThrobberSizeBox->HasAnyChildren())
				{
					UCircularThrobber* Throbber = WidgetTree->ConstructWidget<UCircularThrobber>(
						UCircularThrobber::StaticClass());
					Throbber->SetPeriod(1.0f);
					FindingSessionsThrobberSizeBox->AddChild(Throbber);
				}
				UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(FindPublicSessionsButton, false);

				UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CancelFindingPublicSessionsButton, true);
				UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(CancelFindingPublicSessionsButton,
				                                                  ESlateVisibility::Visible);
				if (bIsGamepadMode && FindPublicSessionsButton->HasAnyUserFocus())
				{
					if (IsValid(PlayerControllerRef))
					{
						CancelFindingPublicSessionsButton->SetUserFocus(PlayerControllerRef);
					}
					else
					{
						CancelFindingPublicSessionsButton->SetKeyboardFocus();
					}
				}
			}
		}
	}
}

void UWCPP_ConfigureLevelParams::CancelFindingPublicSessionsButtonOnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->CancelFindingSessionsDelegate.IsBound())
	{
		GameInstanceRef->CancelFindingSessionsDelegate.Execute();
	}
	PublicSessionsScrollBox->ClearChildren();

	if (bIsGamepadMode && CancelFindingPublicSessionsButton->HasAnyUserFocus())
	{
		if (IsValid(PlayerControllerRef))
		{
			FindPublicSessionsButton->SetUserFocus(PlayerControllerRef);
		}
		else
		{
			FindPublicSessionsButton->SetKeyboardFocus();
		}
	}

	if (GetWorld()->GetTimerManager().TimerExists(TH_ClearFindingPublicSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ClearFindingPublicSessionUnusedWidgets);
	}
	GetWorld()->GetTimerManager().SetTimer(TH_ClearFindingPublicSessionUnusedWidgets,
	                                       this,
	                                       &UWCPP_ConfigureLevelParams::
	                                       ClearFindingPublicSessionUnusedWidgets,
	                                       0.02f,
	                                       false);

	if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(CancelFindingPublicSessionsInscription, 3.5f);
	}
}

void UWCPP_ConfigureLevelParams::FindingSessionsCompleted(const TSharedPtr<FOnlineSessionSearch>& FoundSessions)
{
	if (bIsGamepadMode && CancelFindingPublicSessionsButton->HasAnyUserFocus())
	{
		if (IsValid(PlayerControllerRef))
		{
			FindPublicSessionsButton->SetUserFocus(PlayerControllerRef);
		}
		else
		{
			FindPublicSessionsButton->SetKeyboardFocus();
		}
	}
	PublicSessionsScrollBox->ClearChildren();

	if (FoundSessions == nullptr)
	{
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(NoPublicSessionsFoundInscription, 3.5f);
		}
	}
	else if (FoundSessions.IsValid() &&
		IsValid(PlayerControllerRef) &&
		IsValid(GameInstanceRef))
	{
		if (FoundSessions->SearchResults.Num() <= 0)
		{
			if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
			{
				PlayerControllerRef->CreateNotificationDelegate.Execute(NoPublicSessionsFoundInscription, 3.5f);
			}
		}
		else
		{
			const FName Row = FName(TEXT("PublicSessionInfo"));
			if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
				UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
					WidgetBlueprintsDataTable,
					Row)))
			{
				for (int32 SearchIdx = 0; SearchIdx < FoundSessions->SearchResults.Num(); SearchIdx++)
				{
					if (UWCPP_PublicSessionInfo* PublicSessionInfo_Widget = CreateWidget<UWCPP_PublicSessionInfo>(
							PlayerControllerRef, Class);
						IsValid(PublicSessionInfo_Widget))
					{
						PublicSessionInfo_Widget->SetFlags(RF_StrongRefOnFrame);
						PublicSessionInfo_Widget->SessionName = FName(
							FoundSessions->SearchResults[SearchIdx].Session.SessionSettings.
							                                        Settings.FindRef(
								                                        FName("SESSION_NAME")).Data.ToString());
						PublicSessionInfo_Widget->SessionNumber = SearchIdx + 1;
						PublicSessionInfo_Widget->UserId = FoundSessions->SearchResults[SearchIdx].Session.OwningUserId;
						PublicSessionInfo_Widget->SearchResult = FoundSessions->SearchResults[SearchIdx];
						PublicSessionInfo_Widget->PassSessionInfoDelegate.BindUObject(
							this, &UWCPP_ConfigureLevelParams::GetOneSessionInfo);
						PublicSessionsScrollBox->AddChild(PublicSessionInfo_Widget);
					}
				}
			}
		}
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_ClearFindingPublicSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ClearFindingPublicSessionUnusedWidgets);
	}
	GetWorld()->GetTimerManager().SetTimer(TH_ClearFindingPublicSessionUnusedWidgets,
	                                       this,
	                                       &UWCPP_ConfigureLevelParams::
	                                       ClearFindingPublicSessionUnusedWidgets,
	                                       0.3f,
	                                       false);
}

void UWCPP_ConfigureLevelParams::ClearFindingPublicSessionUnusedWidgets()
{
	if (IsValid(GetWorld()) && GetWorld()->GetTimerManager()
	                                     .TimerExists(TH_ClearFindingPublicSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ClearFindingPublicSessionUnusedWidgets);
	}

	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CancelFindingPublicSessionsButton, false);
	UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(CancelFindingPublicSessionsButton,
	                                                  ESlateVisibility::Collapsed);
	if (FindingSessionsThrobberSizeBox->HasAnyChildren())
	{
		FindingSessionsThrobberSizeBox->RemoveChildAt(0);
	}
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(FindPublicSessionsButton, true);
}

void UWCPP_ConfigureLevelParams::GetOneSessionInfo(const int32 SessionNumber,
                                                   const FUniqueNetIdRepl& UserId,
                                                   const FName& SessionName,
                                                   const FOnlineSessionSearchResult& SearchResult)
{
	ChosenPublicSessionNumber = SessionNumber;
	ChosenPublicSessionUserId = UserId;
	ChosenPublicSessionName = SessionName;
	ChosenPublicSessionSearchResult = SearchResult;
	FString NameToPrint = ChosenPublicSessionName.ToString();
	const int32 StrLen = NameToPrint.Len();
	if (StrLen >= 6)
	{
		NameToPrint.RemoveAt(5, StrLen - 5, true);
		NameToPrint.Append(TEXT("..."));
	}
	SessionNameToJoin_TB->SetText(
		FText::FromString(FString::Printf(TEXT("#%d - %s"), ChosenPublicSessionNumber,
		                                  *NameToPrint)));
}

void UWCPP_ConfigureLevelParams::ConnectToPublicSessionButtonOnClick()
{
	if (ChosenPublicSessionNumber == -1)
	{
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(NoAvailableSessionToConnectInscription, 3.5f);
		}
	}
	else if (GameInstanceRef->StartJoiningSessionDelegate.IsBound() &&
		ChosenPublicSessionNumber != -1 && ChosenPublicSessionUserId != nullptr)
	{
		if (GetWorld()->GetTimerManager().TimerExists(TH_ClearJoiningToPublicSessionUnusedWidgets))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_ClearJoiningToPublicSessionUnusedWidgets);
		}
		if (WidgetTree && !JoinToPublicSessionThrobberSizeBox->HasAnyChildren())
		{
			UCircularThrobber* Throbber = WidgetTree->ConstructWidget<UCircularThrobber>(
				UCircularThrobber::StaticClass());
			Throbber->SetPeriod(1.0f);
			JoinToPublicSessionThrobberSizeBox->AddChild(Throbber);
		}
		JoinToPublicSessionHorizontalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ConnectToPublicSessionButton, false);
		bIsJoiningToPublicServer = true;

		const bool bSuccess = GameInstanceRef->StartJoiningSessionDelegate.Execute(ChosenPublicSessionUserId,
			ChosenPublicSessionName,
			ChosenPublicSessionSearchResult);
		if (!bSuccess)
		{
			JoiningToPublicSessionFailed();
		}
		else
		{
			GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::OnlineMultiplayerClientInPublicSession);
		}
	}
	SessionNameToJoin_TB->SetText(FText());
	ChosenPublicSessionNumber = -1;
	ChosenPublicSessionUserId = nullptr;
	ChosenPublicSessionName = FName();
	ChosenPublicSessionSearchResult = FOnlineSessionSearchResult();
}

void UWCPP_ConfigureLevelParams::JoiningToSessionFailed()
{
	if (bIsJoiningToPublicServer)
	{
		JoiningToPublicSessionFailed();
	}
	else if (bIsJoiningToPrivateSession)
	{
		JoiningToPrivateSessionFailed();
	}
}

void UWCPP_ConfigureLevelParams::JoiningToPublicSessionFailed()
{
	if (bIsJoiningToPublicServer)
	{
		bIsJoiningToPublicServer = false;

		if (GetWorld()->GetTimerManager().TimerExists(TH_ClearJoiningToPublicSessionUnusedWidgets))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_ClearJoiningToPublicSessionUnusedWidgets);
		}
		GetWorld()->GetTimerManager().SetTimer(TH_ClearJoiningToPublicSessionUnusedWidgets,
		                                       this,
		                                       &UWCPP_ConfigureLevelParams::
		                                       ClearJoiningToPublicSessionUnusedWidgets,
		                                       0.5f,
		                                       false);

		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(JoiningToPublicSessionFailedInscription, 3.5f);
		}
	}
}

void UWCPP_ConfigureLevelParams::ClearJoiningToPublicSessionUnusedWidgets()
{
	if (IsValid(GetWorld()) && GetWorld()->GetTimerManager()
	                                     .TimerExists(TH_ClearJoiningToPublicSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ClearJoiningToPublicSessionUnusedWidgets);
	}

	JoinToPublicSessionHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ConnectToPublicSessionButton, true);
	if (JoinToPublicSessionThrobberSizeBox->HasAnyChildren())
	{
		JoinToPublicSessionThrobberSizeBox->RemoveChildAt(0);
	}
}

void UWCPP_ConfigureLevelParams::ConnectToPrivateSessionButtonOnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->IsPlayerLoggedInDelegate.IsBound())
	{
		if (!GameInstanceRef->IsPlayerLoggedInDelegate.Execute())
		{
			if (GameInstanceRef->StartLoginDelegate.IsBound())
			{
				GameInstanceRef->StartLoginDelegate.Execute();
			}
		}
		else
		{
			if (GameInstanceRef->StartFindingPrivateSessionDelegate.IsBound())
			{
				if (GetWorld()->GetTimerManager().TimerExists(TH_FindingAndJoiningToPrivateSessionUnusedWidgets))
				{
					GetWorld()->GetTimerManager().ClearTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets);
				}
				if (WidgetTree && !JoinToPrivateSessionThrobberSizeBox->HasAnyChildren())
				{
					UCircularThrobber* Throbber = WidgetTree->ConstructWidget<UCircularThrobber>(
						UCircularThrobber::StaticClass());
					Throbber->SetPeriod(1.0f);
					JoinToPrivateSessionThrobberSizeBox->AddChild(Throbber);
				}
				JoinToPrivateSessionHorizontalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ConnectToPrivateSessionButton, false);
				UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CancelFindingPrivateSessionButton, true);
				if (bIsGamepadMode)
				{
					if (IsValid(PlayerControllerRef))
					{
						CancelFindingPrivateSessionButton->SetUserFocus(PlayerControllerRef);
					}
					else
					{
						CancelFindingPrivateSessionButton->SetKeyboardFocus();
					}
				}
				bIsJoiningToPrivateSession = true;

				const bool bSuccess = GameInstanceRef->StartFindingPrivateSessionDelegate.Execute(
					GameInstanceRef->GetPrimaryPlayerUniqueIdRepl(),
					(InputCode_EditableText->GetText()).ToString(),
					FindPrivateSessionOnLAN_CheckBox->GetCheckedState() == ECheckBoxState::Checked);

				if (!bSuccess)
				{
					JoiningToPrivateSessionFailed();
				}
				else
				{
					GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::OnlineMultiplayerClientInPrivateSession);
				}
			}
		}
	}
}

void UWCPP_ConfigureLevelParams::CancelFindingPrivateSessionButtonOnClick()
{
	if (IsValid(GameInstanceRef) && GameInstanceRef->CancelFindingSessionsDelegate.IsBound())
	{
		GameInstanceRef->CancelFindingSessionsDelegate.Execute();
	}

	if (GetWorld()->GetTimerManager().TimerExists(TH_FindingAndJoiningToPrivateSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets);
	}
	GetWorld()->GetTimerManager().SetTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets,
	                                       this,
	                                       &UWCPP_ConfigureLevelParams::
	                                       ClearFindingAndJoiningToPrivateSessionUnusedWidgets,
	                                       0.005f,
	                                       false);

	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ConnectToPrivateSessionButton, true);

	if (bIsGamepadMode)
	{
		if (IsValid(PlayerControllerRef))
		{
			ConnectToPrivateSessionButton->SetUserFocus(PlayerControllerRef);
		}
		else
		{
			ConnectToPrivateSessionButton->SetKeyboardFocus();
		}
	}
	bIsJoiningToPrivateSession = false;
	if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(CancelFindingPrivateSessionInscription, 3.5f);
	}
}

void UWCPP_ConfigureLevelParams::OnFindingPrivateSessionFailed()
{
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ConnectToPrivateSessionButton, true);

	if (GetWorld()->GetTimerManager().TimerExists(TH_FindingAndJoiningToPrivateSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets);
	}
	GetWorld()->GetTimerManager().SetTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets,
	                                       this,
	                                       &UWCPP_ConfigureLevelParams::
	                                       ClearFindingAndJoiningToPrivateSessionUnusedWidgets,
	                                       0.5f,
	                                       false);

	if (bIsGamepadMode)
	{
		if (IsValid(PlayerControllerRef))
		{
			ConnectToPrivateSessionButton->SetUserFocus(PlayerControllerRef);
		}
		else
		{
			ConnectToPrivateSessionButton->SetKeyboardFocus();
		}
	}
	bIsJoiningToPrivateSession = false;
	if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(FindingPrivateSessionFailedInscription, 6.5f);
	}
}

void UWCPP_ConfigureLevelParams::JoiningToPrivateSessionFailed()
{
	if (bIsJoiningToPrivateSession)
	{
		bIsJoiningToPrivateSession = false;
		UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(ConnectToPrivateSessionButton, true);

		if (GetWorld()->GetTimerManager().TimerExists(TH_FindingAndJoiningToPrivateSessionUnusedWidgets))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets);
		}
		GetWorld()->GetTimerManager().SetTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets,
		                                       this,
		                                       &UWCPP_ConfigureLevelParams::
		                                       ClearFindingAndJoiningToPrivateSessionUnusedWidgets,
		                                       0.5f,
		                                       false);

		if (bIsGamepadMode)
		{
			if (IsValid(PlayerControllerRef))
			{
				ConnectToPrivateSessionButton->SetUserFocus(PlayerControllerRef);
			}
			else
			{
				ConnectToPrivateSessionButton->SetKeyboardFocus();
			}
		}
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(JoiningToPrivateSessionFailedInscription, 3.5f);
		}
	}
}

void UWCPP_ConfigureLevelParams::ClearFindingAndJoiningToPrivateSessionUnusedWidgets()
{
	if (IsValid(GetWorld()) && GetWorld()->GetTimerManager()
	                                     .TimerExists(TH_FindingAndJoiningToPrivateSessionUnusedWidgets))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FindingAndJoiningToPrivateSessionUnusedWidgets);
	}

	if (JoinToPrivateSessionThrobberSizeBox->HasAnyChildren())
	{
		JoinToPrivateSessionThrobberSizeBox->RemoveChildAt(0);
	}
	JoinToPrivateSessionHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(CancelFindingPrivateSessionButton, false);
}

void UWCPP_ConfigureLevelParams::InputCodeCommitted(const FText& NewText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter &&
		PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
	{
		DestroyVirtualKeyboard();
		ConnectToPrivateSessionButtonOnClick();
	}
}

void UWCPP_ConfigureLevelParams::CallServerCreation(const int32 LevelNumber) const
{
	GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::OnlineMultiplayerServer);
	GameInstanceRef->SetLevelNumber(LevelNumber);
	GameInstanceRef->SetCurrentSplitscreenType(0);
	GameInstanceRef->SetMaxOnlinePlayersNumber(FCString::Atoi(*MaxPlayersNumOnServer_ComboBox->
		GetSelectedOption()));
	if (GameInstanceRef->CallSessionCreationDelegate.IsBound())
	{
		GameInstanceRef->CallSessionCreationDelegate.Execute(
			GameInstanceRef->GetLocalPlayerByIndex(0)->GetNickname(),
			SessionShouldBePublic_CheckBox->IsChecked(),
			CreateSessionForLANConnectionOnly_CheckBox->IsChecked(),
			GameInstanceRef->GetMaxOnlinePlayersNumber());
	}
}

void UWCPP_ConfigureLevelParams::SessionCreationFailed() const
{
	if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
	{
		PlayerControllerRef->CreateNotificationDelegate.Execute(SessionCreationFailedInscription, 3.5f);
	}
}

void UWCPP_ConfigureLevelParams::StartLevel1_ButtonOnClick()
{
	StartGame(1);
}

void UWCPP_ConfigureLevelParams::StartLevel2_ButtonOnClick()
{
	StartGame(2);
}

void UWCPP_ConfigureLevelParams::StartLevel3_ButtonOnClick()
{
	StartGame(3);
}

void UWCPP_ConfigureLevelParams::StartLevel4_ButtonOnClick()
{
	StartGame(4);
}

void UWCPP_ConfigureLevelParams::StartLevel5_ButtonOnClick()
{
	StartGame(5);
}

void UWCPP_ConfigureLevelParams::StartRandomLevel_ButtonOnClick()
{
	StartGame(6);
}

void UWCPP_ConfigureLevelParams::OnControllerConnectionChanged(EInputDeviceConnectionState NewConnectionState,
                                                               FPlatformUserId PlatformUserId,
                                                               FInputDeviceId InputDeviceId)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta,
	                                 FString::Printf(
		                                 TEXT("UWCPP_ConfigureLevelParams::OnControllerConnectionChanged = %d"),
		                                 NewConnectionState));*/
	if (NewConnectionState == EInputDeviceConnectionState::Connected)
	{
		ChangeLocalMultiplayerCheckBoxEnabling(true);
	}
	else if (!UCPP_StaticLibrary::IsAnyGamepadConnected())
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta,
		                                 FString::Printf(
			                                 TEXT(
				                                 "UWCPP_ConfigureLevelParams::OnControllerConnectionChanged, IsAnyGamepadConnected() = FALSE")));*/
		ChangeLocalMultiplayerCheckBoxEnabling(false);
		SplitscreenType = 0;
		NumberOfPlayers = 1;
		if (CurrentPlayingMode == EPlayingMode::LocalMultiplayer &&
			(PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel ||
				PanelsSwitcher->GetActiveWidget() == ChooseLevel_CanvasPanel))
		{
			PanelsSwitcher->SetActiveWidget(ChoosePlayingMode_CanvasPanel);
			UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(OpenPreviousPanelButton,
			                                                  ESlateVisibility::Collapsed);
			UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(OpenPreviousPanelButton, false);
			UCPP_StaticWidgetLibrary::ChangeImageVisibility(LB_Image, ESlateVisibility::Collapsed);
		}
	}
	else
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta,
		                                 FString::Printf(
			                                 TEXT(
				                                 "UWCPP_ConfigureLevelParams::OnControllerConnectionChanged, IsAnyGamepadConnected() = TRUE")));*/
		int32 MaxPlayersNumber;
		if (UseKeyboardForFirstPlayer_CheckBox->IsChecked())
		{
			MaxPlayersNumber = UKismetMathLibrary::Min(4,
			                                           UCPP_StaticLibrary::GetConnectedGamepadsNumber() + 1);
		}
		else
		{
			MaxPlayersNumber = UKismetMathLibrary::Min(4,
			                                           UCPP_StaticLibrary::GetConnectedGamepadsNumber());
			if (MaxPlayersNumber == 1)
			{
				UseKeyboardForFirstPlayer_CheckBox->SetCheckedState(ECheckBoxState::Checked);
				UseKeyboardForFirstPlayerOnCheckStateChanged(true);
				UCPP_StaticLibrary::MakeFirstLocalPlayerUseKeyboard();
				bShouldFirstLocalPlayerUseKeyboard = true;
				MaxPlayersNumber = 2;
			}
		}

		if (LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex() + 2 > MaxPlayersNumber)
		{
			int32 Index = MaxPlayersNumber - 2;
			if (Index < 0)
			{
				Index = 0;
			}
			LocalPlayers_WidgetSwitcher->SetActiveWidgetIndex(Index);
			NumberOfPlayers = MaxPlayersNumber;

			UpdateCurrentSplitscreenType(Index);
		}
	}
}

void UWCPP_ConfigureLevelParams::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (!bIsWidgetActive)
		return;

	if (PressedKey == EKeys::Gamepad_LeftShoulder || PressedKey == EKeys::Left) // LB
	{
		LeftArrowOnClick();
	}
	else if (PressedKey == EKeys::Gamepad_RightShoulder || PressedKey == EKeys::Right) // RB
	{
		RightArrowOnClick();
	}
	else if (PressedKey == EKeys::Gamepad_LeftTrigger) // LT
	{
		LT_Pressed();
	}
	else if (PressedKey == EKeys::Gamepad_RightTrigger) // RT
	{
		RT_Pressed();
	}
	else if (PressedKey == EKeys::Gamepad_FaceButton_Top) // Y button
	{
		if (VirtualKeyboardRef == nullptr)
		{
			InitVirtualKeyboard();
		}
		else
		{
			DestroyVirtualKeyboard();
		}
	}
	else if (PressedKey == EKeys::Gamepad_FaceButton_Left) // X button
	{
		if (PanelsSwitcher->GetActiveWidget() == PublicSessions_CanvasPanel)
		{
			ConnectToPublicSessionButtonOnClick();
		}
	}
	else if (PressedKey == EKeys::Enter)
	{
		if (const UWidget* ActiveWidget = PanelsSwitcher->GetActiveWidget();
			ActiveWidget == ChoosePlayingMode_CanvasPanel ||
			ActiveWidget == ChooseLocalPlayers_CanvasPanel ||
			ActiveWidget == CreateSession_CanvasPanel)
		{
			RightArrowOnClick();
		}
	}
}

void UWCPP_ConfigureLevelParams::PrepareWidgetForWorkingWithGamepad()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(1);
	ChangeLocalPlayersNumber_WidgetSwitcher->SetActiveWidgetIndex(1);
	X_Key_ConnectToPublicSession_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		OnOffVirtualKeyboard_EnterName_HorizontalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		OnOffVirtualKeyboard_EnterName_HorizontalBox->SetIsEnabled(true);
	}

	OnOffVirtualKeyboard_PrivateSession_HorizontalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnOffVirtualKeyboard_PrivateSession_HorizontalBox->SetIsEnabled(true);

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_ConfigureLevelParams::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (!bIsWidgetActive)
		return;

	if (IsValid(PlayerControllerRef))
	{
		if (PanelsSwitcher->GetActiveWidget() == ChoosePlayingMode_CanvasPanel)
		{
			SingleMode_CheckBox->SetUserFocus(PlayerControllerRef);
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel)
		{
			UseKeyboardForFirstPlayer_CheckBox->SetUserFocus(PlayerControllerRef);
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseOnlineMultiplayerPage_CanvasPanel)
		{
			OpenPublicSessionsPanelButton->SetUserFocus(PlayerControllerRef);
		}
		else if (PanelsSwitcher->GetActiveWidget() == PublicSessions_CanvasPanel)
		{
			if (FindPublicSessionsButton->GetIsEnabled())
			{
				FindPublicSessionsButton->SetUserFocus(PlayerControllerRef);
			}
			else
			{
				CancelFindingPublicSessionsButton->SetUserFocus(PlayerControllerRef);
			}
		}
		else if (PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
		{
			if (ConnectToPrivateSessionButton->GetIsEnabled())
			{
				ConnectToPrivateSessionButton->SetUserFocus(PlayerControllerRef);
			}
			else
			{
				CancelFindingPrivateSessionButton->SetUserFocus(PlayerControllerRef);
			}
		}
		else if (PanelsSwitcher->GetActiveWidget() == CreateSession_CanvasPanel)
		{
			MaxPlayersNumOnServer_ComboBox->SetUserFocus(PlayerControllerRef);
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseLevel_CanvasPanel)
		{
			StartLevel1_Button->SetUserFocus(PlayerControllerRef);
		}
	}
	else
	{
		if (PanelsSwitcher->GetActiveWidget() == ChoosePlayingMode_CanvasPanel)
		{
			SingleMode_CheckBox->SetKeyboardFocus();
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel)
		{
			UseKeyboardForFirstPlayer_CheckBox->SetKeyboardFocus();
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseOnlineMultiplayerPage_CanvasPanel)
		{
			OpenPublicSessionsPanelButton->SetKeyboardFocus();
		}
		else if (PanelsSwitcher->GetActiveWidget() == PublicSessions_CanvasPanel)
		{
			if (FindPublicSessionsButton->GetIsEnabled())
			{
				FindPublicSessionsButton->SetKeyboardFocus();
			}
			else
			{
				CancelFindingPublicSessionsButton->SetKeyboardFocus();
			}
		}
		else if (PanelsSwitcher->GetActiveWidget() == PrivateSession_CanvasPanel)
		{
			if (ConnectToPrivateSessionButton->GetIsEnabled())
			{
				ConnectToPrivateSessionButton->SetKeyboardFocus();
			}
			else
			{
				CancelFindingPrivateSessionButton->SetKeyboardFocus();
			}
		}
		else if (PanelsSwitcher->GetActiveWidget() == CreateSession_CanvasPanel)
		{
			MaxPlayersNumOnServer_ComboBox->SetKeyboardFocus();
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseLevel_CanvasPanel)
		{
			StartLevel1_Button->SetKeyboardFocus();
		}
	}
}

void UWCPP_ConfigureLevelParams::PrepareWidgetForWorkingWithKeyboard()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(0);
	ChangeLocalPlayersNumber_WidgetSwitcher->SetActiveWidgetIndex(0);
	X_Key_ConnectToPublicSession_Image->SetVisibility(ESlateVisibility::Collapsed);

	if (PanelsSwitcher->GetActiveWidget() == EnterPlayerName_CanvasPanel)
	{
		OnOffVirtualKeyboard_EnterName_HorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
		OnOffVirtualKeyboard_EnterName_HorizontalBox->SetIsEnabled(false);
	}
	OnOffVirtualKeyboard_PrivateSession_HorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	OnOffVirtualKeyboard_PrivateSession_HorizontalBox->SetIsEnabled(false);
	DestroyVirtualKeyboard();

	Super::PrepareWidgetForWorkingWithKeyboard();
}

void UWCPP_ConfigureLevelParams::GamepadScrollCalled(const bool bIsRightStickScroll, const float Rate) const
{
	if (!bIsRightStickScroll)
	{
		if (PanelsSwitcher->GetActiveWidget() == ChoosePlayingMode_CanvasPanel)
		{
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel)
		{
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseOnlineMultiplayerPage_CanvasPanel)
		{
		}
		else if (PanelsSwitcher->GetActiveWidget() == ChooseLevel_CanvasPanel)
		{
		}
	}
}

void UWCPP_ConfigureLevelParams::LT_Pressed()
{
	if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel)
	{
		DecrementLocalPlayersNumberButtonOnClick();
		switch (LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex())
		{
		case 0:
			SplitType1_CheckBox->SetKeyboardFocus();
			break;
		case 1:
			SplitType3_CheckBox->SetKeyboardFocus();
			break;
		case 2:
			SplitType7_CheckBox->SetKeyboardFocus();
			break;
		default:
			break;
		}
	}
}

void UWCPP_ConfigureLevelParams::RT_Pressed()
{
	if (PanelsSwitcher->GetActiveWidget() == ChooseLocalPlayers_CanvasPanel)
	{
		IncrementLocalPlayersNumberButtonOnClick();
		switch (LocalPlayers_WidgetSwitcher->GetActiveWidgetIndex())
		{
		case 0:
			SplitType1_CheckBox->SetKeyboardFocus();
			break;
		case 1:
			SplitType3_CheckBox->SetKeyboardFocus();
			break;
		case 2:
			SplitType7_CheckBox->SetKeyboardFocus();
			break;
		default:
			break;
		}
	}
}

bool UWCPP_ConfigureLevelParams::CheckLevelAvailability(const int32 LevelNumber) const
{
	if (const int32 MaxAvailableLevel = IsValid(PlayerControllerRef)
		                                    ? PlayerControllerRef->GetMaxOpenedLevelNumber()
		                                    : 1;
		LevelNumber > MaxAvailableLevel || LevelNumber < 1)
	{
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(LevelIsUnavailableInscription, 3.5f);
		}
		return false;
	}
	return true;
}

bool UWCPP_ConfigureLevelParams::CanStartGame(const int32 LevelNumber)
{
	if (!CheckLevelAvailability(LevelNumber))
		return false;

	bool bSuccess = false;

	switch (CurrentPlayingMode)
	{
	case EPlayingMode::SinglePlayer:
		bSuccess = true;
		break;

	case EPlayingMode::LocalMultiplayer:
		int32 NumberOfAvailableDevices;
		if (UseKeyboardForFirstPlayer_CheckBox->IsChecked())
		{
			NumberOfAvailableDevices = UCPP_StaticLibrary::GetConnectedGamepadsNumber() + 1;
		}
		else
		{
			NumberOfAvailableDevices = UCPP_StaticLibrary::GetConnectedGamepadsNumber();
		}
		bSuccess = NumberOfPlayers <= NumberOfAvailableDevices;

		if (IsValid(PlayerControllerRef) && PlayerControllerRef->CreateNotificationDelegate.IsBound())
		{
			PlayerControllerRef->CreateNotificationDelegate.Execute(SmallConnectedDevicesAmountInscription, 3.5f);
		}

		break;

	case EPlayingMode::OnlineMultiplayerServer:
		bSuccess = true;
		break;

	case EPlayingMode::OnlineMultiplayerClientInPublicSession:
	case EPlayingMode::OnlineMultiplayerClientInPrivateSession:
		bSuccess = false;
		break;
	}
	return bSuccess;
}

void UWCPP_ConfigureLevelParams::StartGame(const int32 LevelNumber)
{
	if (!CanStartGame(LevelNumber) || !IsValid(GameInstanceRef))
		return;

	ChosenLevelNumber = LevelNumber;

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->CallCollectingInfoForSavingItToFile();
	}

	GameInstanceRef->bIsGamepadModeForPlayer0 = bIsGamepadMode;

	switch (CurrentPlayingMode)
	{
	case EPlayingMode::SinglePlayer:
		GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::SinglePlayer);
		GameInstanceRef->LoadNewLevel(LevelNumber,
		                              0,
		                              1);

		break;

	case EPlayingMode::LocalMultiplayer:
		GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::LocalMultiplayer);
		GameInstanceRef->bIsGamepadModeForPlayer0 = !bShouldFirstLocalPlayerUseKeyboard;
		GameInstanceRef->LoadNewLevel(LevelNumber,
		                              SplitscreenType,
		                              NumberOfPlayers);
		break;

	case EPlayingMode::OnlineMultiplayerServer:
		GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::OnlineMultiplayerServer);
		CallServerCreation(LevelNumber);
		break;

	case EPlayingMode::OnlineMultiplayerClientInPublicSession:
		GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::OnlineMultiplayerClientInPublicSession);
		break;

	case EPlayingMode::OnlineMultiplayerClientInPrivateSession:
		GameInstanceRef->SetPlayingModeAsEnum(EPlayingMode::OnlineMultiplayerClientInPrivateSession);
		break;
	}
}
