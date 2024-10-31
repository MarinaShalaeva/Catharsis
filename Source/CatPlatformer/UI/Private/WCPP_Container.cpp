// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_Container.h"

UWCPP_Container::UWCPP_Container(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	ContainerCanvasPanel(nullptr),
	BackgroundImage(nullptr),
	LoadingWidgetSizeBox(nullptr),
	MainMenuAndPauseWidgetSizeBox(nullptr),
	LevelWidgetSizeBox(nullptr),
	NotificationsScrollBox(nullptr)
{
}
