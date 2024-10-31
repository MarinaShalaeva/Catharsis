// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_HUD.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

#ifndef WCPP_NOTIFICATION_H
#define WCPP_NOTIFICATION_H
#include "CatPlatformer/UI/Classes/WCPP_Notification.h"
#endif
class UWCPP_Notification;

#ifndef CPP_STATICWIDGETLIBRARY_H
#define CPP_STATICWIDGETLIBRARY_H
#include "CatPlatformer/StaticLibraries/Classes/CPP_StaticWidgetLibrary.h"
#endif

ACPP_HUD::ACPP_HUD(): GameInstanceRef(nullptr), PlayerControllerRef(nullptr),
                      PlayerStateRef(nullptr), WidgetBlueprintsDataTable(nullptr),
                      SoundManagersDataTable(nullptr),
                      Container_Widget(nullptr), ChooseSaveSlot_Widget(nullptr),
                      MainMenu_Widget(nullptr), Pause_Widget(nullptr),
                      LoadingScreen_Widget(nullptr), Level_Widget(nullptr),
                      EndLevel_Widget(nullptr)
{
}

void ACPP_HUD::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* CurrentWorld = GetWorld();
	GameInstanceRef = CurrentWorld->GetGameInstance<UCPP_GameInstance>();
	PlayerControllerRef = Cast<ACPP_PlayerController>(PlayerOwner.Get());

	InitializeContainerWidget();
	InitializeLoadingScreenWidget();

	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange,
	                                 FString::Printf(TEXT("IsValid(PlayerControllerRef) =  %hhd"),
	                                                 IsValid(PlayerControllerRef)));*/

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->PauseChangedDelegate.BindUObject(this, &ACPP_HUD::PauseModeChanged);
		PlayerControllerRef->CharacterWasPossessedDelegate.AddUObject(this,
		                                                              &ACPP_HUD::NewCharacterWasPossessed);
		PlayerControllerRef->CreateNotificationDelegate.BindUObject(this,
		                                                            &ACPP_HUD::CreateNewNotification);
		PlayerControllerRef->ChangeCursorVisibility(!PlayerControllerRef->GetIsGamepadMode());
	}

	FString LevelName = CurrentWorld->GetMapName();
	LevelName.RemoveFromStart(CurrentWorld->StreamingLevelsPrefix);
	if (LevelName.Equals(TEXT("L_MainMenu")))
	{
		if (IsValid(GameInstanceRef) && !GameInstanceRef->GetSaveManager()->bSaveGameObjectIsDeclared)
		{
			InitializeChooseSaveSlotWidget();
		}
	}

	if (IsValid(PlayerControllerRef))
	{
		if (IsValid(PlayerControllerRef->GetCharacterRef()))
		{
			NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetCharacterRef()));
		}
		else if (IsValid(PlayerControllerRef->GetPawn()))
		{
			NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetPawn()));
		}
	}

	DH_ControllerConnectionChanging = IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddUObject(
		this,
		&ACPP_HUD::OnControllerConnectionChanged);

	if (LevelName.Equals(TEXT("L_MainMenu")))
	{
		GameInstanceRef->SetPlayingModeAsInt(0);
	}
	if (IsValid(GameInstanceRef) &&
		(GameInstanceRef->GetPlayingModeAsInt() == 3 ||
			GameInstanceRef->GetPlayingModeAsInt() == 4))
	{
		if (IsValid(GetWorld()) &&
			GetWorld()->GetTimerManager().TimerExists(TH_DestroyLoadingScreen))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_DestroyLoadingScreen);
		}
		GetWorld()->GetTimerManager().SetTimer(
			TH_DestroyLoadingScreen,
			[&]()
			{
				if (IsValid(PlayerControllerRef))
				{
					if (IsValid(PlayerControllerRef->GetCharacterRef()))
					{
						NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetCharacterRef()));
					}
					else if (IsValid(PlayerControllerRef->GetPawn()))
					{
						NewCharacterWasPossessed(Cast<ACPP_Character>(PlayerControllerRef->GetPawn()));
					}
				}
				DestroyLoadingScreenWidget(!LevelName.Equals(TEXT("L_MainMenu")));
			},
			2.0f,
			false);
	}
}

