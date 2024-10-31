// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_StaticWidgetLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"

void UCPP_StaticWidgetLibrary::ChangePanelWidgetVisibility(UPanelWidget* PanelRef, const ESlateVisibility Mode)
{
	if (IsValid(PanelRef))
	{
		PanelRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangeTextBlocksVisibility(UTextBlock* TextBlockRef, const ESlateVisibility Mode)
{
	if (IsValid(TextBlockRef))
	{
		TextBlockRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangeButtonsEnabling(UButton* ButtonRef, const bool bShouldEnable)
{
	if (IsValid(ButtonRef))
	{
		ButtonRef->SetIsEnabled(bShouldEnable);
	}
}

void UCPP_StaticWidgetLibrary::ChangeButtonsVisibility(UButton* ButtonRef, const ESlateVisibility Mode)
{
	if (IsValid(ButtonRef))
	{
		ButtonRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangeButtonsEnablingAndVisibility(UButton* ButtonRef,
                                                                  const bool bShouldEnable,
                                                                  const ESlateVisibility Mode)
{
	if (IsValid(ButtonRef))
	{
		ButtonRef->SetIsEnabled(bShouldEnable);
		ButtonRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::ChangeImageVisibility(UImage* ImageRef, const ESlateVisibility Mode)
{
	if (IsValid(ImageRef))
	{
		ImageRef->SetVisibility(Mode);
	}
}

void UCPP_StaticWidgetLibrary::GamepadScroll(UScrollBox* ScrollBox, const float Speed)
{
	if (const float Offset = ScrollBox->GetScrollOffset() + Speed;
		Offset < 0.0f)
	{
		ScrollBox->SetScrollOffset(0.0f);
	}
	else if (Offset >= ScrollBox->GetScrollOffsetOfEnd())
	{
		ScrollBox->SetScrollOffset(ScrollBox->GetScrollOffsetOfEnd());
	}
	else
	{
		ScrollBox->SetScrollOffset(Offset);
	}
}

TSoftClassPtr<UUserWidget> UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
	const UDataTable* DataTable,
	const FName& Row)
{
	if (!IsValid(DataTable))
	{
		return nullptr;
	}
	if (const FWidgetBlueprintTableRow* FoundRow = DataTable->FindRow<FWidgetBlueprintTableRow>(Row,
		TEXT(""),
		true))
	{
		return FoundRow->WidgetClass;
	}
	return nullptr;
}


TSoftClassPtr<ACPP_SoundManager> UCPP_StaticWidgetLibrary::GetSoftReferenceToSoundManagerByRowName(
	const UDataTable* DataTable,
	const FName& Row)
{
	if (!IsValid(DataTable))
	{
		return nullptr;
	}
	if (const FSoundManagersTableRow* FoundRow = DataTable->FindRow<FSoundManagersTableRow>(Row,
		TEXT(""),
		true))
	{
		return FoundRow->SoundManagerClass;
	}
	return nullptr;
}
