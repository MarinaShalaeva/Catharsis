// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_GameInfo.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_GameInfo::UWCPP_GameInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              InfoScrollBox(nullptr),
                                                                              Button_URL(nullptr)
{
}

void UWCPP_GameInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(PlayerControllerRef))
	{
		PlayerControllerRef = Cast<ACPP_PlayerController>(GetOwningPlayer());
	}
	if (IsValid(PlayerControllerRef))
	{
		DH_GamepadScroll = PlayerControllerRef->GamepadScrollDelegate.AddUObject(this, &UWCPP_GameInfo::GamepadScroll);
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_GameInfo::AnyKeyPressed);
		if (PlayerControllerRef->GetIsGamepadMode())
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}
	}
}

void UWCPP_GameInfo::NativeDestruct()
{
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->GamepadScrollDelegate.Remove(DH_GamepadScroll);
		DH_GamepadScroll.Reset();
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();
		PlayerControllerRef = nullptr;
	}
	Super::NativeDestruct();
}

void UWCPP_GameInfo::GamepadScroll(const bool bIsRightScroll, const float Rate)
{
	if (!bIsRightScroll)
	{
		UButton* TmpButton = GetFocusedButton();
		InfoScrollBox->ScrollWidgetIntoView(TmpButton != nullptr && IsValid(TmpButton) ? TmpButton : Button_URL);
		return;
	}

	UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox, Rate * 5.5f);
}

void UWCPP_GameInfo::OpenWebsite(UTextBlock* TextBlockRef)
{
	if (IsValid(TextBlockRef))
	{
		UCPP_StaticLibrary::OpenWebsiteByURL(TextBlockRef->GetText().ToString());
	}
}

UButton* UWCPP_GameInfo::GetFocusedButton()
{
	for (const auto& Button : URL_Buttons_Array)
	{
		if (Button->HasAnyUserFocus())
		{
			return Button;
		}
	}
	return nullptr;
}

void UWCPP_GameInfo::AnyKeyPressed(const FKey& PressedKey)
{
	Super::AnyKeyPressed(PressedKey);

	if (!bIsWidgetActive)
		return;

	if (PressedKey == EKeys::PageUp)
	{
		InfoScrollBox->ScrollToStart();
	}
	else if (PressedKey == EKeys::PageDown)
	{
		InfoScrollBox->ScrollToEnd();
	}
	else if (PressedKey == EKeys::Up)
	{
		GamepadScroll(true, -4.0f);
	}
	else if (PressedKey == EKeys::Down)
	{
		GamepadScroll(true, 4.0f);
	}
}

void UWCPP_GameInfo::PrepareWidgetForWorkingWithGamepad()
{
	Super::PrepareWidgetForWorkingWithGamepad();
}

void UWCPP_GameInfo::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (!bIsWidgetActive)
		return;

	if (IsValid(PlayerControllerRef))
	{
		Button_URL->SetUserFocus(PlayerControllerRef);
	}
	else
	{
		Button_URL->SetKeyboardFocus();
	}
	InfoScrollBox->ScrollWidgetIntoView(Button_URL);
}

void UWCPP_GameInfo::PrepareWidgetForWorkingWithKeyboard()
{
	Super::PrepareWidgetForWorkingWithKeyboard();
}
