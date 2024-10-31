// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
class UCanvasPanel;
class USizeBox;
class UScrollBox;
class UImage;
#include "WCPP_Container.generated.h"

/**
 * Widget container for all other widgets in game.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_Container : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_Container(const FObjectInitializer& ObjectInitializer);

protected:
	/** General canvas panel for storing other widgets. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ContainerCanvasPanel;

public:
	/**
	 * Background image for the main menu and the loading
	 * screen.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundImage;
	
	/** Size box for the loading widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* LoadingWidgetSizeBox;

	/** Size box for the main menu and the pause widgets. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* MainMenuAndPauseWidgetSizeBox;

	/** Size box for the level widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* LevelWidgetSizeBox;

	/** Scroll box for storing list of notifications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* NotificationsScrollBox;
};
