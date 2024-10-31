// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_SlowBuff.h"

ACPP_SlowBuff::ACPP_SlowBuff()
{
	BuffTypeId = 3;
	EffectDuration = 4.0f;
	ScoreToAdd = 15;
}

void ACPP_SlowBuff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
	if (IsValid(Character))
	{
		Character->LaunchSlowBuff(EffectDuration, BuffImage);
	}
	Super::Multicast_CollectBuff_Implementation(Character);
}
