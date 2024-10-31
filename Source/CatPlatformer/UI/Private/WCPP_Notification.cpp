// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Notification.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UWCPP_Notification::UWCPP_Notification(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ClosingTime(3.5f),
	TB_Notification(nullptr),
	CloseNotificationButton(nullptr)
{
}

void UWCPP_Notification::NativeConstruct()
{
	Super::NativeConstruct();

	CloseNotificationButton->OnClicked.AddDynamic(this, &UWCPP_Notification::CloseNotificationButtonOnClick);

	GetWorld()->GetTimerManager().SetTimer(TH_CloseNotification,
	                                       this,
	                                       &UWCPP_Notification::StartSelfDestroying,
	                                       ClosingTime,
	                                       false);
}

void UWCPP_Notification::NativeDestruct()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CloseNotification))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CloseNotification);
	}

	CloseNotificationButton->OnClicked.RemoveDynamic(this, &UWCPP_Notification::CloseNotificationButtonOnClick);

	Super::NativeDestruct();
}

void UWCPP_Notification::SetClosingTime(const float NewValue)
{
	ClosingTime = NewValue;
}

void UWCPP_Notification::SetNewNotificationText(const FText& NotificationText) const
{
	TB_Notification->SetText(NotificationText);
}

void UWCPP_Notification::CloseNotificationButtonOnClick()
{
	RemoveFromParent();
}
