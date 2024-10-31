// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef TABLEROWS_H
#define TABLEROWS_H
#include "CatPlatformer/AssetPointer/Classes/TableRows.h"
#endif
class UDataTable;
class UButton;
class UTextBlock;
class UPanelWidget;
class UImage;
class UScrollBox;

#include "CPP_StaticWidgetLibrary.generated.h"

/** Library with static functions for working with widgets. */
UCLASS()
class CATPLATFORMER_API UCPP_StaticWidgetLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Function for changing visibility of any UPanelWidget's
	 * children (such as horizontal/vertical box).
	 * @param PanelRef Reference to the panel widget.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangePanelWidgetVisibility(UPanelWidget* PanelRef, ESlateVisibility Mode);

	/**
	 * Function for changing text block's visibility.
	 * @param TextBlockRef Reference to text block.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangeTextBlocksVisibility(UTextBlock* TextBlockRef, ESlateVisibility Mode);

	/**
	 * Function for changing button's enabling.
	 * @param ButtonRef Reference to button.
	 * @param bShouldEnable Should we make a button enabled
	 * or disabled?
	 */
	static void ChangeButtonsEnabling(UButton* ButtonRef, bool bShouldEnable);

	/**
	 * Function for changing button's visibility.
	 * @param ButtonRef Reference to button.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangeButtonsVisibility(UButton* ButtonRef, ESlateVisibility Mode);

	/**
	 * Function for changing button's enabling.
	 * @param ButtonRef Reference to button.
	 * @param bShouldEnable Should we make a button enabled
	 * or disabled?
	 * @param Mode Visibility mode to set.
	 */
	static void ChangeButtonsEnablingAndVisibility(UButton* ButtonRef,
	                                               const bool bShouldEnable,
	                                               const ESlateVisibility Mode);

	/**
	 * Function for changing image's visibility.
	 * @param ImageRef Reference to image.
	 * @param Mode Visibility mode to set.
	 */
	static void ChangeImageVisibility(UImage* ImageRef, ESlateVisibility Mode);

	/**
	 * Function to perform scroll box's scrolling by gamepad.
	 * @param ScrollBox Scroll box to work with.
	 * @param Speed Scrolling speed.
	 */
	static void GamepadScroll(UScrollBox* ScrollBox, const float Speed);

	/**
	 * Function for getting a pointer (soft reference) to
	 * widget blueprint from data table by the name of the
	 * row.
	 * @param DataTable Data table with the list of available
	 * widget blueprints.
	 * @param Row The name of the row from data table, by
	 * which we should find information.
	 * @return Soft reference to the widget blueprint.
	 */
	static TSoftClassPtr<UUserWidget> GetSoftReferenceToWidgetBlueprintByRowName(
		const UDataTable* DataTable, const FName& Row);

	/**
	 * Function for getting a pointer (soft reference) to
	 * the Sound Manager class from data table by the name
	 * of the row.
	 * @param DataTable Data table with the list of available
	 * Sound Manager child classes.
	 * @param Row The name of the row from data table, by
	 * which we should find information.
	 * @return Soft reference to the Sound Manager class.
	 */
	static TSoftClassPtr<ACPP_SoundManager> GetSoftReferenceToSoundManagerByRowName(
		const UDataTable* DataTable, const FName& Row);
};
