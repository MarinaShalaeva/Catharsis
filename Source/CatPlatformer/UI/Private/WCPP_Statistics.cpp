// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Statistics.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

UWCPP_Statistics::UWCPP_Statistics(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	PlayerStateRef(nullptr),
	InfoScrollBox(nullptr),
	TB_Name(nullptr),
	TB_StartedLevelsNumber(nullptr),
	TB_EndedLevelsNumber(nullptr),
	TB_DeathsNumber(nullptr),
	TB_JumpsNumber(nullptr),
	TB_BuffsNumber(nullptr),
	TB_NPCsKilled(nullptr),
	TB_OnlineWinsNumber(nullptr),
	TB_OnlineLossesNumber(nullptr),
	TB_GeneralScore(nullptr),
	TB_TimeInGame(nullptr),
	TB_BestTimeLevel_1(nullptr),
	TB_BestTimeLevel_2(nullptr),
	TB_BestTimeLevel_3(nullptr),
	TB_BestTimeLevel_4(nullptr),
	TB_BestTimeLevel_5(nullptr),
	TB_BestTimeLevel_6(nullptr)
{
}

void UWCPP_Statistics::NativeConstruct()
{
	Super::NativeConstruct();
	
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
		if (PlayerControllerRef->GetIsGamepadMode())
		{
			PrepareWidgetForWorkingWithGamepad();
		}
		else
		{
			PrepareWidgetForWorkingWithKeyboard();
		}
		DH_GamepadScroll = PlayerControllerRef->GamepadScrollDelegate.
		                                        AddUObject(this, &UWCPP_Statistics::GamepadScroll);
		DH_InputKeyWasPressed = PlayerControllerRef->InputKeyWasPressedDelegate.
		                                             AddUObject(this, &UWCPP_Statistics::AnyKeyPressed);
		DH_CharacterWasPossessed = PlayerControllerRef->CharacterWasPossessedDelegate.AddUObject(this,
			&UWCPP_Statistics::NewCharacterWasPossessed);
		if (PlayerControllerRef->GetCharacter())
		{
			NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetCharacter()));
		}
	}
}

void UWCPP_Statistics::NativeDestruct()
{
	if (PlayerControllerRef.IsValid())
	{
		PlayerControllerRef->GamepadScrollDelegate.Remove(DH_GamepadScroll);
		DH_GamepadScroll.Reset();

		PlayerControllerRef->InputKeyWasPressedDelegate.Remove(DH_InputKeyWasPressed);
		DH_InputKeyWasPressed.Reset();

		PlayerControllerRef->CharacterWasPossessedDelegate.Remove(DH_CharacterWasPossessed);
		DH_CharacterWasPossessed.Reset();

		PlayerControllerRef = nullptr;
	}
	PlayerStateRef = nullptr;
	Super::NativeDestruct();
}


void UWCPP_Statistics::NewCharacterWasPossessed(ACPP_Character* NewCharacter)
{
	if (IsValid(NewCharacter))
	{
		if (!NewCharacter->PlayerStateChangedDelegate.IsBoundToObject(this))
		{
			NewCharacter->PlayerStateChangedDelegate.AddUObject(this, &UWCPP_Statistics::PlayerStateWasChanged);
			PlayerStateWasChanged(NewCharacter->GetPlayerState<ACPP_PlayerState>());
		}
	}
}

void UWCPP_Statistics::PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState)
{
	if (!IsValid(NewPlayerState))
		return;

	if (PlayerStateRef.IsValid())
	{
		PlayerStateRef->TimeInGameChangedDelegate.Unbind();
	}
	PlayerStateRef = NewPlayerState;
	if (PlayerStateRef.IsValid())
	{
		PlayerStateRef->TimeInGameChangedDelegate.BindUObject(this, &UWCPP_Statistics::UpdateTimeInGameTextBlock);
	}
	UpdateTextBlocks();
}

void UWCPP_Statistics::UpdateTextBlocks() const
{
	if (!PlayerStateRef.IsValid())
		return;

	FSaveSlot SaveSlot;
	PlayerStateRef->GetTheSaveSlot(SaveSlot);
	TB_Name->SetText(FText::FromString(SaveSlot.UserName));
	TB_StartedLevelsNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.NumberOfStartedLevels)));
	TB_EndedLevelsNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.NumberOfEndedLevels)));
	TB_DeathsNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.DeathsNumber)));
	TB_JumpsNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.JumpsNumber)));
	TB_BuffsNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.CollectedBuffsNumber)));
	TB_NPCsKilled->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.NPCsKilledNumber)));
	TB_OnlineWinsNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.OnlineWinsNumber)));
	TB_OnlineLossesNumber->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.OnlineLossesNumber)));
	TB_GeneralScore->SetText(FText::FromString(FString::Printf(TEXT("%u"), SaveSlot.GeneralScore)));
	TB_TimeInGame->SetText(UCPP_StaticLibrary::GetTextFromSeconds(SaveSlot.TimeInGame));
	TB_BestTimeLevel_1->SetText(UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(SaveSlot.BestTimeLevel_1));
	TB_BestTimeLevel_2->SetText(UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(SaveSlot.BestTimeLevel_2));
	TB_BestTimeLevel_3->SetText(UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(SaveSlot.BestTimeLevel_3));
	TB_BestTimeLevel_4->SetText(UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(SaveSlot.BestTimeLevel_4));
	TB_BestTimeLevel_5->SetText(UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(SaveSlot.BestTimeLevel_5));
	TB_BestTimeLevel_6->SetText(UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(SaveSlot.BestTimeLevel_6));
}

void UWCPP_Statistics::UpdateTimeInGameTextBlock(const uint32 NewTime) const
{
	TB_TimeInGame->SetText(UCPP_StaticLibrary::GetTextFromSeconds(NewTime));
}

void UWCPP_Statistics::GamepadScroll(const bool bIsRightScroll, const float Rate) const
{
	if (!bIsWidgetActive)
		return;

	UCPP_StaticWidgetLibrary::GamepadScroll(InfoScrollBox, Rate * 7.0f);
}

void UWCPP_Statistics::AnyKeyPressed(const FKey& PressedKey)
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
		GamepadScroll(false, -2.0f);
	}
	else if (PressedKey == EKeys::Down)
	{
		GamepadScroll(false, 2.0f);
	}
}
