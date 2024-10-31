// (c) M. A. Shalaeva, 2024

#include "../Classes/WCPP_PublicSessionInfo.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UWCPP_PublicSessionInfo::UWCPP_PublicSessionInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), SessionNumber(0),
	  SessionButton(nullptr), TB_SessionNumber(nullptr),
	  TB_SessionName(nullptr)
{
}

void UWCPP_PublicSessionInfo::NativeConstruct()
{
	Super::NativeConstruct();

	TB_SessionNumber->SetText(FText::FromString(FString::FromInt(SessionNumber)));
	TB_SessionName->SetText(FText::FromName(SessionName));

	SessionButton->OnClicked.AddDynamic(this, &UWCPP_PublicSessionInfo::SessionButtonOnClick);
}

void UWCPP_PublicSessionInfo::NativeDestruct()
{
	SessionButton->OnClicked.RemoveDynamic(this, &UWCPP_PublicSessionInfo::SessionButtonOnClick);

	Super::NativeDestruct();
}

void UWCPP_PublicSessionInfo::SessionButtonOnClick()
{
	if (PassSessionInfoDelegate.IsBound())
	{
		PassSessionInfoDelegate.Execute(SessionNumber, UserId, SessionName, SearchResult);
	}
}
