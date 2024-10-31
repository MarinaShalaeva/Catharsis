// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Rules.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_Rules::UWCPP_Rules(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                        BottomPanel_WidgetSwitcher(nullptr),
                                                                        OpenPreviousPanelButton(nullptr),
                                                                        OpenNextPanelButton(nullptr),
                                                                        PanelsSwitcher(nullptr),
                                                                        PanelsNumber(0),
                                                                        InfoScrollBox(nullptr),
                                                                        InfoScrollBox_1(nullptr),
                                                                        InfoScrollBox_2(nullptr),
                                                                        InfoScrollBox_3(nullptr),
                                                                        InfoScrollBox_4(nullptr),
                                                                        InfoScrollBox_5(nullptr),
                                                                        InfoScrollBox_6(nullptr),
                                                                        InfoScrollBox_7(nullptr)
{
}

void UWCPP_Rules::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(PlayerControllerRef))
	{
		PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
	}
	if (IsValid(PlayerControllerRef))
	{
		DH_GamepadScroll = PlayerControllerRef->GamepadScrollDelegate.AddUObject(this, &UWCPP_Rules::GamepadScroll);
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_Rules::AnyKeyPressed);

		if (PlayerControllerRef->GetIsGamepadMode())
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}
	}

	//=====================All Panels==============================

	PanelsNumber = PanelsSwitcher->GetNumWidgets();

	//===================Bottom Panel==============================

	OpenPreviousPanelButton->OnClicked.AddDynamic(this, &UWCPP_Rules::LeftArrowOnClick);
	OpenNextPanelButton->OnClicked.AddDynamic(this, &UWCPP_Rules::RightArrowOnClick);
}

void UWCPP_Rules::NativeDestruct()
{
	//===================Bottom Panel==============================

	OpenPreviousPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_Rules::LeftArrowOnClick);
	OpenNextPanelButton->OnClicked.RemoveDynamic(this, &UWCPP_Rules::RightArrowOnClick);

	Super::NativeDestruct();
}

void UWCPP_Rules::RightArrowOnClick()
{
	if (const int32 CurrentIndex = PanelsSwitcher->GetActiveWidgetIndex();
		CurrentIndex + 1 == PanelsNumber)
	{
		PanelsSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		PanelsSwitcher->SetActiveWidgetIndex(CurrentIndex + 1);
	}
}

void UWCPP_Rules::LeftArrowOnClick()
{
	if (const int32 CurrentIndex = PanelsSwitcher->GetActiveWidgetIndex();
		CurrentIndex - 1 == -1)
	{
		PanelsSwitcher->SetActiveWidgetIndex(PanelsNumber - 1);
	}
	else
	{
		PanelsSwitcher->SetActiveWidgetIndex(CurrentIndex - 1);
	}
}

void UWCPP_Rules::GamepadScroll(const bool bIsRightScroll, const float Rate) const
{
	if (!bIsWidgetActive)
		return;

	const float Speed = Rate * 8.5f;

	if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_1)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_1, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_2)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_2, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_3)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_3, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_4)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_4, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_5)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_5, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_6)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_6, Speed);
	}
	else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_7)
	{
		UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox_7, Speed);
	}
}

void UWCPP_Rules::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (!bIsWidgetActive)
		return;

	if (PressedKey == EKeys::PageUp)
	{
		if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox)
		{
			InfoScrollBox->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_1)
		{
			InfoScrollBox_1->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_2)
		{
			InfoScrollBox_2->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_3)
		{
			InfoScrollBox_3->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_4)
		{
			InfoScrollBox_4->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_5)
		{
			InfoScrollBox_5->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_6)
		{
			InfoScrollBox_6->ScrollToStart();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_7)
		{
			InfoScrollBox_7->ScrollToStart();
		}
	}
	else if (PressedKey == EKeys::PageDown)
	{
		if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox)
		{
			InfoScrollBox->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_1)
		{
			InfoScrollBox_1->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_2)
		{
			InfoScrollBox_2->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_3)
		{
			InfoScrollBox_3->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_4)
		{
			InfoScrollBox_4->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_5)
		{
			InfoScrollBox_5->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_6)
		{
			InfoScrollBox_6->ScrollToEnd();
		}
		else if (PanelsSwitcher->GetActiveWidget() == InfoScrollBox_7)
		{
			InfoScrollBox_7->ScrollToEnd();
		}
	}
	else if (PressedKey == EKeys::Up)
	{
		GamepadScroll(false, -5.0f);
	}
	else if (PressedKey == EKeys::Down)
	{
		GamepadScroll(false, 5.0f);
	}
	else if (PressedKey == EKeys::Gamepad_LeftShoulder || PressedKey == EKeys::Left) // LB
	{
		LeftArrowOnClick();
	}
	else if (PressedKey == EKeys::Gamepad_RightShoulder || PressedKey == EKeys::Right) // RB
	{
		RightArrowOnClick();
	}
}

void UWCPP_Rules::PrepareWidgetForWorkingWithGamepad()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(1);

	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_Rules::PrepareWidgetForWorkingWithKeyboard()
{
	BottomPanel_WidgetSwitcher->SetActiveWidgetIndex(0);

	Super::PrepareWidgetForWorkingWithKeyboard();
}
