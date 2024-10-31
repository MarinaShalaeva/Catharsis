// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#ifndef CPP_PLATFORM_H
#define CPP_PLATFORM_H
#include "CatPlatformer/Platform/Classes/CPP_Platform.h"
#endif
class ACPP_Platform;

#ifndef CPP_BUFF_H
#define CPP_BUFF_H
#include "CatPlatformer/Buffs/Classes/CPP_Buff.h"
#endif
class ACPP_Buff;

#ifndef CPP_VICTORYACTOR_H
#define CPP_VICTORYACTOR_H
#include "CatPlatformer/GameWorldObjects/Classes/CPP_VictoryActor.h"
#endif
class ACPP_VictoryActor;

#ifndef CPP_GAMEINSTANCE_H
#define CPP_GAMEINSTANCE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameInstance.h"
#endif
class UCPP_GameInstance;
class UGameInstance;

#include "CPP_PlatformSpawner.generated.h"

/**
 * Class for working with platforms in the game world.
 */
UCLASS()
class CATPLATFORMER_API UCPP_PlatformSpawner : public UObject
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	UCPP_PlatformSpawner();

	/** Reference to the instance of UCPP_GameInstance class. */
	UPROPERTY()
	UCPP_GameInstance* GameInstanceRef;

public:
	/**
	 * Function for initializing GameInstanceRef variable.
	 * @param GI Game Instance reference.
	 */
	void InitGameInstanceRef(UGameInstance* GI);

	/**
	 * Function for spawning platforms on the map.
	 * @param WorldContext World instance.
	 * @param PlatformClasses All platforms' classes whose
	 * instances can be placed on the map.
	 * @param FinalPlatformClass The platform class, an
	 * instance of which the player needs to collect to
	 * complete the level.
	 * @param VictoryActorClass The class of the actor that
	 * needs to be collected to win the level.
	 * @param Length How many platforms should be in one row
	 * (X coordinate).
	 * @param Width How many platforms should be in one line
	 * (Y coordinate).
	 * @param PlatformsZCoordinateOffset Maximum offset for
	 * the Z coordinate of each platform.
	 * @param SpawnDistance The distance between neighboring
	 * platforms.
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlatforms(UWorld* WorldContext,
	                    const TArray<TSoftClassPtr<ACPP_Platform>>& PlatformClasses,
	                    const TSoftClassPtr<ACPP_Platform>& FinalPlatformClass,
	                    const TSoftClassPtr<ACPP_VictoryActor>& VictoryActorClass,
	                    const int32& Length,
	                    const int32& Width,
	                    const float& PlatformsZCoordinateOffset,
	                    const float& SpawnDistance);

	bool SpawnPlatforms_Validate(UWorld* WorldContext,
	                             const TArray<TSoftClassPtr<ACPP_Platform>>& PlatformClasses,
	                             const TSoftClassPtr<ACPP_Platform>& FinalPlatformClass,
	                             const TSoftClassPtr<ACPP_VictoryActor>& VictoryActorClass,
	                             const int32& Length,
	                             const int32& Width,
	                             const float& PlatformsZCoordinateOffset,
	                             const float& SpawnDistance);

	void SpawnPlatforms_Implementation(UWorld* WorldContext,
	                                   const TArray<TSoftClassPtr<ACPP_Platform>>& PlatformClasses,
	                                   const TSoftClassPtr<ACPP_Platform>& FinalPlatformClass,
	                                   const TSoftClassPtr<ACPP_VictoryActor>& VictoryActorClass,
	                                   const int32& Length,
	                                   const int32& Width,
	                                   const float& PlatformsZCoordinateOffset,
	                                   const float& SpawnDistance);

	/**
	 * Function for spawning buffs on the map.
	 * @param WorldContext World instance.
	 * @param BuffsClasses All buffs' classes whose
	 * instances can be placed on the map.
	 * @param BuffsSelectionProbabilities An array with
	 * probabilities with which buffs from the
	 * BuffsClasses array can be spawned on the map.
	 * @param TotalBuffsNumber The number of buffs that
	 * should be eventually spawned on the map.
	 * @param Length The length of the rectangle in which
	 * the platforms are located on the map (how many
	 * platforms should be in one row).
	 * @param Width The width of the rectangle in which the
	 * platforms are located on the map (how many platforms
	 * should be in one line).
	 * @param PlatformsSpawnDistance The distance between
	 * neighboring platforms.
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnBuffs(UWorld* WorldContext,
	                const TArray<TSoftClassPtr<ACPP_Buff>>& BuffsClasses,
	                const TArray<float>& BuffsSelectionProbabilities,
	                const int32& TotalBuffsNumber,
	                const int32& Length,
	                const int32& Width,
	                const float& PlatformsSpawnDistance);

	bool SpawnBuffs_Validate(UWorld* WorldContext,
	                         const TArray<TSoftClassPtr<ACPP_Buff>>& BuffsClasses,
	                         const TArray<float>& BuffsSelectionProbabilities,
	                         const int32& TotalBuffsNumber,
	                         const int32& Length,
	                         const int32& Width,
	                         const float& PlatformsSpawnDistance);

	void SpawnBuffs_Implementation(UWorld* WorldContext,
	                               const TArray<TSoftClassPtr<ACPP_Buff>>& BuffsClasses,
	                               const TArray<float>& BuffsSelectionProbabilities,
	                               const int32& TotalBuffsNumber,
	                               const int32& Length,
	                               const int32& Width,
	                               const float& PlatformsSpawnDistance);
};
