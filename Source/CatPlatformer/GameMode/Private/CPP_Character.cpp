// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_Character.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"

ACPP_Character::ACPP_Character() : bSprintNow(false), BaseSpeed(165.0f), SprintSpeed(300.0f),
                                   BaseTurnRate(45.f), BaseLookUpRate(45.f), // Set turn rates for input.
                                   BaseJumpZVelocity(400.0f), HighJumpZVelocity(900.0f),
                                   AnimInstance(nullptr),
                                   AttackMontage(nullptr),
                                   ReceiveDamageMontage(nullptr),
                                   bIsOnGrass(false),
                                   bIsJumping(false),
                                   bIsAttacking(false),
                                   bIsReceivingDamage(false),
                                   bIsWaiting(false),
                                   CurrentColor(nullptr),
                                   bDoubleJumpBuffIsActive(false), bHighJumpBuffIsActive(false),
                                   bSlowBuffIsActive(false), bFastBuffIsActive(false),
                                   bShieldBuffIsActive(false), ShieldNiagara(nullptr),
                                   PlayerStateRef(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement.
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 165.0f;

	// Create a camera boom (pulls in towards the player if there is a collision).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("CameraBoom")));
	CameraBoom->SetupAttachment(RootComponent);
	// The camera follows at this distance behind the character.
	CameraBoom->TargetArmLength = 300.0f;
	// Rotate the arm based on the controller.
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("FollowCamera")));
	// Attach the camera to the end of the boom and let
	// the boom adjust to match the controller orientation.
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Camera does not rotate relative to arm.
	FollowCamera->bUsePawnControlRotation = false;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName(TEXT("Niagara Component")));
	NiagaraComponent->SetupAttachment(GetMesh());

	LeftPawCollision = CreateDefaultSubobject<USphereComponent>(FName(TEXT("Paw Sphere")));
	LeftPawCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                    FName(TEXT("left_foot_socket")));
	LeftPawCollision->InitSphereRadius(0.05f);

	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(NiagaraComponent) && !NiagaraComponent->IsActive())
	{
		NiagaraComponent->ActivateSystem();
		NiagaraComponent->DeactivateImmediate();
	}

	AnimInstance = GetMesh()->GetAnimInstance();
	if (!AttackMontageEndedDelegate.IsBound())
	{
		AttackMontageEndedDelegate.BindUObject(this, &ACPP_Character::OnAttackMontageEnded);
	}
	if (!ReceiveDamageMontageEndedDelegate.IsBound())
	{
		ReceiveDamageMontageEndedDelegate.BindUObject(this, &ACPP_Character::OnReceiveDamageMontageEnded);
	}

	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->UpdateCatsColorUpToSavedIndex();
	}
}

void ACPP_Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_TurnOff_PP_DamageMaterial))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_TurnOff_PP_DamageMaterial);
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallApplyingDamage))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallApplyingDamage);
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_CheckForHittingTheGround))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CheckForHittingTheGround);
	}

	ResetAllActiveBuffs();
	if (PlayerStateChangedDelegate.IsBound())
	{
		PlayerStateChangedDelegate.Clear();
	}
	if (CollectBuffDelegate.IsBound())
	{
		CollectBuffDelegate.Unbind();
	}
	if (ResetBuffEffectDelegate.IsBound())
	{
		ResetBuffEffectDelegate.Unbind();
	}
	if (AttackMontageEndedDelegate.IsBound())
	{
		AttackMontageEndedDelegate.Unbind();
	}
	if (ReceiveDamageMontageEndedDelegate.IsBound())
	{
		ReceiveDamageMontageEndedDelegate.Unbind();
	}

	PlayerStateRef = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ACPP_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_Character, PlayerStateRef);
	DOREPLIFETIME(ACPP_Character, bIsWaiting);
	DOREPLIFETIME(ACPP_Character, CurrentColor);
	DOREPLIFETIME(ACPP_Character, bSprintNow);
}

void ACPP_Character::OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsAttacking = false;
}

void ACPP_Character::OnReceiveDamageMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsReceivingDamage = false;
}

void ACPP_Character::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
	Super::OnPlayerStateChanged(NewPlayerState, OldPlayerState);

	if (IsValid(NewPlayerState))
	{
		PlayerStateRef = Cast<ACPP_PlayerState>(NewPlayerState);
		if (IsValid(PlayerStateRef))
		{
			PlayerStateChangedDelegate.Broadcast(PlayerStateRef);
			PlayerStateRef->UpdateCatsColorUpToSavedIndex();
		}
	}
}

