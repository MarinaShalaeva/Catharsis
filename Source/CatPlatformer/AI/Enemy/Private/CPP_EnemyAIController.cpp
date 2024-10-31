// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_EnemyAIController.h"

#include "VectorTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
class UCapsuleComponent;

ACPP_EnemyAIController::ACPP_EnemyAIController(): EnemyCharacter(nullptr),
                                                  bReachTheTarget(false),
                                                  bIsFlying(false),
                                                  bFlyingTargetLocationIsSet(false),
                                                  MaxFlyingTargets(0), FlyingTargetsCounter(0),
                                                  WalkingSearchRadius(140.0f),
                                                  bIsWalkingToLocation(false),
                                                  bWalkingTargetLocationIsSet(false),
                                                  MaxWalkingTargets(0), WalkingTargetsCounter(0),
                                                  CharacterToChase(nullptr), bIsFlyingChasing(false),
                                                  bChasingTargetReached(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_EnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_EnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_WalkingAndFlyingSwitcher))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_WalkingAndFlyingSwitcher);
	}
	if (GetWorld()->GetTimerManager().TimerExists(TH_UpdateCharacterBehavior))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_UpdateCharacterBehavior);
	}

	Super::EndPlay(EndPlayReason);
}

void ACPP_EnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacterBehavior(DeltaSeconds);
}

void ACPP_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyCharacter = Cast<ACPP_EnemyCharacter>(InPawn);
}

void ACPP_EnemyAIController::OnUnPossess()
{
	if (IsValid(EnemyCharacter) && EnemyCharacter->EnemyIsDeadDelegate.IsBound())
	{
		EnemyCharacter->EnemyIsDeadDelegate.Unbind();
		EnemyCharacter = nullptr;
	}

	Super::OnUnPossess();
}

void ACPP_EnemyAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_EnemyAIController, bIsFlying);
	DOREPLIFETIME(ACPP_EnemyAIController, bFlyingTargetLocationIsSet);
	DOREPLIFETIME(ACPP_EnemyAIController, FlyingTargetLocation);
	DOREPLIFETIME(ACPP_EnemyAIController, MaxFlyingTargets);
	DOREPLIFETIME(ACPP_EnemyAIController, FlyingTargetsCounter);

	DOREPLIFETIME(ACPP_EnemyAIController, bIsWalkingToLocation);
	DOREPLIFETIME(ACPP_EnemyAIController, bWalkingTargetLocationIsSet);
	DOREPLIFETIME(ACPP_EnemyAIController, WalkingTargetLocation);
	DOREPLIFETIME(ACPP_EnemyAIController, MaxWalkingTargets);
	DOREPLIFETIME(ACPP_EnemyAIController, WalkingTargetsCounter);

	DOREPLIFETIME(ACPP_EnemyAIController, CharacterToChase);
	DOREPLIFETIME(ACPP_EnemyAIController, ChasingTargetLocation);
	DOREPLIFETIME(ACPP_EnemyAIController, bIsFlyingChasing);
}

void ACPP_EnemyAIController::UpdateCharacterBehavior_Implementation(const float DeltaSeconds)
{
	Multicast_UpdateCharacterBehavior(DeltaSeconds);
}

