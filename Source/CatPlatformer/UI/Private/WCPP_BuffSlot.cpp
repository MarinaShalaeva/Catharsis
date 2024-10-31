// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_BuffSlot.h"
#include "Slate/SlateBrushAsset.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

UWCPP_BuffSlot::UWCPP_BuffSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              BuffImage(nullptr),
                                                                              TimerProgressBar(nullptr),
                                                                              EffectDuration(0),
                                                                              BuffTypeId(-1)
{
}

void UWCPP_BuffSlot::NativeDestruct()
{
	ResetTimers();
	Super::NativeDestruct();
}

void UWCPP_BuffSlot::SetBuffTypeId(const int32 InBuffTypeId)
{
	BuffTypeId = InBuffTypeId;
}

int32 UWCPP_BuffSlot::GetBuffTypeId() const
{
	return BuffTypeId;
}

void UWCPP_BuffSlot::SetImage(USlateBrushAsset* InBuffImage)
{
	if (IsValid(InBuffImage))
	{
		BuffImage->SetBrushFromAsset(InBuffImage);
	}
}

void UWCPP_BuffSlot::SetEffectDuration(const float InEffectDuration)
{
	EffectDuration = InEffectDuration;
	TimerProgressBar->SetPercent(1.0f);

	ResetTimers();

	GetWorld()->GetTimerManager().SetTimer(
		TH_UpdateProgressBar,
		this,
		&UWCPP_BuffSlot::UpdateProgressBar,
		0.01f,
		true);

	GetWorld()->GetTimerManager().SetTimer(
		TH_ResetEffect,
		this,
		&UWCPP_BuffSlot::ResetTimers,
		InEffectDuration,
		false);
}

void UWCPP_BuffSlot::UpdateProgressBar() const
{
	const float Percent = TimerProgressBar->GetPercent();
	TimerProgressBar->SetPercent(Percent - 1.0f / (EffectDuration * 100));
}

void UWCPP_BuffSlot::ResetTimers()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_UpdateProgressBar))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_UpdateProgressBar);
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_ResetEffect))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ResetEffect);
	}
}
