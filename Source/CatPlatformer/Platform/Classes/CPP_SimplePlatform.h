// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef CPP_PLATFORM_H
#define CPP_PLATFORM_H
#include "CatPlatformer/Platform/Classes/CPP_Platform.h"
#endif
class ACPP_Platform;

#include "CPP_SimplePlatform.generated.h"

/**
 * Platform with no effect on players.
 */
UCLASS(Blueprintable)
class CATPLATFORMER_API ACPP_SimplePlatform : public ACPP_Platform
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_SimplePlatform();
};
