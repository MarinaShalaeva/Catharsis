// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;

#include "CPP_GameViewportClient.generated.h"

/** Overriden engine's interface to a game viewport. */
UCLASS()
class CATPLATFORMER_API UCPP_GameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

	/**
	 * Function that stores logic that should be applied
	 * when the Game Viewport Client object is created.
	 */
	virtual void Init(FWorldContext& WorldContext,
	                  UGameInstance* OwningGameInstance,
	                  bool bCreateNewAudioDevice) override;

	/**
	 * Function that stores logic that should be applied
	 * when the Game Viewport Client object starts destroying.
	 */
	virtual void BeginDestroy() override;

	/**
	 * Function for changing layout of screens in local
	 * multiplayer.
	 * None = 0;
	 * 2 player horizontal split = 1;
	 * 2 player vertical split = 2;
	 * 3 Player split with 1 player on top and 2 on bottom = 3;
	 * 3 Player split with 1 player on bottom and 2 on top = 4;
	 * 3 Player vertical split = 5;
	 * 3 Player horizontal split = 6;
	 * 4 Player grid split = 7;
	 * 4 Player vertical split = 8;
	 * 4 Player horizontal split = 9.
	 * @param NewType Type of splitscreen to set to
	 * the ActiveSplitscreenType variable.
	 */
	UFUNCTION()
	void SetActiveSplitscreenType(const int32 NewType);

	/**
	 * Delegate handle for storing the response on Change
	 * Active Splitscreen Type event.
	 */
	FDelegateHandle DH_ChangeActiveSplitscreenType;

	/**
	 * Weak pointer to the instance of UCPP_GameInstance
	 * class.
	 */
	TWeakObjectPtr<UCPP_GameInstance> GameInstanceRef = nullptr;
};
