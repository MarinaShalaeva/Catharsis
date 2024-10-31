// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_GameState.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerController.h"
#endif
class ACPP_PlayerController;

#ifndef CPP_GAMEMODE_H
#define CPP_GAMEMODE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameMode.h"
#endif
class ACPP_GameMode;

ACPP_GameState::ACPP_GameState()
{
	PrimaryActorTick.bCanEverTick = true;
	SkyMaterialIndex = 0;
	LevelNumber = 1;
}

void ACPP_GameState::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_GameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACPP_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_GameState, LevelNumber);
	DOREPLIFETIME(ACPP_GameState, SkyMaterialIndex);
}

void ACPP_GameState::SetLevelNumber_Implementation(const int32 NewValue)
{
	LevelNumber = NewValue;
	switch (LevelNumber)
	{
	case 1:
	case 2:
		{
			SkyMaterialIndex = 0;
			break;
		}
	case 3:
		{
			SkyMaterialIndex = 1;
			break;
		}
	case 4:
	case 5:
		{
			SkyMaterialIndex = 2;
			break;
		}
	case 6:
		{
			SkyMaterialIndex = FMath::RandRange(0, 2);
			break;
		}
	default:
		break;
	}
	ED_ChangeSkyMaterial.Broadcast(SkyMaterialIndex);
}

void ACPP_GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	Server_AddPlayerState(PlayerState);
}

void ACPP_GameState::Server_AddPlayerState_Implementation(APlayerState* PlayerState)
{
	Multicast_AddPlayerState(PlayerState);
}

void ACPP_GameState::Multicast_AddPlayerState_Implementation(APlayerState* PlayerState)
{
	if (ACPP_PlayerState* PS = Cast<ACPP_PlayerState>(PlayerState);
		IsValid(PS) && !PS->PlayerIsReadyForGameDelegate.IsBoundToObject(this))
	{
		PS->PlayerIsReadyForGameDelegate.AddUObject(this, &ACPP_GameState::PlayerCanStartTheLevel);
	}
	PlayersNumberChangedDelegate.Broadcast(PlayerArray.Num());
}

void ACPP_GameState::RemovePlayerState(APlayerState* PlayerState)
{
	if (ACPP_PlayerState* PS = Cast<ACPP_PlayerState>(PlayerState);
		IsValid(PS) && PS->PlayerIsReadyForGameDelegate.IsBoundToObject(this))
	{
		PS->PlayerIsReadyForGameDelegate.RemoveAll(this);
	}

	Super::RemovePlayerState(PlayerState);

	PlayersNumberChangedDelegate.Broadcast(PlayerArray.Num());
}

void ACPP_GameState::UpdateAllCharactersAppearance_Implementation()
{
	for (const auto& PS : PlayerArray)
	{
		Cast<ACPP_PlayerState>(PS)->UpdateCatsColorUpToSavedIndex();
	}
}

void ACPP_GameState::CallLoadingScreenClosing_Implementation()
{
	Multicast_CallLoadingScreenClosing();
}

void ACPP_GameState::Multicast_CallLoadingScreenClosing_Implementation()
{
	for (const auto& PS : PlayerArray)
	{
		Cast<ACPP_PlayerState>(PS)->DestroyLoadingScreenDelegate.Broadcast(true);
	}
}

void ACPP_GameState::PlayerCanStartTheLevel_Implementation(ACPP_PlayerState* PlayerState)
{
	Multicast_PlayerCanStartTheLevel(PlayerState);
}

void ACPP_GameState::Multicast_PlayerCanStartTheLevel_Implementation(ACPP_PlayerState* PlayerState)
{
	for (const auto& PS : PlayerArray)
	{
		if (!Cast<ACPP_PlayerState>(PS.Get())->bIsReadyToStart)
			return;
	}
	NotifyThatLevelCanBeStarted();
}

void ACPP_GameState::NotifyThatLevelCanBeStarted_Implementation()
{
	Multicast_NotifyThatLevelCanBeStarted();
}

void ACPP_GameState::Multicast_NotifyThatLevelCanBeStarted_Implementation()
{
	for (const auto& PS : PlayerArray)
	{
		Cast<ACPP_PlayerState>(PS)->ShouldBeginCountdownToStartLevelDelegate.Broadcast();
	}
}

void ACPP_GameState::AddUserScore_Implementation(ACPP_PlayerState* PlayerState, const int32 ScoreToAdd)
{
	if (IsValid(PlayerState))
	{
		TArray<APawn*> Players{};
		TArray<FTransform> PlayersTransforms{};
		if (ScoreToAdd == -5)
		{
			for (const auto& PS : PlayerArray)
			{
				Players.Emplace(PS->GetPawn());
				PlayersTransforms.Emplace(PS->GetPawn()->GetActorTransform());
			}
		}
		Multicast_AddUserScore(PlayerState, ScoreToAdd, Players, PlayersTransforms);
	}
}

void ACPP_GameState::Multicast_AddUserScore_Implementation(ACPP_PlayerState* PlayerState, const int32 ScoreToAdd,
                                                           const TArray<APawn*>& Players,
                                                           const TArray<FTransform>& PlayersTransforms)
{
	PlayerState->AddNewPointsToUserScore(ScoreToAdd);
	if (ScoreToAdd == -5)
	{
		for (int32 i = 0; i < Players.Num(); i++)
		{
			Players[i]->SetActorLocation(PlayersTransforms[i].GetLocation());
			Players[i]->SetActorRotation(PlayersTransforms[i].GetRotation());
		}
	}
	if (PlayerState->ScoreChangedDelegate.IsBound())
	{
		PlayerState->ScoreChangedDelegate.Execute(ScoreToAdd);
	}
}

