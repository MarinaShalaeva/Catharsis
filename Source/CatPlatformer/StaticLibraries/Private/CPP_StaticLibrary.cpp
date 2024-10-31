// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_StaticLibrary.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#if PLATFORM_WINDOWS
/*#define WINVER 0x0501  
#define _WIN32_WINNT 0x0501

#define _WIN32_WINNT_WIN10_TH2
#define _WIN32_WINNT_WIN10_RS1*/
#define WIN32_LEAN_AND_MEAN
/*#ifndef NOMINMAX
#define NOMINMAX
#endif*/
/*#include <windows.h>
#include <memory>
#include "winuser.h"*/
#include <roapi.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include "windows.gaming.input.h"

using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Gaming::Input;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

#pragma comment(lib, "runtimeobject.lib")
#endif

#include "GameMapsSettings.h"

FText UCPP_StaticLibrary::GetTimeAsText(const FDateTime& DT)
{
	const FString Hour = DT.GetHour() / 10 == 0
		                     ? FString::Printf(TEXT("0%d"), DT.GetHour())
		                     : FString::FromInt(DT.GetHour());

	const FString Minute = DT.GetMinute() / 10 == 0
		                       ? FString::Printf(TEXT("0%d"), DT.GetMinute())
		                       : FString::FromInt(DT.GetMinute());

	return FText::FromString(FString::Printf(TEXT("%s:%s"), *Hour, *Minute));
}

FText UCPP_StaticLibrary::GetDateAsText(const FDateTime& DT)
{
	const FString Day = DT.GetDay() / 10 == 0
		                    ? FString::Printf(TEXT("0%d"), DT.GetDay())
		                    : FString::FromInt(DT.GetDay());

	const FString Month = DT.GetMonth() / 10 == 0
		                      ? FString::Printf(TEXT("0%d"), DT.GetMonth())
		                      : FString::FromInt(DT.GetMonth());

	return FText::FromString(FString::Printf(TEXT("%s.%s.%d"), *Day, *Month, DT.GetYear()));
}

FText UCPP_StaticLibrary::GetTextFromSeconds(const float SpentSeconds)
{
	TArray<FText> Arr;
	const int32 RoundSeconds = UKismetMathLibrary::Round(SpentSeconds);

	const int32 Hours = RoundSeconds / 3600;
	if (Hours != 0)
	{
		Arr.Emplace(FText::AsNumber(Hours));
		Arr.Emplace(FText::AsCultureInvariant(TEXT(":")));
	}

	const int32 Minutes = (RoundSeconds / 60) - 60 * (RoundSeconds / 3600);

	if (Minutes / 10 == 0)
	{
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
	}
	Arr.Emplace(FText::AsNumber(Minutes));
	Arr.Emplace(FText::AsCultureInvariant(TEXT(":")));

	const int32 Seconds = RoundSeconds % 60;
	if (Seconds / 10 == 0)
	{
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
	}

	Arr.Emplace(FText::AsNumber(FGenericPlatformMath::TruncToInt
		(SpentSeconds - (Minutes * 60 + Hours * 3600))));

	return FText::Join(FText::AsCultureInvariant(TEXT("")), Arr);
}

FText UCPP_StaticLibrary::GetTextFromSeconds(const uint32 SpentSeconds)
{
	TArray<FText> Arr;

	const uint32 Hours = SpentSeconds / 3600;
	if (Hours != 0)
	{
		Arr.Emplace(FText::FromString(FString::Printf(TEXT("%u"), Hours)));
		Arr.Emplace(FText::AsCultureInvariant(TEXT(":")));
	}

	const uint32 Minutes = (SpentSeconds / 60) - 60 * (SpentSeconds / 3600);

	if (Minutes / 10 == 0)
	{
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
	}
	Arr.Emplace(FText::FromString(FString::Printf(TEXT("%u"), Minutes)));
	Arr.Emplace(FText::AsCultureInvariant(TEXT(":")));

	const uint32 Seconds = SpentSeconds % 60;
	if (Seconds / 10 == 0)
	{
		Arr.Emplace(FText::AsCultureInvariant(TEXT("0")));
	}

	Arr.Emplace(FText::FromString(FString::Printf(TEXT("%u"), SpentSeconds - (Minutes * 60 + Hours * 3600))));

	return FText::Join(FText::AsCultureInvariant(TEXT("")), Arr);
}

FText UCPP_StaticLibrary::GetTextFromSecondsWithMilliseconds(const float SpentSeconds)
{
	TArray<FText> Arr;

	Arr.Emplace(GetTextFromSeconds(SpentSeconds));

	FText WholePart, Reminder;
	GetWholePartAndRemainderAsText(SpentSeconds, 2, WholePart, Reminder);

	Arr.Emplace(FText::AsCultureInvariant(TEXT(",")));
	Arr.Emplace(Reminder);

	return FText::Join(FText::AsCultureInvariant(TEXT("")), Arr);
}

void UCPP_StaticLibrary::GetWholePartAndRemainderAsText(float TheFloat, const int32 Precision,
                                                        FText& OutWholePart, FText& OutReminder)
{
	if (const float Rounded = roundf(TheFloat);
		FMath::Abs(TheFloat - Rounded) < FMath::Pow(10.0f, -1 * Precision))
	{
		TheFloat = Rounded;
	}

	const int32 Whole = static_cast<int32>(TheFloat);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = 0;
	NumberFormat.MaximumFractionalDigits = 0;

	OutWholePart = FText::AsNumber(Whole, &NumberFormat);
	OutReminder = FText::AsNumber((TheFloat - Whole) * 100, &NumberFormat);
}

bool UCPP_StaticLibrary::IsAnyGamepadConnected()
{
	if (const TSharedPtr<GenericApplication> GenericApplicationPtr = FSlateApplication::Get().GetPlatformApplication();
		GenericApplicationPtr.IsValid() &&
		GenericApplicationPtr->IsGamepadAttached())
	{
		return true;
	}
	return false;
}

int32 UCPP_StaticLibrary::GetConnectedGamepadsNumber()
{
	uint32_t GamepadCount = 0;

#if PLATFORM_WINDOWS
	RoInitialize(RO_INIT_MULTITHREADED);

	ComPtr<IGamepadStatics> GamepadStatics;
	RoGetActivationFactory(HStringReference(L"Windows.Gaming.Input.Gamepad").Get(),
	                       __uuidof(IGamepadStatics), &GamepadStatics);

	ComPtr<IVectorView<Gamepad*>> Gamepads;
	GamepadStatics->get_Gamepads(&Gamepads);
	Gamepads->get_Size(&GamepadCount);

#else
	TArray<FInputDeviceId> OutInputDevices;
	GamepadCount = IPlatformInputDeviceMapper::Get().GetAllInputDevices(OutInputDevices);
	
#endif

	return GamepadCount;
}

void UCPP_StaticLibrary::MakeAllLocalPlayersUseGamepads()
{
	UGameMapsSettings::GetGameMapsSettings()->SetSkipAssigningGamepadToPlayer1(false);
}

void UCPP_StaticLibrary::MakeFirstLocalPlayerUseKeyboard()
{
	UGameMapsSettings::GetGameMapsSettings()->SetSkipAssigningGamepadToPlayer1(true);
}

void UCPP_StaticLibrary::OpenWebsiteByURL(const FString& URL)
{
	if (UKismetSystemLibrary::CanLaunchURL(URL))
	{
		UKismetSystemLibrary::LaunchURL(URL);
	}
}