void ACPP_Character::ResetAllActiveBuffs()
{
	if (bDoubleJumpBuffIsActive && GetWorld()->GetTimerManager().TimerExists(TH_DoubleJump))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_DoubleJump);
	}
	if (bHighJumpBuffIsActive && GetWorld()->GetTimerManager().TimerExists(TH_HighJump))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_HighJump);
	}
	if (bSlowBuffIsActive && GetWorld()->GetTimerManager().TimerExists(TH_SlowBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_SlowBuff);
	}
	if (bFastBuffIsActive && GetWorld()->GetTimerManager().TimerExists(TH_FastBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FastBuff);
	}
	if (bShieldBuffIsActive && GetWorld()->GetTimerManager().TimerExists(TH_ShieldBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ShieldBuff);
	}
}

void ACPP_Character::LaunchDoubleJumpBuff(const float Duration, USlateBrushAsset* Image)
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementCollectedBuffsNumber();
	}

	CollectBuffDelegate.ExecuteIfBound(1, Image, Duration);

	bDoubleJumpBuffIsActive = true;

	JumpMaxCount = 2;

	if (GetWorld()->GetTimerManager().TimerExists(TH_DoubleJump))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_DoubleJump);
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_DoubleJump,
		this,
		&ACPP_Character::ResetDoubleJumpBuff,
		Duration,
		false);
}

void ACPP_Character::ResetDoubleJumpBuff()
{
	if (ResetBuffEffectDelegate.IsBound())
	{
		ResetBuffEffectDelegate.Execute(1);
	}

	bDoubleJumpBuffIsActive = false;

	JumpMaxCount = 1;

	if (GetWorld()->GetTimerManager().TimerExists(TH_DoubleJump))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_DoubleJump);
	}
}

void ACPP_Character::LaunchHighJumpBuff(const float Duration, USlateBrushAsset* Image)
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementCollectedBuffsNumber();
	}

	CollectBuffDelegate.ExecuteIfBound(2, Image, Duration);

	bHighJumpBuffIsActive = true;

	GetCharacterMovement()->JumpZVelocity = HighJumpZVelocity;

	if (GetWorld()->GetTimerManager().TimerExists(TH_HighJump))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_HighJump);
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_HighJump,
		this,
		&ACPP_Character::ResetHighJumpBuff,
		Duration,
		false);
}

void ACPP_Character::ResetHighJumpBuff()
{
	if (ResetBuffEffectDelegate.IsBound())
	{
		ResetBuffEffectDelegate.Execute(2);
	}

	bHighJumpBuffIsActive = false;

	GetCharacterMovement()->JumpZVelocity = BaseJumpZVelocity;

	if (GetWorld()->GetTimerManager().TimerExists(TH_HighJump))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_HighJump);
	}
}

void ACPP_Character::LaunchSlowBuff(const float Duration, USlateBrushAsset* Image)
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementCollectedBuffsNumber();
	}

	if (bFastBuffIsActive)
	{
		ResetFastBuff();
	}

	CollectBuffDelegate.ExecuteIfBound(3, Image, Duration);

	bSlowBuffIsActive = true;

	BaseSpeed = 65.0f;
	SprintSpeed = 200.0f;
	ChangeCurrentSpeed(bSprintNow);

	if (GetWorld()->GetTimerManager().TimerExists(TH_SlowBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_SlowBuff);
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_SlowBuff,
		this,
		&ACPP_Character::ResetSlowBuff,
		Duration,
		false);
}

void ACPP_Character::ResetSlowBuff()
{
	if (ResetBuffEffectDelegate.IsBound())
	{
		ResetBuffEffectDelegate.Execute(3);
	}

	bSlowBuffIsActive = false;

	BaseSpeed = 165.0f;
	SprintSpeed = 300.0f;
	ChangeCurrentSpeed(bSprintNow);

	if (GetWorld()->GetTimerManager().TimerExists(TH_SlowBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_SlowBuff);
	}
}

void ACPP_Character::LaunchFastBuff(const float Duration, USlateBrushAsset* Image)
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementCollectedBuffsNumber();
	}

	if (bSlowBuffIsActive)
	{
		ResetSlowBuff();
	}

	CollectBuffDelegate.ExecuteIfBound(4, Image, Duration);

	bFastBuffIsActive = true;

	BaseSpeed = 300.0f;
	SprintSpeed = 450.0f;
	ChangeCurrentSpeed(bSprintNow);

	if (GetWorld()->GetTimerManager().TimerExists(TH_FastBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FastBuff);
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_FastBuff,
		this,
		&ACPP_Character::ResetFastBuff,
		Duration,
		false);
}

