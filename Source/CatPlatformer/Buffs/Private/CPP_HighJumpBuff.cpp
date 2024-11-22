// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_HighJumpBuff.h"

ACPP_HighJumpBuff::ACPP_HighJumpBuff()
{
	BuffTypeId = 2;
	EffectDuration = 6.0f;
	ScoreToAdd = 5;
}

void ACPP_HighJumpBuff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
	if (IsValid(Character))
	{
		Character->LaunchHighJumpBuff(EffectDuration, BuffImage);
	}
	Super::Multicast_CollectBuff_Implementation(Character);
}
