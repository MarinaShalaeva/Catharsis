// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UButton;
class UScrollBox;
class UTextBlock;

#include "WCPP_GameInfo.generated.h"

/**
 * Parent widget class for representing a panel this
 * information about development of the «Cat Platformer»
 * game.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_GameInfo : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_GameInfo(const FObjectInitializer& ObjectInitializer);

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

protected:
	/**
	 * Scroll box with information about game's author,
	 * used assets, etc.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* InfoScrollBox;

	/** Button for the first URL. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "URL Buttons")
	UButton* Button_URL;

	/** Array of buttons with URLs. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URL Buttons", DisplayName = "URL Buttons Array")
	TArray<UButton*> URL_Buttons_Array;

private:
	/**
	 * Delegate handle for storing the response on moving
	 * gamepad's stick event.
	 */
	FDelegateHandle DH_GamepadScroll;

	/**
	 * Function for scroll the game's info by gamepad's
	 * sticks.
	 * @param bIsRightScroll Is scrolling made by right
	 * gamepad's stick?
	 * @param Rate Scrolling rate.
	 */
	void GamepadScroll(const bool bIsRightScroll, const float Rate);

	/**
	 * Function for calling URL's launching.
	 * @param TextBlockRef Text block with website's URL.
	 */
	UFUNCTION(BlueprintCallable)
	void OpenWebsite(UTextBlock* TextBlockRef);

	/**
	 * Function for finding button with user focus.
	 * @return Button which has focus.
	 */
	UFUNCTION()
	UButton* GetFocusedButton();

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
	 * Function for setting focus on necessary UI element
	 * for correct working with this widget by gamepad.
	 */
	virtual void SetFocusForGamepadMode() override;

	/**
	 * Switch all necessary panels to the keyboard & mouse
	 * mode.
	 */
	virtual void PrepareWidgetForWorkingWithKeyboard() override;
};
