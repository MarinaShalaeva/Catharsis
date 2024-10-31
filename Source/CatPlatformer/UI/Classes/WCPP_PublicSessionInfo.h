// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"

#ifndef WCPP_WIDGETPARENT_H
#define WCPP_WIDGETPARENT_H
#include "CatPlatformer/UI/Classes/WCPP_WidgetParent.h"
#endif
class UWCPP_WidgetParent;
class UTextBlock;
class UButton;
class FOnlineSessionSearch;

#include "WCPP_PublicSessionInfo.generated.h"

DECLARE_DELEGATE_FourParams(FPassSessionInfo,
                            const int32 /* SessionNumber */,
                            const FUniqueNetIdRepl& /* UserId */,
                            const FName& /* SessionName */,
                            const FOnlineSessionSearchResult& /* SearchResult */);

/**
 * Widget class for representing one public session.
 */
UCLASS()
class CATPLATFORMER_API UWCPP_PublicSessionInfo : public UWCPP_WidgetParent
{
	GENERATED_BODY()

	//================Overriden functions==========================

	/**
	 * The constructor to set default variables.
	 * @param ObjectInitializer Internal class to finalize
	 * UObject creation.
	 */
	UWCPP_PublicSessionInfo(const FObjectInitializer& ObjectInitializer);

	/**
	 * Function that stores logic that should be applied
	 * when the widget is created.
	 */
	virtual void NativeConstruct() override;

	/**
	 * Function that stores logic that should be applied
	 * when the widget starts destroying.
	 */
	virtual void NativeDestruct() override;

public:
	/**
	 * Delegate for sending information about the associated
	 * with this widget online session.
	 */
	FPassSessionInfo PassSessionInfoDelegate;

	/** Number of session. */
	int32 SessionNumber;

	/** ID of the online session's owner. */
	FUniqueNetIdRepl UserId;

	/** The name of the session. */
	FName SessionName;

	/** Struct with additional info about the session. */
	FOnlineSessionSearchResult SearchResult;

protected:
	/**
	 * Button for passing information about the related
	 * session.
	 */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SessionButton;

	/** Text block for representing session's number. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_SessionNumber;

	/** Text block for representing session's name. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TB_SessionName;

	/**
	 * Function for sending information about the associated
	 * with this widget online session to the instance of
	 * UWCPP_ConfigureLevelParams class.
	 */
	UFUNCTION()
	void SessionButtonOnClick();
};
