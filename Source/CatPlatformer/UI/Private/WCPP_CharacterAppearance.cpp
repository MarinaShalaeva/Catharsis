// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_CharacterAppearance.h"
#include "Components/CheckBox.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif

UWCPP_CharacterAppearance::UWCPP_CharacterAppearance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), PlayerStateRef(nullptr),
	  GrayColor_CheckBox(nullptr), OrangeColor_CheckBox(nullptr),
	  BlackColor_CheckBox(nullptr), CatColorIndex(0)
{
}

void UWCPP_CharacterAppearance::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GameInstanceRef.IsValid())
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GetGameInstance());
	}

	if (!PlayerControllerRef.IsValid())
	{
		if (GameInstanceRef.IsValid())
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

	if (PlayerControllerRef.IsValid())
	{
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_CharacterAppearance::AnyKeyPressed);
		DH_CharacterWasPossessed = PlayerControllerRef->CharacterWasPossessedDelegate.AddUObject(this,
			&UWCPP_CharacterAppearance::NewCharacterWasPossessed);
		if (PlayerControllerRef->GetCharacter())
		{
			NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetCharacter()));
		}
		if (PlayerControllerRef->GetIsGamepadMode())
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}
	}

	GrayColor_CheckBox->OnCheckStateChanged
	                  .AddDynamic(this, &UWCPP_CharacterAppearance::GrayColorOnCheckStateChanged);
	OrangeColor_CheckBox->OnCheckStateChanged
	                    .AddDynamic(this, &UWCPP_CharacterAppearance::OrangeColorOnCheckStateChanged);
	BlackColor_CheckBox->OnCheckStateChanged
	                   .AddDynamic(this, &UWCPP_CharacterAppearance::BlackColorOnCheckStateChanged);
}

void UWCPP_CharacterAppearance::NativeDestruct()
{
	if (PlayerStateRef.IsValid())
	{
		PlayerStateRef->SaveDataToFile();
		PlayerStateRef = nullptr;
	}

	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();

		PlayerControllerRef->CharacterWasPossessedDelegate.Remove(DH_CharacterWasPossessed);
		DH_CharacterWasPossessed.Reset();

		PlayerControllerRef = nullptr;
	}

	GrayColor_CheckBox->OnCheckStateChanged
	                  .RemoveDynamic(this, &UWCPP_CharacterAppearance::GrayColorOnCheckStateChanged);
	OrangeColor_CheckBox->OnCheckStateChanged
	                    .RemoveDynamic(this, &UWCPP_CharacterAppearance::OrangeColorOnCheckStateChanged);
	BlackColor_CheckBox->OnCheckStateChanged
	                   .RemoveDynamic(this, &UWCPP_CharacterAppearance::BlackColorOnCheckStateChanged);

	Super::NativeDestruct();
}

void UWCPP_CharacterAppearance::NewCharacterWasPossessed(ACPP_Character* NewCharacter)
{
	if (IsValid(NewCharacter))
	{
		if (!NewCharacter->PlayerStateChangedDelegate.IsBoundToObject(this))
		{
			NewCharacter->PlayerStateChangedDelegate.
			              AddUObject(this, &UWCPP_CharacterAppearance::PlayerStateWasChanged);
			PlayerStateWasChanged(NewCharacter->GetPlayerState<ACPP_PlayerState>());
		}
	}
}

void UWCPP_CharacterAppearance::PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState)
{
	if (!IsValid(NewPlayerState))
		return;

	PlayerStateRef = NewPlayerState;
	CatColorIndex = PlayerStateRef->GetCatColorIndex();
	switch (CatColorIndex)
	{
	case 0:
		{
			BlackColor_CheckBox->SetCheckedState(ECheckBoxState::Checked);
			BlackColorOnCheckStateChanged(true);
			break;
		}
	case 1:
		{
			GrayColor_CheckBox->SetCheckedState(ECheckBoxState::Checked);
			GrayColorOnCheckStateChanged(true);
			break;
		}
	case 2:
		{
			OrangeColor_CheckBox->SetCheckedState(ECheckBoxState::Checked);
			OrangeColorOnCheckStateChanged(true);
			break;
		}
	default: break;
	}
}

void UWCPP_CharacterAppearance::GrayColorOnCheckStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		ChangeCatsColor(1);
		OrangeColor_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		BlackColor_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		GrayColor_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_CharacterAppearance::OrangeColorOnCheckStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		ChangeCatsColor(2);
		GrayColor_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		BlackColor_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		OrangeColor_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_CharacterAppearance::BlackColorOnCheckStateChanged(const bool bNewState)
{
	if (bNewState)
	{
		ChangeCatsColor(0);
		GrayColor_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		OrangeColor_CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		BlackColor_CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UWCPP_CharacterAppearance::ChangeCatsColor_Implementation(const int32 ColorIndex)
{
	CatColorIndex = ColorIndex;
	PlayerStateRef->SetCatColorIndex(CatColorIndex);
}

void UWCPP_CharacterAppearance::SetFocusForGamepadMode()
{
	Super::SetFocusForGamepadMode();

	if (!bIsWidgetActive)
		return;

	if (PlayerControllerRef.IsValid())
	{
		if (GrayColor_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			GrayColor_CheckBox->SetUserFocus(PlayerControllerRef.Get());
		}
		else if (OrangeColor_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			OrangeColor_CheckBox->SetUserFocus(PlayerControllerRef.Get());
		}
		else if (BlackColor_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			BlackColor_CheckBox->SetUserFocus(PlayerControllerRef.Get());
		}
	}
	else
	{
		if (GrayColor_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			GrayColor_CheckBox->SetKeyboardFocus();
		}
		else if (OrangeColor_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			OrangeColor_CheckBox->SetKeyboardFocus();
		}
		else if (BlackColor_CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			BlackColor_CheckBox->SetKeyboardFocus();
		}
	}
}
