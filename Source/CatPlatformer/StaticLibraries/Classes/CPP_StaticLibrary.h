// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CPP_StaticLibrary.generated.h"

/** Enumeration for the current playlist's repeating mode. */
UENUM(Blueprintable)
enum class EPlaylistRepeatingMode : uint8
{
	RepeatPlaylist,
	RepeatOneTrack,
	NoRepeat
};

/**
 * Structure for storing information about the player that
 * should be saved.
 */
USTRUCT()
struct FSaveSlot
{
	GENERATED_BODY()

	/** The name chosen by the player for himself. */
	UPROPERTY(SaveGame)
	FString UserName;

	/**
	 * The highest opened level's number (or the amount of
	 * all opened levels).
	 */
	UPROPERTY(SaveGame)
	uint8 MaxOpenedLevelNumber;

	/** The cat's color index. */
	UPROPERTY(SaveGame)
	uint8 CatColorIndex;

	/** General number of started levels. */
	UPROPERTY(SaveGame)
	uint32 NumberOfStartedLevels;

	/**
	 * General number of levels that were both started and
	 * ended.
	 */
	UPROPERTY(SaveGame)
	uint32 NumberOfEndedLevels;

	/**
	 * The number of deaths of the player in all the levels
	 * he played.
	 */
	UPROPERTY(SaveGame)
	uint32 DeathsNumber;

	/**
	 * The number of performed by the player jumps in all
	 * the levels he played.
	 */
	UPROPERTY(SaveGame)
	uint32 JumpsNumber;

	/**
	 * The number of buffs and debuffs that were collected
	 * by the player.
	 */
	UPROPERTY(SaveGame)
	uint32 CollectedBuffsNumber;

	/**
	 * The number of NPCs that were killed by the player
	 * during all played levels.
	 */
	UPROPERTY(SaveGame)
	uint32 NPCsKilledNumber;

	/**
	 * The number of player's wins in all the online levels
	 * he has played.
	 */
	UPROPERTY(SaveGame)
	uint32 OnlineWinsNumber;

	/**
	 * The number of player's losses in all the online levels
	 * he has played.
	 */
	UPROPERTY(SaveGame)
	uint32 OnlineLossesNumber;

	/**
	 * The sum of the points collected by the player on all
	 * completed levels.
	 */
	UPROPERTY(SaveGame)
	uint32 GeneralScore;

	/** Seconds in game. */
	UPROPERTY(SaveGame)
	uint32 TimeInGame;

	/**
	 * The best number of seconds the player spent to
	 * complete the 1st level.
	 */
	UPROPERTY(SaveGame)
	float BestTimeLevel_1;

	/**
	 * The best number of seconds the player spent to
	 * complete the 2nd level.
	 */
	UPROPERTY(SaveGame)
	float BestTimeLevel_2;

	/**
	 * The best number of seconds the player spent to
	 * complete the 3rd level.
	 */
	UPROPERTY(SaveGame)
	float BestTimeLevel_3;

	/**
	 * The best number of seconds the player spent to
	 * complete the 4th level.
	 */
	UPROPERTY(SaveGame)
	float BestTimeLevel_4;

	/**
	 * The best number of seconds the player spent to
	 * complete the 5th level.
	 */
	UPROPERTY(SaveGame)
	float BestTimeLevel_5;

	/**
	 * The best number of seconds the player spent to
	 * complete the random level.
	 */
	UPROPERTY(SaveGame)
	float BestTimeLevel_6;

	/** Volume of the sounds. */
	UPROPERTY(SaveGame)
	float SFX_Volume;

	/** Volume of the music. */
	UPROPERTY(SaveGame)
	float Music_Volume;

	/**
	 * Array of the main menu tracks' numbers added to the
	 * playlist by the user.
	 */
	UPROPERTY(SaveGame)
	TArray<uint8> MM_ActiveTracksNumbers;

	/** Number of the last played main menu track. */
	UPROPERTY(SaveGame)
	uint8 MM_LastTrackNumber;

	/** Main menu tracks' repeating mode. */
	UPROPERTY(SaveGame)
	EPlaylistRepeatingMode MM_PlaylistRepeatingMode;

	/**
	 * Array of the level tracks' numbers added to the
	 * playlist by the user.
	 */
	UPROPERTY(SaveGame)
	TArray<uint8> L_ActiveTracksNumbers;

	/** Number of the last played level track. */
	UPROPERTY(SaveGame)
	uint8 L_LastTrackNumber;

