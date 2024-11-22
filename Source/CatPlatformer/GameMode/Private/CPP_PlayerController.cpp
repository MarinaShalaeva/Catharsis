// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInputLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
class IOnlineSubsystem;

#ifndef CPP_SOUNDMANAGER_H
#define CPP_SOUNDMANAGER_H
#include "CatPlatformer/Sound/Classes/CPP_SoundManager.h"
#endif

#ifndef CPP_GAMEMODE_H
#define CPP_GAMEMODE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameMode.h"
#endif

ACPP_PlayerController::ACPP_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickableWhenPaused(true);

	GameInstanceRef = nullptr;
	CharacterRef = nullptr;

	DefaultMappingContext = nullptr;
	JumpAction = nullptr;
	PauseAction = nullptr;
	SprintAction = nullptr;
	AttackAction = nullptr;
	AnyKeyAction = nullptr;
	LB_Action = nullptr;
	RB_Action = nullptr;
	LT_Action = nullptr;
	RT_Action = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	LeftHorizontalScrollAction = nullptr;
	RightHorizontalScrollAction = nullptr;
	LeftVerticalScrollAction = nullptr;
	RightVerticalScrollAction = nullptr;

	bIsMainMenu = true;
	bCursorIsVisible = false;
	bIsGamepadMode = false;
	bIsFirstPlayer = true;
	bCharacterIsIdle = true;
	bCanBePaused = false;
	bIsPaused = false;
	bWaitingForReadinessToStartLevel = true;
}

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameInstanceRef = GetGameInstance<UCPP_GameInstance>();

	GetWorld()->GetTimerManager().SetTimer(TH_CheckVelocity,
	                                       this,
	                                       &ACPP_PlayerController::CheckVelocity,
	                                       0.05f,
	                                       true);
}

void ACPP_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CharacterWasPossessedDelegate.Clear();

	GameInstanceRef = nullptr;
	CharacterRef = nullptr;

	if (IsValid(SoundManagerRef))
	{
		SoundManagerRef->TrackWasSwitchedDelegate.Remove(DH_TrackWasSwitched);
		DH_TrackWasSwitched.Reset();
		SoundManagerRef = nullptr;
	}

	if (GetWorld()->GetTimerManager().TimerExists(TH_CheckVelocity))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CheckVelocity);
	}

	Super::EndPlay(EndPlayReason);
}

void ACPP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const UWorld* CurrentWorld = GetWorld();
	FString LevelName = CurrentWorld->GetMapName();
	LevelName.RemoveFromStart(CurrentWorld->StreamingLevelsPrefix);
	if (LevelName.Equals(TEXT("L_MainMenu")))
	{
		bIsMainMenu = true;
	}
	else
	{
		bIsMainMenu = false;
	}

	InputComponent->BindAction("AnyKey", IE_Pressed, this, &ACPP_PlayerController::AnyKeyPressed).
	                bExecuteWhenPaused = true;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings.
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (!bIsMainMenu)
		{
			// Moving.
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
			                                   &ACPP_PlayerController::Move);

			// Looking.
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
			                                   &ACPP_PlayerController::Look);

			// Jumping.
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
			                                   &ACPP_PlayerController::StartJump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
			                                   &ACPP_PlayerController::StopJump);

			// Sprinting.
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
			                                   &ACPP_PlayerController::StartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
			                                   &ACPP_PlayerController::StopSprint);

			// Attacking.
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this,
			                                   &ACPP_PlayerController::StartAttack);

			// Pause.
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this,
			                                   &ACPP_PlayerController::PauseKeyPressed);
		}

		// Gamepad Scrolling.
		EnhancedInputComponent->BindAction(LeftHorizontalScrollAction, ETriggerEvent::Triggered, this,
		                                   &ACPP_PlayerController::LeftHorizontalScrollByGamepad);
		EnhancedInputComponent->BindAction(LeftVerticalScrollAction, ETriggerEvent::Triggered, this,
		                                   &ACPP_PlayerController::LeftVerticalScrollByGamepad);
		EnhancedInputComponent->BindAction(RightHorizontalScrollAction, ETriggerEvent::Triggered, this,
		                                   &ACPP_PlayerController::RightHorizontalScrollByGamepad);
		EnhancedInputComponent->BindAction(RightVerticalScrollAction, ETriggerEvent::Triggered, this,
		                                   &ACPP_PlayerController::RightVerticalScrollByGamepad);
	}
}

void ACPP_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_PlayerController, CharacterRef);
	DOREPLIFETIME(ACPP_PlayerController, PlayerNetId);
}

