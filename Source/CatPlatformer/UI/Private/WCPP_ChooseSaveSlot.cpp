// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_ChooseSaveSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
class UCPP_SaveManager;

UWCPP_ChooseSaveSlot::UWCPP_ChooseSaveSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  ChooseSaveSlot_CanvasPanel(nullptr),
	  ChosenSaveSlot(0),
	  ChooseSaveSlot1_Button(nullptr),
	  TB_PlayerName1(nullptr),
	  VerticalBox_DateCreation_Slot1(nullptr),
	  TB_CreationDate_Slot1(nullptr),
	  TB_CreationTime_Slot1(nullptr),
	  ChooseSaveSlot2_Button(nullptr),
	  TB_PlayerName2(nullptr),
	  VerticalBox_DateCreation_Slot2(nullptr),
	  TB_CreationDate_Slot2(nullptr),
	  TB_CreationTime_Slot2(nullptr),
	  ChooseSaveSlot3_Button(nullptr),
	  TB_PlayerName3(nullptr),
	  VerticalBox_DateCreation_Slot3(nullptr),
	  TB_CreationDate_Slot3(nullptr),
	  TB_CreationTime_Slot3(nullptr),
	  BottomPanel_WidgetSwitcher(nullptr),
	  bBottomPanelIsVisible(false),
	  LoadSaveButton(nullptr),
	  ChooseCurrentEmptySaveSlotButton(nullptr),
	  DeleteSaveButton(nullptr),
	  ExitButton(nullptr),
	  X_Key_LoadSlot_Image(nullptr),
	  B_Key_DeleteSlot_Image(nullptr),
	  X_Key_ChooseSlot_Image(nullptr),
	  Y_Key_ExitButton_Image(nullptr)
{
}

void UWCPP_ChooseSaveSlot::NativeConstruct()
{
	Super::NativeConstruct();

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
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_ChooseSaveSlot::AnyKeyPressed);
	}

	ChooseSaveSlot1_Button->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::ChooseSaveSlot1_ButtonOnClick);
	ChooseSaveSlot2_Button->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::ChooseSaveSlot2_ButtonOnClick);
	ChooseSaveSlot3_Button->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::ChooseSaveSlot3_ButtonOnClick);

	LoadSaveButton->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::CreateOrLoadSlot);
	ChooseCurrentEmptySaveSlotButton->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::CreateOrLoadSlot);
	DeleteSaveButton->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::DeleteSlotInfo);
	ExitButton->OnClicked.AddDynamic(this, &UWCPP_ChooseSaveSlot::ExitButtonOnClick);

	if (!GameInstanceRef.IsValid())
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}

	FString Name = FString();
	SaveSlotsExistence.Init(false, 3);
	FDateTime FileDataCreation;
	FDateTime LatestDataCreation{};
	uint8 ExistingSlotsCounter = 0;
	if (UCPP_SaveManager::GetUserNameAndDateCreationFromSaveSlot(1, Name, FileDataCreation))
	{
		TB_PlayerName1->SetText(FText::FromString(Name));
		TB_CreationDate_Slot1->SetText(UCPP_StaticLibrary::GetDateAsText(FileDataCreation));
		TB_CreationTime_Slot1->SetText(UCPP_StaticLibrary::GetTimeAsText(FileDataCreation));
		VerticalBox_DateCreation_Slot1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SaveSlotsExistence[0] = true;
		if (FileDataCreation >= LatestDataCreation)
		{
			LatestDataCreation = FileDataCreation;
			ChooseSaveSlot1_ButtonOnClick();
		}
		ExistingSlotsCounter++;
	}
	if (UCPP_SaveManager::GetUserNameAndDateCreationFromSaveSlot(2, Name, FileDataCreation))
	{
		TB_PlayerName2->SetText(FText::FromString(Name));
		TB_CreationDate_Slot2->SetText(UCPP_StaticLibrary::GetDateAsText(FileDataCreation));
		TB_CreationTime_Slot2->SetText(UCPP_StaticLibrary::GetTimeAsText(FileDataCreation));
		VerticalBox_DateCreation_Slot2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SaveSlotsExistence[1] = true;
		if (FileDataCreation >= LatestDataCreation)
		{
			LatestDataCreation = FileDataCreation;
			ChooseSaveSlot2_ButtonOnClick();
		}
		ExistingSlotsCounter++;
	}
	if (UCPP_SaveManager::GetUserNameAndDateCreationFromSaveSlot(3, Name, FileDataCreation))
	{
		TB_PlayerName3->SetText(FText::FromString(Name));
		TB_CreationDate_Slot3->SetText(UCPP_StaticLibrary::GetDateAsText(FileDataCreation));
		TB_CreationTime_Slot3->SetText(UCPP_StaticLibrary::GetTimeAsText(FileDataCreation));
		VerticalBox_DateCreation_Slot3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SaveSlotsExistence[2] = true;
		if (FileDataCreation >= LatestDataCreation)
		{
			LatestDataCreation = FileDataCreation;
			ChooseSaveSlot3_ButtonOnClick();
		}
		ExistingSlotsCounter++;
	}

	if (ExistingSlotsCounter == 0)
	{
		ChangeBottomPanelVisibility(false);
	}
}

