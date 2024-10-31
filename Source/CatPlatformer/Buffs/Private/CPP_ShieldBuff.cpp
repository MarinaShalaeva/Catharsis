// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_ShieldBuff.h"

ACPP_ShieldBuff::ACPP_ShieldBuff()
{
	BuffTypeId = 5;
	EffectDuration = 8.0f;
	ScoreToAdd = 0;
}

void ACPP_ShieldBuff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
	if (IsValid(Character))
	{
		Character->LaunchShieldBuff(EffectDuration, BuffImage);
	}
	Super::Multicast_CollectBuff_Implementation(Character);
}
