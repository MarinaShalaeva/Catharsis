// (c)  M. A. Shalaeva, 2024

#include "../Classes/WCPP_WidgetParent.h"
#include "Kismet/KismetInputLibrary.h"

UWCPP_WidgetParent::UWCPP_WidgetParent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	bIsGamepadMode(false),
	GameInstanceRef(nullptr),
	PlayerControllerRef(nullptr),
	bIsWidgetActive(true)
{
}

void UWCPP_WidgetParent::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerControllerRef.IsValid())
	{
		DH_AllGamepadsWereDisconnected = PlayerControllerRef->AllGamepadsWereDisconnectedDelegate.AddUObject(
			this,
			&UWCPP_WidgetParent::PrepareWidgetForWorkingWithKeyboard);
	}
}

void UWCPP_WidgetParent::NativeDestruct()
{
	if (PlayerControllerRef.IsValid() && PlayerControllerRef->AllGamepadsWereDisconnectedDelegate.IsBound())
	{
		PlayerControllerRef->AllGamepadsWereDisconnectedDelegate.Remove(DH_AllGamepadsWereDisconnected);
		DH_AllGamepadsWereDisconnected.Reset();
	}

	Super::NativeDestruct();
}

void UWCPP_WidgetParent::SetGameInstanceRef(UCPP_GameInstance* NewGameInstance)
{
	GameInstanceRef = NewGameInstance;
}

void UWCPP_WidgetParent::SetPlayerControllerRef(ACPP_PlayerController* NewPlayerController)
{
	PlayerControllerRef = NewPlayerController;
}

void UWCPP_WidgetParent::AnyKeyPressed(const FKey& PressedKey)
{
	if (UKismetInputLibrary::Key_IsGamepadKey(PressedKey) && !bIsGamepadMode)
	{
		PrepareWidgetForWorkingWithGamepad();
	}
	else if ((UKismetInputLibrary::Key_IsMouseButton(PressedKey) || UKismetInputLibrary::Key_IsKeyboardKey(PressedKey))
		&& bIsGamepadMode)
	{
		PrepareWidgetForWorkingWithKeyboard();
	}
}

void UWCPP_WidgetParent::PrepareWidgetForWorkingWithGamepad()
{
	bIsGamepadMode = true;
	SetFocusForGamepadMode();
}

void UWCPP_WidgetParent::SetFocusForGamepadMode()
{
}

void UWCPP_WidgetParent::PrepareWidgetForWorkingWithKeyboard()
{
	bIsGamepadMode = false;
}
