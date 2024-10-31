// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"

#ifndef CPP_PLAYERCONTROLLER_H
#define CPP_PLAYERCONTROLLER_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerController.h"
#endif

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif

#ifndef CPP_HUD_H
#define CPP_HUD_H
#include "CatPlatformer/UI/Classes/CPP_HUD.h"
#endif

#ifndef CPP_GAMESESSION_H
#define CPP_GAMESESSION_H
#include "CatPlatformer/Net/Classes/CPP_GameSession.h"
#endif

ACPP_GameMode::ACPP_GameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter> CharacterBPClass
		(TEXT("/Game/CatPlatformer/Player/BP_CatCharacter"));
	if (CharacterBPClass.Class != nullptr)
	{
		DefaultPawnClass = CharacterBPClass.Class;
	}
	else
	{
		DefaultPawnClass = ACPP_Character::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<AHUD> HUD_BP_Class
		(TEXT("/Game/CatPlatformer/UMG/BP_HUD"));
	if (HUD_BP_Class.Succeeded())
	{
		HUDClass = HUD_BP_Class.Class;
	}
	else
	{
		HUDClass = ACPP_HUD::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass
		(TEXT("/Game/CatPlatformer/Player/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	else
	{
		PlayerControllerClass = ACPP_PlayerController::StaticClass();
	}

	GameStateClass = ACPP_GameState::StaticClass();
	PlayerStateClass = ACPP_PlayerState::StaticClass();
	GameSessionClass = ACPP_GameSession::StaticClass();

	PlatformSpawner = nullptr;
	SpawnDistance = 100.0f;

	GameStateRef = nullptr;
	bLevelWasGenerated = false;

	bUseSeamlessTravel = false;
}

void ACPP_GameMode::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = GetGameState<ACPP_GameState>();
}

void ACPP_GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallLoadingScreenDestroying))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallLoadingScreenDestroying);
	}

	Super::EndPlay(EndPlayReason);
}

AActor* ACPP_GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* PlayerStart = Super::ChoosePlayerStart_Implementation(Player);

	if (!IsValid(GameStateRef))
	{
		GameStateRef = GetGameState<ACPP_GameState>();
	}
	if (IsValid(GameStateRef))
	{
		const int32 CurrentPlayersNumber = GameStateRef->PlayerArray.Num();

		const FName Tag(TEXT("Player") + FString::FormatAsNumber(CurrentPlayersNumber));
		TArray<AActor*> PlayerStartsArray;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),
		                                             APlayerStart::StaticClass(),
		                                             Tag,
		                                             PlayerStartsArray);
		if (PlayerStartsArray.Num() > 0)
		{
			PlayerStart = PlayerStartsArray[0];
		}
	}

	return PlayerStart;
}

void ACPP_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (CallRegisterPlayerDelegate.IsBound())
	{
		const FUniqueNetIdRepl NetId = CallRegisterPlayerDelegate.Execute(NewPlayer);
		if (NetId.IsValid())
		{
			Cast<ACPP_PlayerController>(NewPlayer)->SetPlayerNetId(NetId);
		}
	}

	if (GetWorld()->GetTimerManager().TimerExists(TH_CallUpdatingAllCharactersAppearance))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallUpdatingAllCharactersAppearance);
	}
	/*
	 * The timer is needed to ensure that by the time the
	 * function is called, the client has exactly managed
	 * to appear in the game world.
	 */
	GetWorld()->GetTimerManager().SetTimer(TH_CallUpdatingAllCharactersAppearance,
	                                       this,
	                                       &ACPP_GameMode::CallUpdatingAllCharactersAppearance,
	                                       0.1f,
	                                       false);
}

void ACPP_GameMode::PreLogout(const APlayerController* InPlayerController) const
{
	if (!IsValid(InPlayerController))
		return;

	// This code handles logins for both the local player (listen
	// server) and remote players (net connection).
	FUniqueNetIdRepl UniqueNetIdRepl;
	if (InPlayerController->IsLocalController())
	{
		const ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
		if (IsValid(LocalPlayer))
		{
			UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
		}
		else
		{
			const UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
			if (IsValid(RemoteNetConnection))
			{
				UniqueNetIdRepl = RemoteNetConnection->PlayerId;
			}
		}
	}
	else
	{
		const UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
		if (IsValid(RemoteNetConnection))
		{
			UniqueNetIdRepl = RemoteNetConnection->PlayerId;
		}
	}
	if (CallUnregisterPlayerDelegate.IsBound())
	{
		CallUnregisterPlayerDelegate.Execute(InPlayerController, UniqueNetIdRepl);
	}
}

void ACPP_GameMode::CallUpdatingAllCharactersAppearance_Implementation()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallUpdatingAllCharactersAppearance))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallUpdatingAllCharactersAppearance);
	}
	if (!IsValid(GameStateRef))
	{
		GameStateRef = GetGameState<ACPP_GameState>();
	}
	if (IsValid(GameStateRef))
	{
		GameStateRef->UpdateAllCharactersAppearance();
	}
}