void ACPP_GameState::LevelWasEnded_Implementation()
{
	Multicast_LevelWasEnded();
}

void ACPP_GameState::Multicast_LevelWasEnded_Implementation()
{
	TArray<ACPP_PlayerState*> OutWinners;
	GetTheWinner(OutWinners);
	SpawnVictoryFireworks(OutWinners);

	for (const auto& PS : PlayerArray)
	{
		if (ACPP_PlayerState* PlayerState = Cast<ACPP_PlayerState>(PS);
			IsValid(PlayerState))
		{
			PlayerState->SetMaxOpenedLevelNumber(LevelNumber + 1 > PlayerState->GetMaxOpenedLevelNumber()
				                                     ? LevelNumber + 1
				                                     : PlayerState->GetMaxOpenedLevelNumber());

			bool bIsWinner = false;
			for (const auto& Winner : OutWinners)
			{
				if (PlayerState == Winner)
				{
					bIsWinner = true;
					break;
				}
			}
			if (UCPP_GameInstance* GI = GetGameInstance<UCPP_GameInstance>())
			{
				if (GI->GetPlayingModeAsInt() == 2 ||
					GI->GetPlayingModeAsInt() == 3 ||
					GI->GetPlayingModeAsInt() == 4)
				{
					if (bIsWinner)
					{
						PlayerState->IncrementOnlineLevelsWinsNumber();
					}
					else
					{
						PlayerState->IncrementOnlineLossesNumber();
					}
				}
				if (bIsWinner && PlayerState->bIsFirstPlayer)
				{
					PlayerState->TryToUpdateBestResult(GI->GetLevelNumber(),
					                                   PlayerState->GetTimeOnLevel());
				}
			}
			PlayerState->IncrementNumberOfEndedLevels();
			PlayerState->AddCurrentScoreToGeneralScore();
			if (PlayerState->LevelWasEndedDelegate.IsBound())
			{
				PlayerState->LevelWasEndedDelegate.Broadcast(bIsWinner);
			}
			PlayerState->SaveDataToFile();
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT( "ACPP_GameState::Multicast_LevelWasEnded_Implementation,"
				       " PlayerState is not valid"));
		}
	}
}


void ACPP_GameState::GetTheWinner(TArray<ACPP_PlayerState*>& OutWinners)
{
	OutWinners.Empty();

	TArray<ACPP_PlayerState*> MaxScorePlayers;
	int32 MaxScore = 0;
	for (const auto& PS : PlayerArray)
	{
		if (ACPP_PlayerState* PlayerState = Cast<ACPP_PlayerState>(PS);
			IsValid(PlayerState))
		{
			if (MaxScorePlayers.Num() == 0)
			{
				MaxScorePlayers.Emplace(PlayerState);
				MaxScore = PlayerState->GetUserScore();
			}
			else
			{
				if (PlayerState->GetUserScore() > MaxScore)
				{
					MaxScorePlayers.Empty();
					MaxScorePlayers.Emplace(PlayerState);
					MaxScore = PlayerState->GetUserScore();
				}
				else if (PlayerState->GetUserScore() == MaxScore)
				{
					MaxScorePlayers.Emplace(PlayerState);
				}
			}
		}
	}

	if (MaxScorePlayers.Num() > 1)
	{
		TArray<ACPP_PlayerState*> MinDeathsPlayers;
		uint32 MinDeathNumber = 0;

		for (const auto& PlayerState : MaxScorePlayers)
		{
			if (MinDeathsPlayers.Num() == 0)
			{
				MinDeathsPlayers.Emplace(PlayerState);
				MinDeathNumber = PlayerState->GetCurrentLevelDeathsNumber();
			}
			else
			{
				if (PlayerState->GetCurrentLevelDeathsNumber() < MinDeathNumber)
				{
					MinDeathsPlayers.Empty();
					MinDeathsPlayers.Emplace(PlayerState);
					MinDeathNumber = PlayerState->GetCurrentLevelDeathsNumber();
				}
				else if (PlayerState->GetCurrentLevelDeathsNumber() == MinDeathNumber)
				{
					MinDeathsPlayers.Emplace(PlayerState);
				}
			}
		}

		for (const auto& PlayerState : MinDeathsPlayers)
		{
			OutWinners.Emplace(PlayerState);
		}
	}
	else if (MaxScorePlayers.Num() == 1)
	{
		OutWinners.Emplace(MaxScorePlayers[0]);
	}
}

void ACPP_GameState::Multicast_CallSessionEnding_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(*It);
			IsValid(PC) && !PC->IsLocalController())
		{
			PC->ClientDestroyOnlineSession();
		}
	}
}

void ACPP_GameState::SpawnVictoryFireworks_Implementation(const TArray<ACPP_PlayerState*>& InWinners)
{
	for (const auto Winner : InWinners)
	{
		const APawn* Pawn = Winner->GetPawn();
		FVector FireworkLocation = Pawn->GetActorLocation();
		FireworkLocation.Z += 90.f;

		ACPP_VictoryFirework* Firework = GetWorld()->SpawnActorDeferred<ACPP_VictoryFirework>(
			VictoryFireworkClass,
			FTransform::Identity,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		UGameplayStatics::FinishSpawningActor(Firework, FTransform(FRotator(),
		                                                           FireworkLocation,
		                                                           FVector(1.0f)));
	}
}

void ACPP_GameState::StartNonSeamlessTravel_Implementation()
{
	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetAuthGameMode()))
	{
		if (const ACPP_GameMode* GameMode = Cast<ACPP_GameMode>(GetWorld()->GetAuthGameMode());
			IsValid(GameMode))
		{
			if (GameMode->StartTravelDelegate.IsBound())
			{
				bool bSuccess = GameMode->StartTravelDelegate.Execute(false);
			}
		}
	}
}
