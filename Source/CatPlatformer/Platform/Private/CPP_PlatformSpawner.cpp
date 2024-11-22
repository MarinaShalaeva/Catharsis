// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_PlatformSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UCPP_PlatformSpawner::UCPP_PlatformSpawner()
{
	GameInstanceRef = nullptr;
}

void UCPP_PlatformSpawner::InitGameInstanceRef(UGameInstance* GI)
{
	if (IsValid(GI))
	{
		GameInstanceRef = Cast<UCPP_GameInstance>(GI);
	}
}

void UCPP_PlatformSpawner::InitGameModeRef(ACPP_GameMode* GM)
{
	if (IsValid(GM))
	{
		GameModeRef = GM;
	}
}

bool UCPP_PlatformSpawner::SpawnPlatforms_Validate(UWorld* WorldContext,
                                                   const TArray<TSoftClassPtr<ACPP_Platform>>& PlatformClasses,
                                                   const TSoftClassPtr<ACPP_Platform>& FinalPlatformClass,
                                                   const TSoftClassPtr<ACPP_VictoryActor>& VictoryActorClass,
                                                   const int32 Length, const int32 Width,
                                                   const float PlatformsZCoordinateOffset,
                                                   const float SpawnDistance)
{
	bool bSuccess = true;

	if (!WorldContext || Length <= 0 || Width <= 0 || PlatformClasses.Num() <= 0 || !GameInstanceRef.IsValid())
		bSuccess = false;

	return bSuccess;
}

