// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_BUFF_H
#define CPP_BUFF_H
#include "CatPlatformer/Buffs/Classes/CPP_Buff.h"
#endif
class ACPP_Buff;

#include "CPP_HighJumpBuff.generated.h"

/**
 * Class representing a buff that temporarily increases the
 * height of the jump.
 */
UCLASS()
class CATPLATFORMER_API ACPP_HighJumpBuff : public ACPP_Buff
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_HighJumpBuff();

	/**
	 * Function for applying the buff's effect (high jump).
	 * @param Character Character who collected the buff.
	 */
	virtual void Multicast_CollectBuff_Implementation(ACPP_Character* Character) override;
};
