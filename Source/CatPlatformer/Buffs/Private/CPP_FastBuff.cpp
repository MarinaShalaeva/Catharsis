// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_FastBuff.h"

ACPP_FastBuff::ACPP_FastBuff()
{
	BuffTypeId = 4;
	EffectDuration = 4.0f;
	ScoreToAdd = 15;
}

void ACPP_FastBuff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
	if (IsValid(Character))
	{
		Character->LaunchFastBuff(EffectDuration, BuffImage);
	}
	Super::Multicast_CollectBuff_Implementation(Character);
}