void ACPP_HUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyLoadingScreenWidget(false);
	DestroyEndLevelWidget();
	DestroyMainMenuWidget();
	DestroyPauseWidget();
	DestroyLevelWidget();
	DestroyChooseSaveSlotWidget();
	DestroyContainerWidget();

	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->PauseChangedDelegate.Unbind();
		PlayerControllerRef->CreateNotificationDelegate.Unbind();
		PlayerControllerRef = nullptr;
	}

	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->LevelWasEndedDelegate.Remove(DH_LevelWasEnded);
		DH_LevelWasEnded.Reset();
		PlayerStateRef->DestroyLoadingScreenDelegate.Remove(DH_DestroyLoadingScreen);
		DH_DestroyLoadingScreen.Reset();
		PlayerStateRef = nullptr;
	}

	GameInstanceRef = nullptr;

	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().Remove(DH_ControllerConnectionChanging);

	Super::EndPlay(EndPlayReason);
}

void ACPP_HUD::NewCharacterWasPossessed_Implementation(ACPP_Character* NewCharacter)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black,
	                                 FString::Printf(TEXT("NewCharacterWasPossessed()")));*/
	ResetAllBuffsEffects();

	if (IsValid(NewCharacter))
	{
		if (!NewCharacter->PlayerStateChangedDelegate.IsBoundToObject(this))
		{
			NewCharacter->PlayerStateChangedDelegate.AddUObject(this, &ACPP_HUD::PlayerStateWasChanged);
			PlayerStateWasChanged(NewCharacter->GetPlayerState<ACPP_PlayerState>());
		}

		if (!NewCharacter->CollectBuffDelegate.IsBound())
		{
			NewCharacter->CollectBuffDelegate.BindUObject(this, &ACPP_HUD::BuffWasCollected);
		}

		if (!NewCharacter->ResetBuffEffectDelegate.IsBound())
		{
			NewCharacter->ResetBuffEffectDelegate.BindUObject(this, &ACPP_HUD::ShouldResetBuffEffect);
		}
	}
}

void ACPP_HUD::PlayerStateWasChanged(ACPP_PlayerState* NewPlayerState)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta,
	                                 FString::Printf(TEXT("PlayerStateWasChanged()")));*/
	if (!IsValid(NewPlayerState))
		return;

	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->LevelWasEndedDelegate.Remove(DH_LevelWasEnded);
		DH_LevelWasEnded.Reset();
		PlayerStateRef->DestroyLoadingScreenDelegate.Remove(DH_DestroyLoadingScreen);
		DH_DestroyLoadingScreen.Reset();
	}
	PlayerStateRef = NewPlayerState;
	DH_LevelWasEnded = PlayerStateRef->LevelWasEndedDelegate.AddUObject(this, &ACPP_HUD::LevelWasEnded);
	DH_DestroyLoadingScreen = PlayerStateRef->DestroyLoadingScreenDelegate.AddUObject(this,
		&ACPP_HUD::DestroyLoadingScreenWidget);
	const UWorld* CurrentWorld = GetWorld();
	FString LevelName = CurrentWorld->GetMapName();
	LevelName.RemoveFromStart(CurrentWorld->StreamingLevelsPrefix);

	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
	                                 FString::Printf(TEXT("LevelName = %s"),
	                                                 *LevelName));*/

	if (LevelName.Equals(TEXT("L_MainMenu")))
	{
		if (!IsValid(ChooseSaveSlot_Widget) || ChooseSaveSlot_Widget == nullptr)
		{
			InitializeMainMenuWidget();
		}
	}
	else if (LevelName.Equals(TEXT("L_Transition")))
	{
		InitializeLoadingScreenWidget();
	}
	else
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		                                 FString::Printf(TEXT("InitializeLevelWidget()")));*/
		InitializeLevelWidget();
	}
}