void ACPP_GameMode::RespawnPlayer_Implementation(AController* Controller)
{
	ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(Controller);
	if (!PC) return;

	const FTransform Transform = FindPlayerStart(PC)->GetTransform();

	//UMaterialInstance* Color = nullptr;
	uint8 ColorIndex = 0;
	if (ACPP_PlayerState* PS = PC->GetPlayerState<ACPP_PlayerState>())
	{
		ColorIndex = PS->GetCatColorIndex();
	}

	if (PC->GetCharacterRef())
	{
		//Color = PC->GetCharacterRef()->CurrentColor;
		PC->GetCharacterRef()->Destroy();
	}
	else if (ACPP_Character* OldCharacter = Cast<ACPP_Character>(PC->GetPawn()))
	{
		//Color = OldCharacter->CurrentColor;
		OldCharacter->Destroy();
	}
	else if (APawn* Pawn = PC->GetPawn())
	{
		Pawn->Destroy();
	}

	if (ACPP_Character* Character = GetWorld()->SpawnActorDeferred<ACPP_Character>(
		DefaultPawnClass, FTransform::Identity,
		nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn))
	{
		//Character->CurrentColor = Color;
		//Character->ChangeCatsColor(Color);
		Character->SetCurrentColorMaterialByIndex(ColorIndex);
		Character->SetCurrentColorVariableByIndex(ColorIndex);
		/*if (const ACPP_PlayerState* PS = Character->GetPlayerState<ACPP_PlayerState>())
		{
			PS->UpdateCatsColorUpToSavedIndex();
		}*/

		UGameplayStatics::FinishSpawningActor(Character, Transform);
		PC->Possess(Character);
	}
}

void ACPP_GameMode::CallSessionEnding_Implementation()
{
	if (!IsValid(GameStateRef))
	{
		GameStateRef = GetGameState<ACPP_GameState>();
	}
	if (IsValid(GameStateRef))
	{
		GameStateRef->Multicast_CallSessionEnding();
	}
}

void ACPP_GameMode::CallLevelGeneration(const int32 InLevelNumber)
{
	bLevelWasGenerated = true;
	if (!IsValid(GameStateRef))
	{
		GameStateRef = GetGameState<ACPP_GameState>();
	}
	if (IsValid(GameStateRef))
	{
		GameStateRef->SetLevelNumber(InLevelNumber);
	}
	switch (InLevelNumber)
	{
	case 1:
		CallLevelGeneration(4, 4, 10.0f, 7);
		break;
	case 2:
		CallLevelGeneration(5, 5, 15.0f, 9);
		break;
	case 3:
		CallLevelGeneration(6, 7, 20.0f, 12);
		break;
	case 4:
		CallLevelGeneration(7, 7, 30.0f, 13);
		break;
	case 5:
		CallLevelGeneration(8, 7, 40.0f, 15);
		break;
	case 6:
		CallLevelGeneration(FMath::RandRange(9, 12),
		                    FMath::RandRange(8, 11),
		                    FMath::RandRange(40.0f, 50.0f),
		                    FMath::RandRange(16, 20));
		break;
	default:
		break;
	}
}

void ACPP_GameMode::CallLevelGeneration(const uint32 PlatformsLength,
                                        const uint32 PlatformsWidth,
                                        const float PlatformsZCoordinateOffset,
                                        const uint32 TotalBuffsNumber)
{
	if (!IsValid(PlatformSpawner))
	{
		PlatformSpawner = NewObject<UCPP_PlatformSpawner>();
		PlatformSpawner->InitGameInstanceRef(GetGameInstance());
	}
	UWorld* CurrentWorld = GetWorld();
	if (PlatformSpawner)
	{
		PlatformSpawner->SpawnPlatforms(CurrentWorld, PlatformClasses, FinalPlatform, VictoryActorClass,
		                                PlatformsLength, PlatformsWidth, PlatformsZCoordinateOffset,
		                                SpawnDistance);
		PlatformSpawner->SpawnBuffs(CurrentWorld,
		                            BuffsClasses, BuffsSelectionProbabilities, TotalBuffsNumber,
		                            PlatformsLength, PlatformsWidth, SpawnDistance);

		GetWorld()->GetTimerManager().SetTimer(TH_CallLoadingScreenDestroying,
		                                       this,
		                                       &ACPP_GameMode::StartDestroyingLoadingScreen,
		                                       2.5f,
		                                       false);
	}
}

void ACPP_GameMode::StartDestroyingLoadingScreen()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallLoadingScreenDestroying))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallLoadingScreenDestroying);
	}
	if (!IsValid(GameStateRef))
	{
		GameStateRef = GetGameState<ACPP_GameState>();
	}
	if (IsValid(GameStateRef))
	{
		GameStateRef->CallLoadingScreenClosing();
	}
}