void ACPP_EnemyAIController::Multicast_UpdateCharacterBehavior_Implementation(const float DeltaSeconds)
{
	if (!IsValid(EnemyCharacter))
		return;

	switch (EnemyCharacter->GetEnemyState())
	{
	case EEnemyState::Walking:
		{
			bIsFlying = false;
			bFlyingTargetLocationIsSet = false;
			if (MaxWalkingTargets == 0)
			{
				EnemyCharacter->ChangeWalkingSpeed(true);
				bWalkingTargetLocationIsSet = false;
				WalkingTargetsCounter = 0;
				MaxWalkingTargets = UKismetMathLibrary::RandomIntegerInRange(1, 5);
			}
			if (bReachTheTarget || !bWalkingTargetLocationIsSet)
			{
				bIsWalkingToLocation = false;
				FindWalkingLocation();
			}
			if (WalkingTargetsCounter > MaxWalkingTargets)
			{
				MaxWalkingTargets = 0;
				WalkingTargetsCounter = 0;
				bIsWalkingToLocation = false;
				bWalkingTargetLocationIsSet = false;
				bReachTheTarget = false;
				EnemyCharacter->SetEnemyState(EEnemyState::Flying);
			}
			else
			{
				if (EnemyCharacter->GetCharacterMovement()->MovementMode != MOVE_Falling)
				{
					WalkToLocation(DeltaSeconds);
				}
			}
			break;
		}
	case EEnemyState::Flying:
		{
			bIsWalkingToLocation = false;
			bWalkingTargetLocationIsSet = false;
			if (MaxFlyingTargets == 0)
			{
				EnemyCharacter->ChangeFlyingSpeed(true);
				bFlyingTargetLocationIsSet = false;
				FlyingTargetsCounter = 0;
				MaxFlyingTargets = UKismetMathLibrary::RandomIntegerInRange(1, 5);
			}

			if (bReachTheTarget || !bFlyingTargetLocationIsSet)
			{
				bIsFlying = false;
				FindFlyingLocation();
			}
			if (FlyingTargetsCounter > MaxFlyingTargets)
			{
				FlyingTargetsCounter = 0;
				bIsFlying = false;
				bFlyingTargetLocationIsSet = false;
				MaxFlyingTargets = 0;
				bReachTheTarget = false;
				EnemyCharacter->SetEnemyState(EEnemyState::Walking);
			}
			else
			{
				FlyToLocation(DeltaSeconds);
			}
			break;
		}
	case EEnemyState::ChasingCharacter:
		{
			if (bIsFlying)
			{
				EnemyCharacter->ChangeFlyingSpeed(false);
				bIsFlyingChasing = true;
				bIsFlying = false;
				bFlyingTargetLocationIsSet = false;
				FlyingTargetsCounter = 0;
				MaxFlyingTargets = 0;

				bReachTheTarget = false;
			}
			else if (bIsWalkingToLocation)
			{
				EnemyCharacter->ChangeWalkingSpeed(false);
				bIsFlyingChasing = false;
				bIsWalkingToLocation = false;
				bWalkingTargetLocationIsSet = false;
				WalkingTargetsCounter = 0;
				MaxWalkingTargets = 0;

				bReachTheTarget = false;
			}

			if (!FindChasingTargetLocation(DeltaSeconds))
			{
				CharacterToChase = nullptr;

				if (bIsFlyingChasing)
				{
					EnemyCharacter->SetEnemyState(EEnemyState::Flying);
				}
				else
				{
					EnemyCharacter->SetEnemyState(EEnemyState::Walking);
				}
			}
			else
			{
				if (bChasingTargetReached)
				{
					EnemyCharacter->SetEnemyState(EEnemyState::Attacking);
				}
			}

			break;
		}
	case EEnemyState::Attacking:
		{
			if (!EnemyCharacter->bIsAttacking)
			{
				EnemyCharacter->Server_Attack(CharacterToChase);
			}
			else
			{
				EnemyCharacter->SetEnemyState(EEnemyState::ChasingCharacter);
			}
			break;
		}
	case EEnemyState::Dying:
		{
			if (GetWorld()->GetTimerManager().TimerExists(TH_UpdateCharacterBehavior))
			{
				GetWorld()->GetTimerManager().ClearTimer(TH_UpdateCharacterBehavior);
			}
			break;
		}
	}
}

void ACPP_EnemyAIController::Server_SwitchBetweenWalkingAndFlyingModes_Implementation()
{
	Multicast_SwitchBetweenWalkingAndFlyingModes();
}

void ACPP_EnemyAIController::Multicast_SwitchBetweenWalkingAndFlyingModes_Implementation()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_WalkingAndFlyingSwitcher))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_WalkingAndFlyingSwitcher);
	}
	bIsFlying = !bIsFlying;
	if (!IsValid(EnemyCharacter))
	{
		EnemyCharacter = Cast<ACPP_EnemyCharacter>(GetPawn());
	}
	if (IsValid(EnemyCharacter))
	{
		if (bIsFlying)
		{
			EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		else
		{
			EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(
		TH_WalkingAndFlyingSwitcher,
		this,
		&ACPP_EnemyAIController::Server_SwitchBetweenWalkingAndFlyingModes,
		UKismetMathLibrary::RandomFloatInRange(3.0f, 8.0f),
		false);
}

bool ACPP_EnemyAIController::FindFlyingLocation()
{
	if (!IsValid(EnemyCharacter))
		return false;

	constexpr float SearchRadius = 125.0f;
	constexpr float MaxHeight = 270.0f;
	const FVector OriginXY = EnemyCharacter->StartTransform.GetLocation();
	const FVector Origin = FVector(OriginXY.X, OriginXY.Y, (MaxHeight - OriginXY.Z) / 2 + 20.0f);

	FlyingTargetLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, FVector(SearchRadius,
		                                                                    SearchRadius,
		                                                                    (MaxHeight - OriginXY.Z) / 2));

	FlyingTargetsCounter++;
	bFlyingTargetLocationIsSet = true;

	return true;
}

void ACPP_EnemyAIController::FlyToLocation(const float DeltaSeconds)
{
	if (!IsValid(EnemyCharacter))
		return;

	bIsFlying = true;

	const FVector CurrentLocation = EnemyCharacter->GetActorLocation();
	if (constexpr float LocationsEqualsTolerance = 20.0f;
		CurrentLocation.Equals(FlyingTargetLocation, LocationsEqualsTolerance))
	{
		bReachTheTarget = true;
	}
	else
	{
		bReachTheTarget = false;
		const FVector Direction = UKismetMathLibrary::
			GetDirectionUnitVector(CurrentLocation, FlyingTargetLocation) *
			DeltaSeconds;
		EnemyCharacter->AddMovementInput(Direction);
	}
}

bool ACPP_EnemyAIController::FindWalkingLocation()
{
	if (!IsValid(EnemyCharacter))
		return false;

	const FVector Origin = EnemyCharacter->StartTransform.GetLocation();
	WalkingTargetLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin,
	                                                                     FVector(WalkingSearchRadius,
		                                                                     WalkingSearchRadius, 0.001f));
	WalkingTargetsCounter++;
	bWalkingTargetLocationIsSet = true;
	EnemyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	return true;
}