void ACPP_Character::ResetFastBuff()
{
	if (ResetBuffEffectDelegate.IsBound())
	{
		ResetBuffEffectDelegate.Execute(4);
	}

	bFastBuffIsActive = false;

	BaseSpeed = 165.0f;
	SprintSpeed = 300.0f;
	ChangeCurrentSpeed(bSprintNow);

	if (GetWorld()->GetTimerManager().TimerExists(TH_FastBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_FastBuff);
	}
}

void ACPP_Character::LaunchShieldBuff(const float Duration, USlateBrushAsset* Image)
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementCollectedBuffsNumber();
	}

	CollectBuffDelegate.ExecuteIfBound(5, Image, Duration);

	bShieldBuffIsActive = true;
	SpawnShield();

	if (GetWorld()->GetTimerManager().TimerExists(TH_ShieldBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ShieldBuff);
	}

	GetWorld()->GetTimerManager().SetTimer(
		TH_ShieldBuff,
		this,
		&ACPP_Character::ResetShieldBuff,
		Duration,
		false);
}

void ACPP_Character::ResetShieldBuff()
{
	if (ResetBuffEffectDelegate.IsBound())
	{
		ResetBuffEffectDelegate.Execute(5);
	}

	bShieldBuffIsActive = false;
	DestroyShield();

	if (GetWorld()->GetTimerManager().TimerExists(TH_ShieldBuff))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_ShieldBuff);
	}
}

void ACPP_Character::SpawnShield() const
{
	if (IsValid(NiagaraComponent) && !NiagaraComponent->IsActive())
	{
		NiagaraComponent->ActivateSystem();
	}
}

void ACPP_Character::DestroyShield() const
{
	if (IsValid(NiagaraComponent) && NiagaraComponent->IsActive())
	{
		NiagaraComponent->DeactivateImmediate();
	}
}

void ACPP_Character::ChangeCharactersSliding(const bool bShouldSlide) const
{
	if (bShouldSlide)
	{
		GetCharacterMovement()->GroundFriction = 0.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 150.0f;
	}
	else
	{
		GetCharacterMovement()->GroundFriction = 8.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}
}

void ACPP_Character::SetCurrentColorVariableByIndex_Implementation(const int32 ColorIndex)
{
	Multicast_SetCurrentColorVariableByIndex(ColorIndex);
}

void ACPP_Character::Multicast_SetCurrentColorVariableByIndex_Implementation(const int32 ColorIndex)
{
	if (CatColors.Num() == 3)
	{
		CurrentColor = CatColors[ColorIndex % 3];
	}
}

void ACPP_Character::SetCurrentColorMaterialByIndex(const int32 ColorIndex)
{
	if (CatColors.Num() == 3)
	{
		ChangeCatsColor(CatColors[ColorIndex % 3]);
	}
}

void ACPP_Character::ChangeCatsColor_Implementation(UMaterialInstance* Material)
{
	Multicast_ChangeCatsColor(Material);
}

void ACPP_Character::Multicast_ChangeCatsColor_Implementation(UMaterialInstance* Material)
{
	if (IsValid(Material) && IsValid(GetMesh()))
	{
		GetMesh()->SetMaterial(0, Material);
	}
}

void ACPP_Character::TurnAtRate(const float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACPP_Character::LookUpAtRate(const float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACPP_Character::CustomStartJumping_Implementation()
{
	Multicast_CustomStartJumping();
}

void ACPP_Character::Multicast_CustomStartJumping_Implementation()
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementJumpsNumber();
	}
	bIsJumping = true;
	Jump();
	if (!GetWorld()->GetTimerManager().TimerExists(TH_CheckForHittingTheGround))
	{
		GetWorld()->GetTimerManager().SetTimer(
			TH_CheckForHittingTheGround,
			this,
			&ACPP_Character::CheckForHittingTheGround,
			0.05,
			true);
	}
}

void ACPP_Character::CustomStopJumping_Implementation()
{
	Multicast_CustomStopJumping();
}

void ACPP_Character::Multicast_CustomStopJumping_Implementation()
{
	StopJumping();
}

