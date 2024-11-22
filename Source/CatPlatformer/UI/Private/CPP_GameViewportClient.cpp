// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_GameViewportClient.h"
#include "GameMapsSettings.h"

void UCPP_GameViewportClient::Init(FWorldContext& WorldContext,
                                   UGameInstance* OwningGameInstance,
                                   bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);

	if (IsValid(OwningGameInstance))
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(OwningGameInstance);
		if (GameInstanceRef.IsValid())
		{
			DH_ChangeActiveSplitscreenType = GameInstanceRef->ChangeActiveSplitscreenTypeDelegate.AddUObject(
				this, &UCPP_GameViewportClient::SetActiveSplitscreenType);
		}
	}
}

void UCPP_GameViewportClient::BeginDestroy()
{
	if (GameInstanceRef.IsValid())
	{
		GameInstanceRef->ChangeActiveSplitscreenTypeDelegate.Remove(DH_ChangeActiveSplitscreenType);
		DH_ChangeActiveSplitscreenType.Reset();
		GameInstanceRef = nullptr;
	}
	Super::BeginDestroy();
}

void UCPP_GameViewportClient::SetActiveSplitscreenType(const int32 NewType)
{
	if (NewType < 0 || NewType > 9)
	{
		return;
	}
	ActiveSplitscreenType = static_cast<ESplitScreenType::Type>(NewType);

	UGameMapsSettings* Settings = UGameMapsSettings::GetGameMapsSettings();

	switch (NewType)
	{
	case 1:
		Settings->TwoPlayerSplitscreenLayout = ETwoPlayerSplitScreenType::Horizontal;
		break;
	case 2:
		Settings->TwoPlayerSplitscreenLayout = ETwoPlayerSplitScreenType::Vertical;
		break;
	case 3:
		Settings->ThreePlayerSplitscreenLayout = EThreePlayerSplitScreenType::FavorTop;
		break;
	case 4:
		Settings->ThreePlayerSplitscreenLayout = EThreePlayerSplitScreenType::FavorBottom;
		break;
	case 5:
		Settings->ThreePlayerSplitscreenLayout = EThreePlayerSplitScreenType::Vertical;
		break;
	case 6:
		Settings->ThreePlayerSplitscreenLayout = EThreePlayerSplitScreenType::Horizontal;
		break;
	case 7:
		Settings->FourPlayerSplitscreenLayout = EFourPlayerSplitScreenType::Grid;
		break;
	case 8:
		Settings->FourPlayerSplitscreenLayout = EFourPlayerSplitScreenType::Vertical;
		break;
	case 9:
		Settings->FourPlayerSplitscreenLayout = EFourPlayerSplitScreenType::Horizontal;
		break;
	default:
		break;
	}
}