void UWCPP_ChooseSaveSlot::NativeDestruct()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CheckGamepadFocus))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CheckGamepadFocus);
	}

	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();
	}

	ChooseSaveSlot1_Button->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::ChooseSaveSlot1_ButtonOnClick);
	ChooseSaveSlot2_Button->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::ChooseSaveSlot2_ButtonOnClick);
	ChooseSaveSlot3_Button->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::ChooseSaveSlot3_ButtonOnClick);

	LoadSaveButton->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::CreateOrLoadSlot);
	ChooseCurrentEmptySaveSlotButton->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::CreateOrLoadSlot);
	DeleteSaveButton->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::DeleteSlotInfo);
	ExitButton->OnClicked.RemoveDynamic(this, &UWCPP_ChooseSaveSlot::ExitButtonOnClick);

	Super::NativeDestruct();
}

void UWCPP_ChooseSaveSlot::ChangeBottomPanelVisibility(const bool bShouldBeVisible)
{
	if (bShouldBeVisible && !bBottomPanelIsVisible)
	{
		BottomPanel_WidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BottomPanel_WidgetSwitcher->SetIsEnabled(true);
		bBottomPanelIsVisible = true;
	}
	else if (!bShouldBeVisible && bBottomPanelIsVisible)
	{
		BottomPanel_WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		BottomPanel_WidgetSwitcher->SetIsEnabled(false);
		bBottomPanelIsVisible = false;
	}
}

void UWCPP_ChooseSaveSlot::ChooseSaveSlot1_ButtonOnClick()
{
	switch (ChosenSaveSlot)
	{
	case 2:
		ChangeButtonColor(ChooseSaveSlot2_Button, true);
		break;
	case 3:
		ChangeButtonColor(ChooseSaveSlot3_Button, true);
		break;
	default:
		break;
	}
	ChangeButtonColor(ChooseSaveSlot1_Button, false);
	ChosenSaveSlot = 1;
	ChangeBottomPanelVisibility(true);
	const int32 WidgetSwitcherIndex = SaveSlotsExistence[0] ? 0 : 1;
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(WidgetSwitcherIndex);
}

void UWCPP_ChooseSaveSlot::ChooseSaveSlot2_ButtonOnClick()
{
	switch (ChosenSaveSlot)
	{
	case 1:
		ChangeButtonColor(ChooseSaveSlot1_Button, true);
		break;
	case 3:
		ChangeButtonColor(ChooseSaveSlot3_Button, true);
		break;
	default:
		break;
	}
	ChangeButtonColor(ChooseSaveSlot2_Button, false);
	ChosenSaveSlot = 2;
	const int32 WidgetSwitcherIndex = SaveSlotsExistence[1] ? 0 : 1;
	ChangeBottomPanelVisibility(true);
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(WidgetSwitcherIndex);
}

void UWCPP_ChooseSaveSlot::ChooseSaveSlot3_ButtonOnClick()
{
	switch (ChosenSaveSlot)
	{
	case 1:
		ChangeButtonColor(ChooseSaveSlot1_Button, true);
		break;
	case 2:
		ChangeButtonColor(ChooseSaveSlot2_Button, true);
		break;
	default:
		break;
	}
	ChangeButtonColor(ChooseSaveSlot3_Button, false);
	ChosenSaveSlot = 3;
	const int32 WidgetSwitcherIndex = SaveSlotsExistence[2] ? 0 : 1;
	ChangeBottomPanelVisibility(true);
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(WidgetSwitcherIndex);
}

void UWCPP_ChooseSaveSlot::CheckGamepadFocus()
{
	if (ChooseSaveSlot1_Button->HasKeyboardFocus() && ChosenSaveSlot != 1)
	{
		ChooseSaveSlot1_ButtonOnClick();
	}
	else if (ChooseSaveSlot2_Button->HasKeyboardFocus() && ChosenSaveSlot != 2)
	{
		ChooseSaveSlot2_ButtonOnClick();
	}
	else if (ChooseSaveSlot3_Button->HasKeyboardFocus() && ChosenSaveSlot != 3)
	{
		ChooseSaveSlot3_ButtonOnClick();
	}
}

void UWCPP_ChooseSaveSlot::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (!bIsWidgetActive)
		return;

	if (PressedKey == EKeys::Enter || PressedKey == EKeys::Gamepad_FaceButton_Left /* X button */)
	{
		CreateOrLoadSlot();
	}
	else if (PressedKey == EKeys::Gamepad_FaceButton_Right) /* B button */
	{
		DeleteSlotInfo();
	}
	else if (PressedKey == EKeys::Gamepad_FaceButton_Top) /* B button */
	{
		ExitButtonOnClick();
	}
}