void ACPP_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CharacterRef = Cast<ACPP_Character>(InPawn);

	if (IsValid(CharacterRef))
	{
		CharacterRef->ChangeCurrentSpeed(false);
		BroadcastCharacterWasPossessedDelegate(CharacterRef);
		if (!CharacterRef->bIsWaiting && bCharacterIsIdle &&
			CharacterRef->GetCharacterMovement()->Velocity.IsZero())
		{
			bCharacterIsIdle = false;
		}
	}
}

void ACPP_PlayerController::BroadcastCharacterWasPossessedDelegate_Implementation(ACPP_Character* InCharacter)
{
	CharacterWasPossessedDelegate.Broadcast(InCharacter);
}

void ACPP_PlayerController::Move(const FInputActionValue& Value)
{
	if (!bIsPaused && IsValid(CharacterRef) &&
		!CharacterRef->bIsAttacking &&
		!CharacterRef->bIsReceivingDamage)
	{
		// Input is a Vector2D.
		const FVector2D MovementVector = Value.Get<FVector2D>();

		if (!bWaitingForReadinessToStartLevel)
		{
			// Find out which way is forward.
			const FRotator YawRotation(0, GetControlRotation().Yaw, 0);

			// Get forward vector.
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// Get right vector. 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// Add movement.
			CharacterRef->AddMovementInput(ForwardDirection, MovementVector.Y);
			CharacterRef->AddMovementInput(RightDirection, MovementVector.X);
			if (!CharacterRef->bIsJumping)
			{
				if (CharacterRef->GetIsOnGrass())
				{
					CharacterRef->PlaySound(CharacterRef->GetSprintNow()
						                        ? ECatSoundState::GrassRun
						                        : ECatSoundState::GrassWalk);
				}
				else
				{
					CharacterRef->PlaySound(CharacterRef->GetSprintNow()
						                        ? ECatSoundState::BasicRun
						                        : ECatSoundState::BasicWalk);
				}
			}
			else
			{
				CharacterRef->StopAllMovementSounds();
			}
		}
	}
}

void ACPP_PlayerController::CheckVelocity()
{
	if (bIsMainMenu || !IsValid(CharacterRef))
		return;

	if (!CharacterRef->GetCharacterMovement()->Velocity.IsZero())
	{
		if (bCharacterIsIdle)
		{
			bCharacterIsIdle = false;
			EndCharacterWaiting();
		}
	}
	else
	{
		CharacterRef->StopAllMovementSounds();
		if (!bCharacterIsIdle)
		{
			bCharacterIsIdle = true;
			GetWorld()->GetTimerManager().SetTimer(TH_CallWaitingCharacterPose,
			                                       this,
			                                       &ACPP_PlayerController::StartCharacterWaiting,
			                                       4.0f,
			                                       false);
		}
	}
}

void ACPP_PlayerController::StartCharacterWaiting()
{
	CharacterRef->SetIsWaiting(true);
}

void ACPP_PlayerController::EndCharacterWaiting()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallWaitingCharacterPose))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallWaitingCharacterPose);
	}
	CharacterRef->SetIsWaiting(false);
}

void ACPP_PlayerController::Look(const FInputActionValue& Value)
{
	if (!bIsPaused && IsValid(CharacterRef))
	{
		// Input is a Vector2D.
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		// Add yaw and pitch input.
		CharacterRef->TurnAtRate(LookAxisVector.X);
		CharacterRef->LookUpAtRate(LookAxisVector.Y);
	}
}

void ACPP_PlayerController::LeftHorizontalScrollByGamepad(const FInputActionValue& Value)
{
	if (const float Rate = Value.Get<float>())
	{
		InputKeyWasPressedDelegate.Broadcast(EKeys::Gamepad_LeftX);
		GamepadHorizontalScrollDelegate.Broadcast(false, Rate);
	}
}

void ACPP_PlayerController::RightHorizontalScrollByGamepad(const FInputActionValue& Value)
{
	if (const float Rate = Value.Get<float>())
	{
		InputKeyWasPressedDelegate.Broadcast(EKeys::Gamepad_RightX);
		GamepadHorizontalScrollDelegate.Broadcast(true, Rate);
	}
}

void ACPP_PlayerController::LeftVerticalScrollByGamepad(const FInputActionValue& Value)
{
	if (const float Rate = Value.Get<float>())
	{
		InputKeyWasPressedDelegate.Broadcast(EKeys::Gamepad_LeftY);
		GamepadScrollDelegate.Broadcast(false, Rate);
	}
}

void ACPP_PlayerController::RightVerticalScrollByGamepad(const FInputActionValue& Value)
{
	if (const float Rate = Value.Get<float>())
	{
		InputKeyWasPressedDelegate.Broadcast(EKeys::Gamepad_RightY);
		GamepadScrollDelegate.Broadcast(true, Rate);
	}
}

