// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UScrollBox;
class USlider;
class UWidgetSwitcher;
class UButton;

#include "WCPP_Rules.generated.h"

/**
 * Widget class for showing rules of the «Cat Platformer»
 * game.
 */
UCLASS()
class CATPLATFORMER_API UWCPP_Rules : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Rules(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

	//===================Bottom Panel==============================
protected:
	/**
	 * Widget switcher for buttons located on the bottom
	 * panel (Open Previous Panel Button, Open Previous
	 * Panel Button). Is needed for changing images for
	 * gamepad and keyboard & mouse modes.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* BottomPanel_WidgetSwitcher;

	/** Button for opening the previous panel in the switcher. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenPreviousPanelButton;

	/** Button for opening the next panel in the switcher. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OpenNextPanelButton;

private:
	/** Function for opening the next panel. */
	UFUNCTION()
	void RightArrowOnClick();

	/** Function for opening the previous panel. */
	UFUNCTION()
	void LeftArrowOnClick();

	//=====================All Panels==============================
protected:
	/**
	 * Widget switcher for containing panels with different
	 * types of settings.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* PanelsSwitcher;

	/** Number of panels in Widget Switcher. */
	int32 PanelsNumber;

	/**
	 * Scroll boxes with game rules, that could be switched
	 * by Panels Switcher.
	*/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_6;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox_7;

	//===============Gamepad || Keyboard modes=====================
private:
	/**
	 * Delegate handle for storing the response on moving
	 * gamepad's stick event.
	 */
	FDelegateHandle DH_GamepadScroll;

	/**
	 * Function that will be called after using the gamepad's
	 * sticks.
	 * @param bIsRightScroll Is scrolling made by right
	 * gamepad's stick?
	 * @param Rate Scrolling rate.
	 */
	void GamepadScroll(const bool bIsRightScroll, const float Rate) const;

	/**
	 * Function for changing current playing mode (gamepad
	 * or keyboard & mouse) by checking the type of the
	 * pressed key.
	 * @param PressedKey Info about the pressed key.
	 */
	virtual void AnyKeyPressed(const FKey& PressedKey) override;

	/** Switch all necessary panels to the gamepad mode. */
	virtual void PrepareWidgetForWorkingWithGamepad() override;

	/**
	 * Switch all necessary panels to the keyboard & mouse
	 * mode.
	 */
	virtual void PrepareWidgetForWorkingWithKeyboard() override;
};