void ACPP_HUD::OnControllerConnectionChanged(EInputDeviceConnectionState NewConnectionState,
                                             FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
{
	if (NewConnectionState == EInputDeviceConnectionState::Connected)
	{
		CreateNewNotification(GamepadWasConnectedInscription, 3.5f);
	}
	else if (!UCPP_StaticLibrary::IsAnyGamepadConnected())
	{
		CreateNewNotification(AllGamepadsWereDisconnectedInscription, 3.5f);
		if (IsValid(PlayerControllerRef) && PlayerControllerRef->AllGamepadsWereDisconnectedDelegate.IsBound())
		{
			PlayerControllerRef->AllGamepadsWereDisconnectedDelegate.Broadcast();
		}
	}
	else
	{
		CreateNewNotification(GamepadWasDisconnectedInscription, 3.5f);
	}
}

void ACPP_HUD::CreateNewNotification(const FText& NewText, const float TimeToDisplay) const
{
	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		IsValid(Container_Widget))
	{
		const FName Row = FName(TEXT("Notification"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			if (UWCPP_Notification* NotificationWidget = CreateWidget<UWCPP_Notification>(PlayerControllerRef, Class);
				IsValid(NotificationWidget))
			{
				NotificationWidget->SetFlags(RF_StrongRefOnFrame);
				NotificationWidget->SetClosingTime(TimeToDisplay);
				NotificationWidget->SetNewNotificationText(NewText);
				Container_Widget->NotificationsScrollBox->AddChild(NotificationWidget);
			}
		}
	}
}

void ACPP_HUD::PauseModeChanged(const bool bIsPaused)
{
	if (bIsPaused)
	{
		InitializePauseWidget();
	}
	else
	{
		DestroyPauseWidget();
		if (IsValid(PlayerControllerRef) && !PlayerControllerRef->GetIsGamepadMode() &&
			IsValid(Level_Widget) && Level_Widget->bCursorShouldBeVisible)
		{
			PlayerControllerRef->ChangeCursorVisibility(true);
		}
	}
}

void ACPP_HUD::BuffWasCollected(const int32 BuffTypeId, USlateBrushAsset* BuffImage,
                                const float EffectDuration) const
{
	if (IsValid(Level_Widget))
	{
		Level_Widget->CreateOrUpdateBuffWidget(PlayerControllerRef, BuffTypeId, BuffImage, EffectDuration);
	}
}

void ACPP_HUD::ShouldResetBuffEffect(const int32 BuffTypeId) const
{
	if (IsValid(Level_Widget))
	{
		Level_Widget->DestroyBuffWidget(BuffTypeId);
	}
}

void ACPP_HUD::ResetAllBuffsEffects() const
{
	if (IsValid(Level_Widget))
	{
		Level_Widget->DestroyAllBuffsWidgets();
	}
}

void ACPP_HUD::InitializeContainerWidget()
{
	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		Container_Widget == nullptr)
	{
		const FName Row = FName(TEXT("Container"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			Container_Widget = CreateWidget<UWCPP_Container>(PlayerControllerRef, Class);
			if (IsValid(Container_Widget))
			{
				Container_Widget->SetFlags(RF_StrongRefOnFrame);
				Container_Widget->AddToPlayerScreen();
			}
		}
	}
}

void ACPP_HUD::DestroyContainerWidget()
{
	if (IsValid(Container_Widget))
	{
		Container_Widget->RemoveFromParent();
		Container_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeMainMenuWidget()
{
	if (IsValid(Container_Widget))
	{
		Container_Widget->BackgroundImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	DestroyLoadingScreenWidget(false);
	DestroyChooseSaveSlotWidget();

	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		MainMenu_Widget == nullptr)
	{
		const FName Row = FName(TEXT("MainMenu"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			MainMenu_Widget = CreateWidget<UWCPP_MainMenu>(PlayerControllerRef, Class);
			if (IsValid(MainMenu_Widget) && IsValid(Container_Widget))
			{
				MainMenu_Widget->SetFlags(RF_StrongRefOnFrame);
				MainMenu_Widget->SetGameInstanceRef(GameInstanceRef);
				MainMenu_Widget->SetPlayerControllerRef(PlayerControllerRef);
				MainMenu_Widget->ShouldOpenSaveSlotsPanelDelegate.BindUObject(
					this, &ACPP_HUD::InitializeChooseSaveSlotWidget);
				if (PlayerControllerRef->GetSoundManagerRef() == nullptr)
				{
					if (UClass* SoundManagerClass = GameInstanceRef->GetActorClassBySoftReference(
						UCPP_StaticWidgetLibrary::GetSoftReferenceToSoundManagerByRowName(
							SoundManagersDataTable,
							FName(TEXT("MainMenuSoundManager")))))
					{
						ACPP_SoundManager* TmpSoundManager = GetWorld()->SpawnActorDeferred<ACPP_SoundManager>(
							SoundManagerClass,
							FTransform::Identity,
							nullptr,
							nullptr,
							ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

						if (IsValid(TmpSoundManager))
						{
							TmpSoundManager->SetPlayerControllerRef(PlayerControllerRef);
							bool bSetDefaultValues = true;
							if (IsValid(PlayerControllerRef->GetCharacter()) &&
								IsValid(PlayerControllerRef->GetCharacter()->GetPlayerState<ACPP_PlayerState>()))
							{
								bSetDefaultValues = !PlayerControllerRef->GetCharacter()->
								                                          GetPlayerState<ACPP_PlayerState>()->
								                                          GetSaveFileWasCreated();
							}
							TmpSoundManager->ApplySettingsFromTheSaveFile(bSetDefaultValues, false);
							PlayerControllerRef->SetSoundManagerRef(TmpSoundManager);
							UGameplayStatics::FinishSpawningActor(TmpSoundManager, FTransform());
						}
					}
				}
				DestroyPauseWidget();
				Container_Widget->MainMenuAndPauseWidgetSizeBox->AddChild(MainMenu_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyMainMenuWidget()
{
	if (IsValid(MainMenu_Widget))
	{
		MainMenu_Widget->ShouldOpenSaveSlotsPanelDelegate.Unbind();
		MainMenu_Widget->RemoveFromParent();
		MainMenu_Widget = nullptr;

		if (IsValid(PlayerControllerRef))
		{
			ACPP_SoundManager* SoundManager = PlayerControllerRef->GetSoundManagerRef();
			PlayerControllerRef->SetSoundManagerRef(nullptr);
			if (IsValid(SoundManager))
			{
				SoundManager->Destroy();
			}
		}
	}
}

void ACPP_HUD::InitializeChooseSaveSlotWidget()
{
	if (IsValid(Container_Widget))
	{
		Container_Widget->BackgroundImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	DestroyLoadingScreenWidget(false);
	DestroyMainMenuWidget();

	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		ChooseSaveSlot_Widget == nullptr)
	{
		const FName Row = FName(TEXT("ChooseSave"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			ChooseSaveSlot_Widget = CreateWidget<UWCPP_ChooseSaveSlot>(PlayerControllerRef, Class);
			if (IsValid(ChooseSaveSlot_Widget) && IsValid(Container_Widget))
			{
				ChooseSaveSlot_Widget->SetFlags(RF_StrongRefOnFrame);
				ChooseSaveSlot_Widget->SetGameInstanceRef(GameInstanceRef);
				ChooseSaveSlot_Widget->SetPlayerControllerRef(PlayerControllerRef);
				ChooseSaveSlot_Widget->ShouldOpenMainMenuDelegate.
				                       BindUObject(this, &ACPP_HUD::InitializeMainMenuWidget);
				Container_Widget->MainMenuAndPauseWidgetSizeBox->AddChild(ChooseSaveSlot_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyChooseSaveSlotWidget()
{
	if (IsValid(ChooseSaveSlot_Widget))
	{
		ChooseSaveSlot_Widget->ShouldOpenMainMenuDelegate.Unbind();
		ChooseSaveSlot_Widget->RemoveFromParent();
		ChooseSaveSlot_Widget = nullptr;
	}
}

void ACPP_HUD::InitializePauseWidget()
{
	if (IsValid(MainMenu_Widget))
		return;

	if (IsValid(Container_Widget))
	{
		Container_Widget->BackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(Level_Widget))
	{
		Level_Widget->bIsWidgetActive = false;
	}

	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		Pause_Widget == nullptr)
	{
		const FName Row = FName(TEXT("Pause"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			Pause_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(Pause_Widget) && IsValid(Container_Widget))
			{
				Pause_Widget->SetFlags(RF_StrongRefOnFrame);
				Pause_Widget->SetGameInstanceRef(GameInstanceRef);
				Pause_Widget->SetPlayerControllerRef(PlayerControllerRef);
				if (GameInstanceRef->GetPlayingModeAsInt() == 1)
				{
					Pause_Widget->SetOwningPlayer(PlayerControllerRef);
					Pause_Widget->AddToViewport(0);
				}
				else
				{
					Container_Widget->MainMenuAndPauseWidgetSizeBox->AddChild(Pause_Widget);
				}
			}
		}
	}
}

void ACPP_HUD::DestroyPauseWidget()
{
	if (IsValid(Level_Widget))
	{
		Level_Widget->bIsWidgetActive = true;
	}

	if (IsValid(Pause_Widget))
	{
		Pause_Widget->RemoveFromParent();
		Pause_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeLoadingScreenWidget()
{
	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		LoadingScreen_Widget == nullptr)
	{
		const FName Row = FName(TEXT("Loading"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			LoadingScreen_Widget = CreateWidget<UWCPP_WidgetParent>(PlayerControllerRef, Class);
			if (IsValid(LoadingScreen_Widget))
			{
				LoadingScreen_Widget->SetFlags(RF_StrongRefOnFrame);
				if (GameInstanceRef->GetPlayingModeAsInt() == 1)
				{
					LoadingScreen_Widget->AddToViewport(10);
				}
				else if (IsValid(Container_Widget))
				{
					Container_Widget->LoadingWidgetSizeBox->AddChild(LoadingScreen_Widget);
				}
			}
		}
	}
}

void ACPP_HUD::DestroyLoadingScreenWidget(const bool bShouldHideBackgroundImage)
{
	if (IsValid(GetWorld()) &&
		GetWorld()->GetTimerManager().TimerExists(TH_DestroyLoadingScreen))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_DestroyLoadingScreen);
	}
	if (bShouldHideBackgroundImage && IsValid(Container_Widget))
	{
		Container_Widget->BackgroundImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(PlayerControllerRef))
	{
		PlayerControllerRef->bCanBePaused = true;
	}
	if (IsValid(LoadingScreen_Widget))
	{
		LoadingScreen_Widget->RemoveFromParent();
		LoadingScreen_Widget = nullptr;
	}
}

void ACPP_HUD::InitializeLevelWidget()
{
	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		Level_Widget == nullptr)
	{
		const FName Row = FName(TEXT("Level"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			Level_Widget = CreateWidget<UWCPP_Level>(PlayerControllerRef, Class);
			if (IsValid(Level_Widget) && IsValid(Container_Widget))
			{
				Level_Widget->SetFlags(RF_StrongRefOnFrame);
				if (GameInstanceRef->GetPlayingModeAsInt() == 3 ||
					GameInstanceRef->GetPlayingModeAsInt() == 4)
				{
					DestroyLoadingScreenWidget(true);
				}
				Level_Widget->SetGameInstanceRef(GameInstanceRef);
				Level_Widget->SetPlayerControllerRef(PlayerControllerRef);
				if (PlayerControllerRef->GetSoundManagerRef() == nullptr)
				{
					if (UClass* SoundManagerClass = GameInstanceRef->GetActorClassBySoftReference(
						UCPP_StaticWidgetLibrary::GetSoftReferenceToSoundManagerByRowName(
							SoundManagersDataTable,
							FName(TEXT("LevelSoundManager")))))
					{
						ACPP_SoundManager* TmpSoundManager = GetWorld()->SpawnActorDeferred<ACPP_SoundManager>(
							SoundManagerClass,
							FTransform::Identity,
							nullptr,
							nullptr,
							ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

						if (IsValid(TmpSoundManager))
						{
							TmpSoundManager->SetPlayerControllerRef(PlayerControllerRef);
							bool bSetDefaultValues = true;
							if (IsValid(PlayerControllerRef->GetCharacter()) &&
								IsValid(PlayerControllerRef->GetCharacter()->GetPlayerState<ACPP_PlayerState>()))
							{
								bSetDefaultValues = !PlayerControllerRef->GetCharacter()->
								                                          GetPlayerState<ACPP_PlayerState>()->
								                                          GetSaveFileWasCreated();
							}
							TmpSoundManager->ApplySettingsFromTheSaveFile(bSetDefaultValues, false);
							PlayerControllerRef->SetSoundManagerRef(TmpSoundManager);
							UGameplayStatics::FinishSpawningActor(TmpSoundManager, FTransform());
						}
					}
				}
				Container_Widget->LevelWidgetSizeBox->AddChild(Level_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyLevelWidget()
{
	if (IsValid(Level_Widget))
	{
		Level_Widget->RemoveFromParent();
		Level_Widget = nullptr;
	}
}

void ACPP_HUD::LevelWasEnded(const bool bIsWinner)
{
	DestroyLevelWidget();
	InitializeEndLevelWidget(bIsWinner);
}

void ACPP_HUD::InitializeEndLevelWidget(const bool bIsWinner)
{
	if (IsValid(GameInstanceRef) &&
		IsValid(PlayerControllerRef) &&
		EndLevel_Widget == nullptr)
	{
		const FName Row = FName(TEXT("EndLevel"));
		if (UClass* Class = GameInstanceRef->GetWidgetClassBySoftReference(
			UCPP_StaticWidgetLibrary::GetSoftReferenceToWidgetBlueprintByRowName(
				WidgetBlueprintsDataTable,
				Row)))
		{
			EndLevel_Widget = CreateWidget<UWCPP_EndLevel>(PlayerControllerRef, Class);
			if (IsValid(EndLevel_Widget) && IsValid(Container_Widget))
			{
				EndLevel_Widget->SetFlags(RF_StrongRefOnFrame);
				PlayerControllerRef->ChangeInputEnabling(false);
				PlayerControllerRef->ChangeCursorVisibility(true);
				EndLevel_Widget->SetPlayerControllerRef(PlayerControllerRef);
				EndLevel_Widget->SetGameInstanceRef(GameInstanceRef);
				EndLevel_Widget->SetIsWinner(bIsWinner);
				Container_Widget->LevelWidgetSizeBox->AddChild(EndLevel_Widget);
			}
		}
	}
}

void ACPP_HUD::DestroyEndLevelWidget()
{
	if (IsValid(EndLevel_Widget))
	{
		EndLevel_Widget->RemoveFromParent();
		EndLevel_Widget = nullptr;
	}
}

void ACPP_HUD::StartPause()
{
	if (IsValid(MainMenu_Widget) || IsValid(EndLevel_Widget))
		return;

	InitializePauseWidget();
	PlayerControllerRef->StartPause();
}

void ACPP_HUD::EndPause()
{
	DestroyPauseWidget();
	PlayerControllerRef->EndPause();
}