void UWCPP_ChooseSaveSlot::PrepareWidgetForWorkingWithGamepad()
{
	X_Key_LoadSlot_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	B_Key_DeleteSlot_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	X_Key_ChooseSlot_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Y_Key_ExitButton_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_ChooseSaveSlot::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (!bIsWidgetActive)
		return;

	switch (ChosenSaveSlot)
	{
	case 2:
		ChooseSaveSlot2_Button->SetKeyboardFocus();
		break;
	case 3:
		ChooseSaveSlot3_Button->SetKeyboardFocus();
		break;
	default:
		ChooseSaveSlot1_Button->SetKeyboardFocus();
		break;
	}
	GetWorld()->GetTimerManager().SetTimer(TH_CheckGamepadFocus,
	                                       this,
	                                       &UWCPP_ChooseSaveSlot::CheckGamepadFocus,
	                                       0.11f,
	                                       true);
}

void UWCPP_ChooseSaveSlot::PrepareWidgetForWorkingWithKeyboard()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CheckGamepadFocus))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CheckGamepadFocus);
	}

	X_Key_LoadSlot_Image->SetVisibility(ESlateVisibility::Collapsed);
	B_Key_DeleteSlot_Image->SetVisibility(ESlateVisibility::Collapsed);
	X_Key_ChooseSlot_Image->SetVisibility(ESlateVisibility::Collapsed);
	Y_Key_ExitButton_Image->SetVisibility(ESlateVisibility::Collapsed);

	Super::PrepareWidgetForWorkingWithKeyboard();
}

void UWCPP_ChooseSaveSlot::ChangeButtonColor(UButton* Button, const bool bNormalColor) const
{
	if (bNormalColor)
	{
		Button->SetBackgroundColor(ButtonNormalBackgroundColor);
	}
	else
	{
		Button->SetBackgroundColor(ButtonChosenBackgroundColor);
	}
}

void UWCPP_ChooseSaveSlot::CreateOrLoadSlot()
{
	if (ChosenSaveSlot != 0 && GameInstanceRef.IsValid())
	{
		GameInstanceRef->GetSaveManager()->ChooseSaveSlot(ChosenSaveSlot);
		if (GameInstanceRef->GetSaveManager()->DoesSaveSlotExist())
		{
			GameInstanceRef->GetSaveManager()->LoadOrCreateSaveGameObject();
			if (PlayerControllerRef.IsValid())
			{
				PlayerControllerRef->CallLoadingInfoFromSaveFile();
			}
		}
		else
		{
			GameInstanceRef->GetSaveManager()->LoadOrCreateSaveGameObject();
			if (PlayerControllerRef.IsValid())
			{
				// Is needed for applying the base values to the
				// player state variables.
				PlayerControllerRef->CallLoadingInfoFromSaveFile();
				if (PlayerControllerRef->CreateNotificationDelegate.IsBound())
				{
					PlayerControllerRef->CreateNotificationDelegate.Execute(ReadRulesBeforePlayingInscription, 4.0f);
				}
			}
		}
		OpenMainMenu();
	}
}

void UWCPP_ChooseSaveSlot::DeleteSlotInfo()
{
	if (ChosenSaveSlot != 0 && SaveSlotsExistence[ChosenSaveSlot - 1])
	{
		if (UCPP_SaveManager::DeleteSaveBySlotNumber(ChosenSaveSlot))
		{
			switch (ChosenSaveSlot)
			{
			case 1:
				ChangeButtonColor(ChooseSaveSlot1_Button, true);
				TB_PlayerName1->SetText(EmptySlotInscription);
				VerticalBox_DateCreation_Slot1->SetVisibility(ESlateVisibility::Collapsed);
				break;
			case 2:
				ChangeButtonColor(ChooseSaveSlot2_Button, true);
				TB_PlayerName2->SetText(EmptySlotInscription);
				VerticalBox_DateCreation_Slot2->SetVisibility(ESlateVisibility::Collapsed);
				break;
			case 3:
				ChangeButtonColor(ChooseSaveSlot2_Button, true);
				TB_PlayerName3->SetText(EmptySlotInscription);
				VerticalBox_DateCreation_Slot3->SetVisibility(ESlateVisibility::Collapsed);
				break;
			default:
				break;
			}

			BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(1);
			SaveSlotsExistence[ChosenSaveSlot - 1] = false;
		}
	}
}

void UWCPP_ChooseSaveSlot::OpenMainMenu()
{
	if (ShouldOpenMainMenuDelegate.IsBound())
	{
		ShouldOpenMainMenuDelegate.Execute();
	}
}

void UWCPP_ChooseSaveSlot::ExitButtonOnClick()
{
	const UWorld* World = GetWorld();
	if (PlayerControllerRef.IsValid())
	{
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
