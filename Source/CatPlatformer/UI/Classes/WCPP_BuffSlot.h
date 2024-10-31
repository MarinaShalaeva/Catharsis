// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
class USlateBrushAsset;
class UImage;
class UProgressBar;
#include "WCPP_BuffSlot.generated.h"

/**
 * Widget for the buff's representation in the level widget.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_BuffSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_BuffSlot(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;
	
	/** Widget containing current buff's image. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* BuffImage;

	/**
	 * Progress Bar for showing the time remaining until the
	 * expiration of the buff's effect.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* TimerProgressBar;

private:
	/**
	 * Timer handle for destroying this widget after the
	 * resetting of the buff.
	 */
	FTimerHandle TH_ResetEffect;
	/** Timer handle for updating buff's effect progress bar. */
	FTimerHandle TH_UpdateProgressBar;

	/** Duration of the buff's effect. */
	UPROPERTY()
	float EffectDuration;
	
	/**
	 * The identification number of the buff type (for all
	 * instances of one particular child class this id will
	 * be the same).
	 */
	UPROPERTY()
	int32 BuffTypeId;

public:
	/**
	 * Setter for the BuffTypeId variable.
	 * @param InBuffTypeId New buff's type id.
	 */
	UFUNCTION(BlueprintCallable)
	void SetBuffTypeId(const int32 InBuffTypeId);

	/**
	 * Getter for the BuffTypeId variable.
	 * @return BuffTypeId variable.
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetBuffTypeId() const;

	/**
	 * Function for updating the image in BuffImage widget.
	 * @param InBuffImage New buff's image.
	 */
	UFUNCTION(BlueprintCallable)
	void SetImage(USlateBrushAsset* InBuffImage);

	/**
	 * Function for updating the EffectDuration variable
	 * and for launching timers.
	 * @param InEffectDuration New effect duration.
	 */
	UFUNCTION(BlueprintCallable)
	void SetEffectDuration(const float InEffectDuration);

	/** Function for updating buff's effect progress bar. */
	UFUNCTION()
	void UpdateProgressBar() const;

	/**
	 * Function for resetting all existing in this class
	 * timers.
	 */
	UFUNCTION()
	void ResetTimers();
};