void UCPP_PlatformSpawner::SpawnPlatforms_Implementation(UWorld* WorldContext,
                                                         const TArray<TSoftClassPtr<ACPP_Platform>>& PlatformClasses,
                                                         const TSoftClassPtr<ACPP_Platform>& FinalPlatformClass,
                                                         const TSoftClassPtr<ACPP_VictoryActor>& VictoryActorClass,
                                                         const int32 Length, const int32 Width,
                                                         const float PlatformsZCoordinateOffset,
                                                         const float SpawnDistance)
{
	const int32 MaximumPlatformIndex = PlatformClasses.Num() - 1;

	float StartCoordinateY;
	if (Width % 2 == 1)
	{
		StartCoordinateY = -(Width / 2) * SpawnDistance;
	}
	else
	{
		StartCoordinateY = -(Width / 2) * SpawnDistance + (SpawnDistance / 2);
	}

	float CoordinateX = 0.0f;
	for (int32 i = 0; i < Length; i++)
	{
		float CoordinateY = StartCoordinateY;
		for (int32 j = 0; j < Width; j++)
		{
			FTransform Transform = FTransform(FRotator(
				                                  0.0f,
				                                  90.0f * FMath::RandRange(0, 3),
				                                  0.0f),
			                                  FVector(
				                                  CoordinateX,
				                                  CoordinateY,
				                                  FMath::RandRange(PlatformsZCoordinateOffset * -1.0f,
				                                                   PlatformsZCoordinateOffset)),
			                                  FVector(1.0f));

			ACPP_Platform* Platform = WorldContext->SpawnActorDeferred<ACPP_Platform>(
				GameInstanceRef->GetActorClassBySoftReference
				(PlatformClasses[FMath::RandRange(0, MaximumPlatformIndex)]),
				FTransform::Identity,
				GameModeRef.IsValid() ? GameModeRef.Get() : nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			Platform->InitializeBasicVariables(Transform.GetLocation());

			UGameplayStatics::FinishSpawningActor(Platform, Transform);

			CoordinateY += SpawnDistance;
		}
		CoordinateX += SpawnDistance;
	}

	FVector FinalPlatformLocation = FVector(CoordinateX, 0.0f, 0.0f);

	FActorSpawnParameters Params;
	WorldContext->SpawnActor<ACPP_Platform>(GameInstanceRef->GetActorClassBySoftReference(FinalPlatformClass),
	                                        FinalPlatformLocation,
	                                        FRotator(0.0f),
	                                        Params);

	ACPP_VictoryActor* VictoryActor = WorldContext->SpawnActorDeferred<ACPP_VictoryActor>(
		GameInstanceRef->GetActorClassBySoftReference(VictoryActorClass),
		FTransform::Identity,
		GameModeRef.IsValid() ? GameModeRef.Get() : nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FVector VictoryActorLocation = FVector(FinalPlatformLocation.X + VictoryActor->SpawningOffset.X,
	                                       FinalPlatformLocation.Y + VictoryActor->SpawningOffset.Y,
	                                       FinalPlatformLocation.Z + VictoryActor->SpawningOffset.Z);
	FTransform VictoryActorTransform = FTransform(FRotator(), VictoryActorLocation,
	                                              FVector(1.0f));
	VictoryActor->InitializeBasicVariables(VictoryActorLocation);
	UGameplayStatics::FinishSpawningActor(VictoryActor, VictoryActorTransform);
}

bool UCPP_PlatformSpawner::SpawnBuffs_Validate(UWorld* WorldContext,
                                               const TArray<TSoftClassPtr<ACPP_Buff>>& BuffsClasses,
                                               const TArray<float>& BuffsSelectionProbabilities,
                                               const int32 TotalBuffsNumber,
                                               const int32 Length,
                                               const int32 Width,
                                               const float PlatformsSpawnDistance)
{
	bool bSuccess = true;

	if (!WorldContext || BuffsClasses.Num() <= 0 ||
		Length <= 0 || Width <= 0 || TotalBuffsNumber <= 0 ||
		BuffsSelectionProbabilities.Num() != BuffsClasses.Num() ||
		!GameInstanceRef.IsValid())
	{
		bSuccess = false;
	}

	return bSuccess;
}

void UCPP_PlatformSpawner::SpawnBuffs_Implementation(UWorld* WorldContext,
                                                     const TArray<TSoftClassPtr<ACPP_Buff>>& BuffsClasses,
                                                     const TArray<float>& BuffsSelectionProbabilities,
                                                     const int32 TotalBuffsNumber,
                                                     const int32 Length,
                                                     const int32 Width,
                                                     const float PlatformsSpawnDistance)
{
	float OriginX;
	if (Length % 2 == 1)
	{
		OriginX = (Length / 2) * PlatformsSpawnDistance;
	}
	else
	{
		OriginX = ((Length - 1) / 2) * PlatformsSpawnDistance + (PlatformsSpawnDistance / 2);
	}
	const float ExtentX = OriginX;

	float ExtentY;
	if (Width % 2 == 1)
	{
		ExtentY = (Width / 2) * PlatformsSpawnDistance + 70.0f;
	}
	else
	{
		ExtentY = ((Width - 1) / 2) * PlatformsSpawnDistance + (PlatformsSpawnDistance / 2) + 70.0f;
	}

	for (int32 i = 0; i < TotalBuffsNumber; i++)
	{
		const FVector RandomPointToSpawn = UKismetMathLibrary::RandomPointInBoundingBox(
			FVector(OriginX, 0.0, 70.0f),
			FVector(ExtentX, ExtentY, 30.0f));

		FTransform Transform = FTransform(FRotator(
			                                  0.0f,
			                                  FMath::RandRange(0.0f, 360.0f),
			                                  0.0f),
		                                  RandomPointToSpawn,
		                                  FVector(1.0f));

		const float RandomFloat = FMath::RandRange(0.0f, 1.0f);
		int32 Index = 0;
		float ProbabilityCounter = 0.0f;
		for (int32 j = 0; j < BuffsSelectionProbabilities.Num(); j++)
		{
			ProbabilityCounter += BuffsSelectionProbabilities[j];
			if (RandomFloat <= ProbabilityCounter)
			{
				Index = j;
				break;
			}
		}

		ACPP_Buff* Buff = WorldContext->SpawnActorDeferred<ACPP_Buff>(
			GameInstanceRef->GetActorClassBySoftReference(BuffsClasses[Index]),
			FTransform::Identity,
			GameModeRef.IsValid() ? GameModeRef.Get() : nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Buff->InitializeBasicVariables(RandomPointToSpawn);
		UGameplayStatics::FinishSpawningActor(Buff, Transform);
	}
}