	/** Level tracks' repeating mode. */
	UPROPERTY(SaveGame)
	EPlaylistRepeatingMode L_PlaylistRepeatingMode;

	/** The constructor to set default variables. */
	FSaveSlot() : UserName(FString()),
	              MaxOpenedLevelNumber(1), CatColorIndex(0),
	              NumberOfStartedLevels(0), NumberOfEndedLevels(0),
	              DeathsNumber(0), JumpsNumber(0), CollectedBuffsNumber(0), NPCsKilledNumber(0),
	              OnlineWinsNumber(0), OnlineLossesNumber(0), GeneralScore(0),
	              TimeInGame(0), BestTimeLevel_1(0.0f), BestTimeLevel_2(0.0f), BestTimeLevel_3(0.0f),
	              BestTimeLevel_4(0.0f), BestTimeLevel_5(0.0f), BestTimeLevel_6(0.0f),
	              SFX_Volume(1.0f), Music_Volume(1.0f),
	              MM_LastTrackNumber(1),
	              MM_PlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatPlaylist),
	              L_LastTrackNumber(1),
	              L_PlaylistRepeatingMode(EPlaylistRepeatingMode::RepeatPlaylist)
	{
		MM_ActiveTracksNumbers.Add(1);
		MM_ActiveTracksNumbers.Add(2);
		MM_ActiveTracksNumbers.Add(3);

		L_ActiveTracksNumbers.Add(1);
		L_ActiveTracksNumbers.Add(2);
		L_ActiveTracksNumbers.Add(3);
		L_ActiveTracksNumbers.Add(4);
		L_ActiveTracksNumbers.Add(5);
	}
};

/** Enumeration for the current playing mode. */
UENUM()
enum class EPlayingMode : uint8
{
	SinglePlayer,
	LocalMultiplayer,
	OnlineMultiplayerServer,
	OnlineMultiplayerClientInPrivateSession,
	OnlineMultiplayerClientInPublicSession
};

/** C++ library with static functions. */
UCLASS()
class CATPLATFORMER_API UCPP_StaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Function for creating an FText variable containing
	 * time in format HH:MM.
	 */
	static FText GetTimeAsText(const FDateTime& DT);

	/**
	 * Function for creating an FText variable containing
	 * date in format DD.MM.YYYY.
	 */
	static FText GetDateAsText(const FDateTime& DT);

	/**
	 * Function for getting number of seconds in format
	 * HH:MM:SS.
	 * @param SpentSeconds Float number of seconds.
	 * @return Number of seconds in format HH:MM:SS.
	 */
	static FText GetTextFromSeconds(const float SpentSeconds);

	/**
	 * Function for getting number of seconds in format
	 * HH:MM:SS.
	 * @param SpentSeconds Unsigned integer number of
	 * seconds.
	 * @return Number of seconds in format HH:MM:SS.
	 */
	static FText GetTextFromSeconds(const uint32 SpentSeconds);

	/**
	 * Function for getting number of seconds in format
	 * HH:MM:SS.
	 * @param SpentSeconds Number of seconds.
	 * @return Number of seconds in format HH:MM:SS,MS.
	 */
	static FText GetTextFromSecondsWithMilliseconds(const float SpentSeconds);

private:
	/**
	 * Function for obtaining separately the whole part and
	 * the remainder of a float number.
	 * @param TheFloat Input float value.
	 * @param Precision Number of decimal places.
	 * @param OutWholePart A whole part of the value.
	 * @param OutReminder A reminder of the value.
	 */
	static void GetWholePartAndRemainderAsText(float TheFloat, const int32 Precision,
	                                           FText& OutWholePart, FText& OutReminder);

public:
	/**
	 * Function for checking any gamepad's connection to the
	 * player's computer.
	 * @return Is any gamepad connected?
	 */
	UFUNCTION()
	static bool IsAnyGamepadConnected();

	/**
	 * Function for getting the number of connected to the
	 * player's computer gamepads.
	 * @return Number of connected gamepads.
	 */
	UFUNCTION()
	static int32 GetConnectedGamepadsNumber();

	/** Function for making all local players use gamepads. */
	UFUNCTION()
	static void MakeAllLocalPlayersUseGamepads();

	/**
	 * Function for making the first player use keyboard &
	 * mouse and other players use gamepads.
	 */
	UFUNCTION()
	static void MakeFirstLocalPlayerUseKeyboard();

	/**
	 * Function for opening website by URL.
	 * @param URL Website's URL to open.
	 */
	UFUNCTION()
	static void OpenWebsiteByURL(const FString& URL);
};