void ACPP_PlayerController::StartJump()
{
	if (!bIsPaused && IsValid(CharacterRef) &&
		!bWaitingForReadinessToStartLevel &&
		!CharacterRef->bIsReceivingDamage)
	{
		CharacterRef->CustomStartJumping();
	}
}

void ACPP_PlayerController::StopJump()
{
	if (!bIsPaused && IsValid(CharacterRef) &&
		!bWaitingForReadinessToStartLevel &&
		!CharacterRef->bIsReceivingDamage)
	{
		CharacterRef->CustomStopJumping();
	}
}

void ACPP_PlayerController::StartSprint()
{
	if (!bIsPaused && IsValid(CharacterRef) && !bWaitingForReadinessToStartLevel)
	{
		CharacterRef->ChangeCurrentSpeed(true);
	}
}

void ACPP_PlayerController::StopSprint()
{
	if (!bIsPaused && IsValid(CharacterRef) && !bWaitingForReadinessToStartLevel)
	{
		CharacterRef->ChangeCurrentSpeed(false);
	}
}

void ACPP_PlayerController::StartAttack()
{
	if (!bIsPaused && IsValid(CharacterRef) && !bWaitingForReadinessToStartLevel)
	{
		CharacterRef->Server_Attack();
		if (bCharacterIsIdle)
		{
			bCharacterIsIdle = false;
			EndCharacterWaiting();
		}
	}
}

void ACPP_PlayerController::AnyKeyPressed(FKey PressedKey)
{
	if (UKismetInputLibrary::Key_IsGamepadKey(PressedKey) && !bIsGamepadMode)
	{
		bIsGamepadMode = true;
	}
	else if ((UKismetInputLibrary::Key_IsMouseButton(PressedKey) ||
			UKismetInputLibrary::Key_IsKeyboardKey(PressedKey))
		&& bIsGamepadMode)
	{
		bIsGamepadMode = false;
	}
	InputKeyWasPressedDelegate.Broadcast(PressedKey);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *(PressedKey.GetDisplayName()).ToString());
}

void ACPP_PlayerController::PauseKeyPressed()
{
	if (bIsPaused)
	{
		EndPause();
	}
	else
	{
		StartPause();
	}
}

void ACPP_PlayerController::StartPause()
{
	if (!bCanBePaused || IsPaused())
		return;

	if (GameInstanceRef.IsValid())
	{
		if (GameInstanceRef->GetPlayingModeAsInt() == 0 || GameInstanceRef->GetPlayingModeAsInt() == 1)
		{
			SetPause(true);
		}
	}

	ChangeCursorVisibility(true);
	if (PauseChangedDelegate.IsBound())
	{
		PauseChangedDelegate.Execute(true);
	}
	bIsPaused = true;
}

void ACPP_PlayerController::EndPause()
{
	if (GameInstanceRef.IsValid() &&
		(GameInstanceRef->GetPlayingModeAsInt() == 0 ||
			GameInstanceRef->GetPlayingModeAsInt() == 1))
	{
		SetPause(false);
	}
	ChangeCursorVisibility(false);
	if (PauseChangedDelegate.IsBound())
	{
		PauseChangedDelegate.Execute(false);
	}
	bIsPaused = false;
}

void ACPP_PlayerController::ChangeCursorVisibility(const bool bShouldBeVisible)
{
	bCursorIsVisible = bShouldBeVisible;

	this->bShowMouseCursor = bShouldBeVisible;
	this->bEnableClickEvents = bShouldBeVisible;
	this->bEnableMouseOverEvents = bShouldBeVisible;

	if (bShouldBeVisible)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this,
		                                                  nullptr,
		                                                  EMouseLockMode::DoNotLock);
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	}
}

void ACPP_PlayerController::CallLoadingInfoFromSaveFile()
{
	if (bIsFirstPlayer && IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		CharacterRef->GetPlayerStateRef()->LoadInfoFromSaveFile();
	}
}

void ACPP_PlayerController::CallCollectingInfoForSavingItToFile()
{
	if (bIsFirstPlayer)
	{
		if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
		{
			CharacterRef->GetPlayerStateRef()->SaveDataToFile();
		}
	}
	else
	{
		if (const ACPP_PlayerController* FirstPC = Cast<ACPP_PlayerController>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0));
			IsValid(FirstPC) &&
			IsValid(FirstPC->GetCharacterRef()) &&
			IsValid(FirstPC->GetCharacterRef()->GetPlayerStateRef()))
		{
			FirstPC->GetCharacterRef()->GetPlayerStateRef()->SaveDataToFile();
		}
	}
}

void ACPP_PlayerController::OnNetCleanup(UNetConnection* Connection)
{
	if (GetLocalRole() == ROLE_Authority && PlayerState != nullptr)
	{
		if (const ACPP_GameMode* GameMode = Cast<ACPP_GameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->PreLogout(this);
		}
	}

	Super::OnNetCleanup(Connection);
}

