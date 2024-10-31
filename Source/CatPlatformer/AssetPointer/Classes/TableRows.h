// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef CPP_SOUNDMANAGER_H
#define CPP_SOUNDMANAGER_H
#include "CatPlatformer/Sound/Classes/CPP_SoundManager.h"
#endif
class ACPP_SoundManager;

#include "TableRows.generated.h"

/**
 * The table row for storing soft references to widget
 * blueprints.
 */
USTRUCT(Blueprintable)
struct FWidgetBlueprintTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	/** Soft reference to widget blueprint class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> WidgetClass;

	/** The constructor to set default variables. */
	FWidgetBlueprintTableRow() : WidgetClass(nullptr)
	{
	}
};

/**
 * The table row for storing soft references to sound manager
 * classes.
 */
USTRUCT(Blueprintable)
struct FSoundManagersTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	/** Soft reference to widget blueprint class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ACPP_SoundManager> SoundManagerClass;

	/** The constructor to set default variables. */
	FSoundManagersTableRow() : SoundManagerClass(nullptr)
	{
	}
};

/** Container class for table row structs. */
UCLASS()
class CATPLATFORMER_API UTableRows : public UObject
{
	GENERATED_BODY()
};
