// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WCPP_Notification.generated.h"

class UButton;
class UTextBlock;

/**
 * Widget class needed for showing notifications.
 */
UCLASS(Abstract)
class CATPLATFORMER_API UWCPP_Notification : public UUserWidget
{
	GENERATED_BODY()

	//================Overriden functions==========================

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to
	 * finalize UObject creation.
	 */
	UWCPP_Notification(const FObjectInitializer& ObjectInitializer);

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
	 * Number of seconds after which the notification will
	 * be destroyed.
	 */
	float ClosingTime;

	/**
	 * Timer handle to close notification after ClosingTime
	 * number of seconds.
	 */
	FTimerHandle TH_CloseNotification;

	/** Text block for storing notification's text. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_Notification;

	/** Button for destroying current notification. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CloseNotificationButton;

public:
	/**
	 * Function for updating the ClosingTime variable.
	 * @param NewValue Value to set.
	 */
	void SetClosingTime(const float NewValue);

	/**
	 * Function for updating the notification text block.
	 * @param NotificationText New text to set.
	 */
	void SetNewNotificationText(const FText& NotificationText) const;

	/**
	 * Function for calling the decreasing transparency
	 * animation and destroying current notification.
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StartSelfDestroying();

	/** Function for destroying current widget. */
	UFUNCTION()
	void CloseNotificationButtonOnClick();
};
