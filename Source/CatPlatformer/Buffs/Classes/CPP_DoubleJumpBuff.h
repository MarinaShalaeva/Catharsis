// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_BUFF_H
#define CPP_BUFF_H
#include "CatPlatformer/Buffs/Classes/CPP_Buff.h"
#endif
class ACPP_Buff;

#include "CPP_DoubleJumpBuff.generated.h"

/**
 * Class representing a buff that temporarily gives an opportunity
 * to make a second jump when the player is already in the air
 * (double jump).
 */
UCLASS()
class CATPLATFORMER_API ACPP_DoubleJumpBuff : public ACPP_Buff
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_DoubleJumpBuff();

	/**
	 * Function for applying the buff's effect (double jump).
	 * @param Character Character who collected the buff.
	 */
	virtual void Multicast_CollectBuff_Implementation(ACPP_Character* Character) override;
};
