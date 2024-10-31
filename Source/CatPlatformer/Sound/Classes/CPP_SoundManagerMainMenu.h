// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_SOUNDMANAGER_H
#define CPP_SOUNDMANAGER_H
#include "CatPlatformer/Sound/Classes/CPP_SoundManager.h"
#endif
class ACPP_SoundManager;

#include "CPP_SoundManagerMainMenu.generated.h"

/**
 * Class for working with music that should be played in
 * the main menu.
 */
UCLASS()
class CATPLATFORMER_API ACPP_SoundManagerMainMenu : public ACPP_SoundManager
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_SoundManagerMainMenu();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the actor's life (but before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Audio Component for working with the 1st audio track
	 * related to the main menu.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Audio", meta = (AllowPrivateAccess = true))
	UAudioComponent* MM_TrackAudioComponent_1;

	/**
	 * Audio Component for working with the 2nd audio track
	 * related to the main menu.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Audio", meta = (AllowPrivateAccess = true))
	UAudioComponent* MM_TrackAudioComponent_2;

	/**
	 * Audio Component for working with the 3rd audio track
	 * related to the main menu.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Audio", meta = (AllowPrivateAccess = true))
	UAudioComponent* MM_TrackAudioComponent_3;

	/**
	 * Function for adding all audio components to the
	 * array.
	 */
	virtual void FillAllAudioComponentsArray() override;

public:
	/** Function to apply saved data or to apply basic values. */
	virtual void ApplySettingsFromTheSaveFile(const bool bSetDefaultValues,
	                                          const bool bIsAdditionalSoundManager) override;
};