void ACPP_EnemyAIController::WalkToLocation(const float DeltaSeconds)
{
	if (!IsValid(EnemyCharacter))
		return;

	bIsWalkingToLocation = true;

	const FVector CurrentLocation = EnemyCharacter->GetActorLocation();
	if (constexpr float LocationsEqualsTolerance = 12.0f;
		CurrentLocation.Equals(WalkingTargetLocation, LocationsEqualsTolerance))
	{
		bReachTheTarget = true;
	}
	else
	{
		bReachTheTarget = false;
		FVector Direction = UKismetMathLibrary::
			GetDirectionUnitVector(CurrentLocation, WalkingTargetLocation) *
			DeltaSeconds;
		Direction.Z = 0;
		EnemyCharacter->AddMovementInput(Direction);
	}
}

void ACPP_EnemyAIController::Server_EnemyIsDead_Implementation()
{
	Multicast_EnemyIsDead();
}

void ACPP_EnemyAIController::Multicast_EnemyIsDead_Implementation()
{
}

void ACPP_EnemyAIController::PlayerSteppedOnThePlatform(ACPP_Character* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
		return;

	if (!CharactersOnPlatform.Contains(PlayerCharacter))
	{
		CharactersOnPlatform.Emplace(PlayerCharacter);
		CharacterToChase = PlayerCharacter;
		if (IsValid(EnemyCharacter))
		{
			EnemyCharacter->SetEnemyState(EEnemyState::ChasingCharacter);
		}
	}
}

void ACPP_EnemyAIController::PlayerLeftThePlatform(ACPP_Character* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
		return;

	if (CharactersOnPlatform.Contains(PlayerCharacter))
	{
		CharactersOnPlatform.Remove(PlayerCharacter);
		CharacterToChase = CharactersOnPlatform.Num() > 0 ? CharactersOnPlatform.Last() : nullptr;
		if (CharacterToChase == nullptr && IsValid(EnemyCharacter))
		{
			if (bIsFlyingChasing)
			{
				EnemyCharacter->SetEnemyState(EEnemyState::Flying);
			}
			else
			{
				EnemyCharacter->SetEnemyState(EEnemyState::Walking);
			}
		}
	}
}

bool ACPP_EnemyAIController::FindChasingTargetLocation(const float DeltaSeconds)
{
	if (!IsValid(CharacterToChase) || CharacterToChase == nullptr)
		return false;

	if (CharactersOnPlatform.Num() > 1)
	{
		ACPP_Character* MostCloseCharacter = CharacterToChase;
		float MinDistance = UE::Geometry::Distance(EnemyCharacter->GetActorLocation(),
		                                           MostCloseCharacter->GetActorLocation());
		for (int32 i = 0; i < CharactersOnPlatform.Num(); i++)
		{
			if (const float Distance = UE::Geometry::Distance(EnemyCharacter->GetActorLocation(),
			                                                  CharactersOnPlatform[i]->GetActorLocation());
				Distance < MinDistance)
			{
				MinDistance = Distance;
				MostCloseCharacter = CharactersOnPlatform[i];
			}
		}
		CharacterToChase = MostCloseCharacter;
	}
	ChasingTargetLocation = CharacterToChase->GetActorLocation();
	ChaseCharacter(DeltaSeconds);
	return true;
}

void ACPP_EnemyAIController::ChaseCharacter(const float DeltaSeconds)
{
	const FVector CurrentLocation = EnemyCharacter->GetActorLocation();
	if (constexpr float LocationsEqualsTolerance = 25.0f;
		CurrentLocation.Equals(ChasingTargetLocation, LocationsEqualsTolerance))
	{
		bChasingTargetReached = true;
	}
	else
	{
		bChasingTargetReached = false;
		FVector Direction = UKismetMathLibrary::
			GetDirectionUnitVector(CurrentLocation, ChasingTargetLocation) *
			DeltaSeconds;
		if (!bIsFlyingChasing)
		{
			Direction.Z = 0;
		}
		EnemyCharacter->AddMovementInput(Direction);
	}
}