void ACPP_Character::CheckForHittingTheGround()
{
	if (!bPressedJump && !GetMovementComponent()->IsFalling())
	{
		if (GetWorld()->GetTimerManager().TimerExists(TH_CheckForHittingTheGround))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_CheckForHittingTheGround);
		}
		bIsJumping = false;
		PlaySound(ECatSoundState::HitGround);
	}
}

void ACPP_Character::ChangeCurrentSpeed_Implementation(const bool bIsSprint)
{
	Multicast_ChangeCurrentSpeed(bIsSprint);
}

void ACPP_Character::Multicast_ChangeCurrentSpeed_Implementation(const bool bIsSprint)
{
	GetCharacterMovement()->MaxWalkSpeed = bIsSprint ? SprintSpeed : BaseSpeed;
	bSprintNow = bIsSprint;
}

void ACPP_Character::SetIsWaiting_Implementation(const bool bNewValue)
{
	bIsWaiting = bNewValue;
}

void ACPP_Character::Server_Attack_Implementation()
{
	Multicast_Attack();
}

void ACPP_Character::Multicast_Attack_Implementation()
{
	if (AnimInstance.IsValid() && IsValid(AttackMontage.LoadSynchronous()))
	{
		bIsAttacking = true;
		if (UAnimMontage* MontagePointer = AttackMontage.Get();
			!AnimInstance->Montage_IsActive(MontagePointer))
		{
			AnimInstance->Montage_Play(MontagePointer);
			AnimInstance->Montage_SetEndDelegate(AttackMontageEndedDelegate, MontagePointer);
			GetWorld()->GetTimerManager().SetTimer(
				TH_CallApplyingDamage,
				this,
				&ACPP_Character::Multicast_CallApplyingDamage,
				0.2f,
				false);
		}
	}
}

float ACPP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	if (bShieldBuffIsActive || !Cast<ACharacter>(DamageCauser))
		return 0.0f;

	const float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AnimInstance.IsValid() && ReceiveDamageMontage)
	{
		if (!AnimInstance->Montage_IsActive(ReceiveDamageMontage))
		{
			bIsReceivingDamage = true;
			PlaySound(ECatSoundState::Hiss);
			AnimInstance->Montage_Play(ReceiveDamageMontage);
			AnimInstance->Montage_SetEndDelegate(ReceiveDamageMontageEndedDelegate, ReceiveDamageMontage);
		}
	}

	ChangeEnablingOfPostProcessDamageMaterial(true);
	if (HasAuthority())
	{
		if (ACPP_GameState* GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			GameStateRef->AddUserScore(PlayerStateRef, -5);
		}
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_TurnOff_PP_DamageMaterial))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_TurnOff_PP_DamageMaterial);
	}
	GetWorld()->GetTimerManager().SetTimer(
		TH_TurnOff_PP_DamageMaterial,
		this,
		&ACPP_Character::TurnOffPostProcessDamageMaterial,
		1.0f,
		false);
	return Result;
}

void ACPP_Character::TurnOffPostProcessDamageMaterial()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_TurnOff_PP_DamageMaterial))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_TurnOff_PP_DamageMaterial);
	}
	ChangeEnablingOfPostProcessDamageMaterial(false);
}

void ACPP_Character::Multicast_CallApplyingDamage_Implementation()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallApplyingDamage))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallApplyingDamage);
	}
	FVector Origin;
	if (GetMesh()->DoesSocketExist(FName(TEXT("left_foot_socket"))))
	{
		Origin = GetMesh()->GetSocketLocation(FName(TEXT("left_foot_socket")));
	}
	else
	{
		Origin = GetActorLocation();
	}
	TArray<UPrimitiveComponent*> OutOverlappingComponents{};
	LeftPawCollision->GetOverlappingComponents(OutOverlappingComponents);

	for (int32 i = 0; i < OutOverlappingComponents.Num(); i++)
	{
		if (OutOverlappingComponents[i]->GetClass()->IsChildOf<UBoxComponent>())
		{
			OutOverlappingComponents.RemoveAt(i);
			i--;
		}
	}
	if (OutOverlappingComponents.Num() > 0)
	{
		PlaySound(ECatSoundState::Attack);
	}
	const TArray<AActor*> IgnoreActors{};
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, Origin, 20.0f,
	                                    UDamageType::StaticClass(), IgnoreActors, this,
	                                    GetController());
}

void ACPP_Character::NPC_WasKilled() const
{
	if (IsValid(PlayerStateRef))
	{
		PlayerStateRef->IncrementNPCsKilledNumber();
	}
}
