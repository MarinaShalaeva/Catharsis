// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_DoubleJumpBuff.h"

ACPP_DoubleJumpBuff::ACPP_DoubleJumpBuff()
{
	BuffTypeId = 1;
	EffectDuration = 8.0f;
	ScoreToAdd = 5;
}

void ACPP_DoubleJumpBuff::Multicast_CollectBuff_Implementation(ACPP_Character* Character)
{
	if (IsValid(Character))
	{
		Character->LaunchDoubleJumpBuff(EffectDuration, BuffImage);
	}
	Super::Multicast_CollectBuff_Implementation(Character);
}
