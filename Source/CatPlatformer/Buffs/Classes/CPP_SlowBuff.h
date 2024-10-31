// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_BUFF_H
#define CPP_BUFF_H
#include "CatPlatformer/Buffs/Classes/CPP_Buff.h"
#endif
class ACPP_Buff;

#include "CPP_SlowBuff.generated.h"

/**
 * Class representing a buff that temporarily decreases the
 * character's movement speed.
 */
UCLASS()
class CATPLATFORMER_API ACPP_SlowBuff : public ACPP_Buff
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_SlowBuff();

	/**
	 * Function for applying the buff's effect (decreasing speed).
	 * @param Character Character who collected the buff.
	 */
	virtual void Multicast_CollectBuff_Implementation(ACPP_Character* Character) override;
};
