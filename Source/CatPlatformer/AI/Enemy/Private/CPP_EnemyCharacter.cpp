// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ACPP_EnemyCharacter::ACPP_EnemyCharacter(): GameStateRef(nullptr), EnemyState(EEnemyState::Walking),
                                            BasicFlyingSpeed(0), ChasingFlyingSpeed(0),
                                            BasicWalkingSpeed(0), ChasingWalkingSpeed(0),
                                            PointsToAddToCharacter(20), bIsDead(false),
                                            bIsAttacking(false), AnimInstance(nullptr),
                                            FlyingAttackMontage(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	if (!AttackMontageEndedDelegate.IsBound())
	{
		AttackMontageEndedDelegate.BindUObject(this, &ACPP_EnemyCharacter::OnAttackMontageEnded);
	}
	GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void ACPP_EnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AttackMontageEndedDelegate.IsBound())
	{
		AttackMontageEndedDelegate.Unbind();
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_EnemyCharacter, bIsAttacking);
	DOREPLIFETIME(ACPP_EnemyCharacter, EnemyState);
}

void ACPP_EnemyCharacter::ChangeFlyingSpeed(const bool bApplyBasicSpeed)
{
	GetCharacterMovement()->MaxFlySpeed = bApplyBasicSpeed ? BasicFlyingSpeed : ChasingFlyingSpeed;
}

void ACPP_EnemyCharacter::ChangeWalkingSpeed(const bool bApplyBasicSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = bApplyBasicSpeed ? BasicWalkingSpeed : ChasingWalkingSpeed;
}

void ACPP_EnemyCharacter::OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsAttacking = false;
}

float ACPP_EnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                      AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsDead)
	{
		if (AnimInstance->Montage_IsActive(FlyingAttackMontage))
		{
			AnimInstance->Montage_Stop(0, FlyingAttackMontage);
		}
		GetWorld()->GetTimerManager().SetTimer(
			TH_CallSelfDestroying,
			this,
			&ACPP_EnemyCharacter::Server_CallSelfDestroying,
			2.75f,
			false);
		SetEnemyState(EEnemyState::Dying);

		if (const ACPP_Character* Player = Cast<ACPP_Character>(DamageCauser))
		{
			if (!IsValid(GameStateRef))
			{
				GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
			}
			if (IsValid(GameStateRef))
			{
				GameStateRef->AddUserScore(
					IsValid(Player->GetPlayerStateRef())
						? Player->GetPlayerStateRef()
						: Player->GetPlayerState<ACPP_PlayerState>(),
					PointsToAddToCharacter);
				// Is needed for the player's statistics.
				Player->NPC_WasKilled();
			}
		}
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACPP_EnemyCharacter::Server_Attack_Implementation(ACPP_Character* CharacterToAttack)
{
	Multicast_Attack(CharacterToAttack);
}

void ACPP_EnemyCharacter::Multicast_Attack_Implementation(ACPP_Character* CharacterToAttack)
{
	if (AnimInstance && FlyingAttackMontage &&
		!AnimInstance->Montage_IsActive(FlyingAttackMontage))
	{
		bIsAttacking = true;
		AnimInstance->Montage_Play(FlyingAttackMontage);
		AnimInstance->Montage_SetEndDelegate(AttackMontageEndedDelegate, FlyingAttackMontage);
		FVector Origin;
		if (GetMesh()->DoesSocketExist(FName(TEXT("NeckSocket"))))
		{
			Origin = GetMesh()->GetSocketLocation(FName(TEXT("NeckSocket")));
		}
		else
		{
			Origin = GetActorLocation();
		}
		const TArray<AActor*> IgnoreActors{};
		if (IsValid(CharacterToAttack))
		{
			const FVector HitFromDirection = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(), CharacterToAttack->GetActorLocation()).Vector();
			const FHitResult HitResult;
			UGameplayStatics::ApplyPointDamage(CharacterToAttack, 100.0f, HitFromDirection,
			                                   HitResult, GetController(), this,
			                                   UDamageType::StaticClass()
			);
		}
		else
		{
			UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, Origin, 30.0f,
			                                    UDamageType::StaticClass(), IgnoreActors, this,
			                                    GetController());
		}
	}
}

void ACPP_EnemyCharacter::Server_CallSelfDestroying_Implementation()
{
	Multicast_CallSelfDestroying();
}

void ACPP_EnemyCharacter::Multicast_CallSelfDestroying_Implementation()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_CallSelfDestroying))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_CallSelfDestroying);
	}
	if (IsValid(GetController()))
	{
		GetController()->Destroy();
	}
	if (IsValid(this))
	{
		Destroy();
	}
}

void ACPP_EnemyCharacter::SetIsDead_Implementation(const bool bNewValue)
{
	Multicast_SetIsDead(bNewValue);
}

void ACPP_EnemyCharacter::Multicast_SetIsDead_Implementation(const bool bNewValue)
{
	bIsDead = bNewValue;
}

bool ACPP_EnemyCharacter::GetIsDead() const
{
	return bIsDead;
}

void ACPP_EnemyCharacter::SetEnemyState_Implementation(const EEnemyState NewState)
{
	Multicast_SetEnemyState(NewState);
}

void ACPP_EnemyCharacter::Multicast_SetEnemyState_Implementation(const EEnemyState NewState)
{
	if (EnemyState == EEnemyState::Dying)
	{
		return;
	}

	switch (NewState)
	{
	case EEnemyState::Walking:
		{
			if (EnemyState == EEnemyState::Flying)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			}
			else
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
			break;
		}
	case EEnemyState::Flying:
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			break;
		}
	case EEnemyState::ChasingCharacter:
		break;
	case EEnemyState::Attacking:
		break;
	case EEnemyState::Dying:
		{
			if (EnemyState == EEnemyState::Flying)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			}
			else
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
			SetIsDead(true);
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::);
			if (EnemyIsDeadDelegate.IsBound())
			{
				EnemyIsDeadDelegate.Execute();
			}
			if (!GetWorld()->GetTimerManager().TimerExists(TH_CallSelfDestroying))
			{
				Server_CallSelfDestroying();
			}
			break;
		}
	}
	EnemyState = NewState;
}