void ACPP_PlayerController::ClientDestroyOnlineSession_Implementation()
{
	if (GameInstanceRef.IsValid())
	{
		GameInstanceRef->DestroySessionOnClientCustom();
	}
}

void ACPP_PlayerController::TrackWasSwitched()
{
	if (!IsValid(SoundManagerRef))
		return;

	ACPP_PlayerState* PS = nullptr;
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		PS = CharacterRef->GetPlayerStateRef();
	}
	else if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		PS = GetPlayerState<ACPP_PlayerState>();
	}

	if (IsValid(PS))
	{
		if (SoundManagerRef->GetGeneralNumberOfTracks() == 3) // Main Menu.
		{
			PS->Set_MM_LastTrackNumber(SoundManagerRef->GetCurrentTrackNumber());
		}
		else // Level.
		{
			PS->Set_L_LastTrackNumber(SoundManagerRef->GetCurrentTrackNumber());
		}
		PS->CollectAudioInfoForSavingItToFile();
	}
}

void ACPP_PlayerController::ChangeInputEnabling(const bool bShouldEnable)
{
	if (bShouldEnable)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}

int32 ACPP_PlayerController::GetMaxOpenedLevelNumber()
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		return CharacterRef->GetPlayerStateRef()->GetMaxOpenedLevelNumber();
	}
	return 1;
}

void ACPP_PlayerController::SetCustomUserName(const FString& NewName)
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		CharacterRef->GetPlayerStateRef()->SetUserName(NewName);
	}
	else if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		GetPlayerState<ACPP_PlayerState>()->SetUserName(NewName);
	}
}

FString ACPP_PlayerController::GetCustomUserName() const
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		return CharacterRef->GetPlayerStateRef()->GetUserName();
	}
	if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		return GetPlayerState<ACPP_PlayerState>()->GetUserName();
	}
	return FString();
}

int32 ACPP_PlayerController::GetCustomUserScore() const
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		return CharacterRef->GetPlayerStateRef()->GetUserScore();
	}
	if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		return GetPlayerState<ACPP_PlayerState>()->GetUserScore();
	}
	return 0;
}

uint32 ACPP_PlayerController::GetCustomCurrentLevelDeathsNumber() const
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		return CharacterRef->GetPlayerStateRef()->GetCurrentLevelDeathsNumber();
	}
	if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		return GetPlayerState<ACPP_PlayerState>()->GetCurrentLevelDeathsNumber();
	}
	return 0;
}

float ACPP_PlayerController::GetCustomTimeOnLevel() const
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		return CharacterRef->GetPlayerStateRef()->GetTimeOnLevel();
	}
	if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		return GetPlayerState<ACPP_PlayerState>()->GetTimeOnLevel();
	}
	return 0;
}

bool ACPP_PlayerController::GetSavedData(FSaveSlot& OutSaveSlot) const
{
	bool bSuccess = false;
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		bSuccess = true;
		CharacterRef->GetPlayerStateRef()->GetTheSaveSlot(OutSaveSlot);
	}
	if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		bSuccess = true;
		GetPlayerState<ACPP_PlayerState>()->GetTheSaveSlot(OutSaveSlot);
	}
	return bSuccess;
}

void ACPP_PlayerController::SetPlayerIsReadyToStartGame(const bool bIsReady) const
{
	if (IsValid(CharacterRef) && IsValid(CharacterRef->GetPlayerStateRef()))
	{
		CharacterRef->GetPlayerStateRef()->SetPlayerIsReadyToStartGame(bIsReady);
	}
	else if (IsValid(GetPlayerState<ACPP_PlayerState>()))
	{
		GetPlayerState<ACPP_PlayerState>()->SetPlayerIsReadyToStartGame(bIsReady);
	}
}

void ACPP_PlayerController::SetIsGamepadMode(bool bNewValue)
{
	bIsGamepadMode = bNewValue;
}

void ACPP_PlayerController::SetSoundManagerRef(ACPP_SoundManager* NewValue)
{
	if (SoundManagerRef != nullptr && IsValid(SoundManagerRef) &&
		SoundManagerRef->TrackWasSwitchedDelegate.IsBoundToObject(this))
	{
		SoundManagerRef->TrackWasSwitchedDelegate.Remove(DH_TrackWasSwitched);
		DH_TrackWasSwitched.Reset();
	}

	SoundManagerRef = NewValue;
	if (SoundManagerRef != nullptr && IsValid(SoundManagerRef))
	{
		DH_TrackWasSwitched = SoundManagerRef->TrackWasSwitchedDelegate.AddUObject(
			this, &ACPP_PlayerController::TrackWasSwitched);
		TrackWasSwitched();
	}
}
