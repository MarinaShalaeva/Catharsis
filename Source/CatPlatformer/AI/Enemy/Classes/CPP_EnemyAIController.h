// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"

#ifndef CPP_ENEMYCHARACTER_H
#define CPP_ENEMYCHARACTER_H
#include "CatPlatformer/AI/Enemy/Classes/CPP_EnemyCharacter.h"
#endif
class ACPP_EnemyCharacter;

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

#include "CPP_EnemyAIController.generated.h"

/**
 * C++ parent class for the enemy AI controller. 
 */
UCLASS()
class CATPLATFORMER_API ACPP_EnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_EnemyAIController();

	/**
	 * Function for storing logic that should be applied
	 * when the actor appears in the game world.
	 */
	virtual void BeginPlay() override;

	/**
	 * Function for storing logic that should be applied in
	 * the end of the actor's life (but before its destroying).
	 * @param EndPlayReason Specifies why an actor is being
	 * deleted/removed from a level.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** 
	 * Function that is called every frame.
	 * Is needed for updating NPC's state.
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Overriden function for cases when this controller is
	 * asked to possess a pawn.
	 * @param InPawn The Pawn to be possessed.
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * Overriden function for cases when this controller is
	 * asked to unpossess current pawn.
	 */
	virtual void OnUnPossess() override;

	/**
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/**
	 * Reference to the associated instance of Enemy NPC's
	 * character.
	 */
	UPROPERTY()
	ACPP_EnemyCharacter* EnemyCharacter;

private:
	/** Timer handle for updating the character's behavior. */
	FTimerHandle TH_UpdateCharacterBehavior;

	/**
	 * Function for updating the character's behavior.
	 * Is called every frame.
	 */
	UFUNCTION(Server, Reliable)
	void UpdateCharacterBehavior(const float DeltaSeconds);
	void UpdateCharacterBehavior_Implementation(const float DeltaSeconds);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateCharacterBehavior(const float DeltaSeconds);
	void Multicast_UpdateCharacterBehavior_Implementation(const float DeltaSeconds);

public:
	/** Does character reach current target? */
	bool bReachTheTarget;

private:
	/**
	 * Timer handle for switching between flying and walking
	 * on the ground modes.
	 */
	FTimerHandle TH_WalkingAndFlyingSwitcher;

	/**
	 * Function for switching between flying and walking on
	 * the ground modes.
	 * Is called on server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_SwitchBetweenWalkingAndFlyingModes();
	void Server_SwitchBetweenWalkingAndFlyingModes_Implementation();

	/**
	 * Function for switching between flying and walking on
	 * the ground modes.
	 * Is called both on server and on clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SwitchBetweenWalkingAndFlyingModes();
	void Multicast_SwitchBetweenWalkingAndFlyingModes_Implementation();

public:
	//===============================================
	//                   Flying
	//===============================================

	/** Is character flying at the moment? */
	UPROPERTY(Replicated)
	bool bIsFlying;

	/**
	 * Flag indicating whether the target point for the
	 * flight was set.
	 */
	UPROPERTY(Replicated)
	bool bFlyingTargetLocationIsSet;

	/** The target point to which the enemy should fly up. */
	UPROPERTY(Replicated)
	FVector FlyingTargetLocation;

	/**
	 * Number of points that must be reached during flying
	 * before switching the current mode to walking one.
	 */
	UPROPERTY(Replicated)
	int32 MaxFlyingTargets;
	/**
	 * Current number of points that were reached during
	 * current flight.
	 */
	UPROPERTY(Replicated)
	int32 FlyingTargetsCounter;

private:
	/**
	 * Function for finding random flying location.
	 * @return Was the location found successfully?
	 */
	bool FindFlyingLocation();
	/**
	 * Function for performing the flight to chosen location. 
	 * @param DeltaSeconds Delta seconds from event Tick().
	 */
	void FlyToLocation(const float DeltaSeconds);

	//===============================================
	//                   Walking
	//===============================================

	/**
	 * Radius in which point to reach by walking should be
	 * found.
	 */
	float WalkingSearchRadius;

	/**
	 * Flag indicating if enemy is walking to chosen location
	 * at the moment.
	 */
	UPROPERTY(Replicated)
	bool bIsWalkingToLocation;

	/**
	 * Flag indicating whether the target point for the walk
	 * was set.
	 */
	UPROPERTY(Replicated)
	bool bWalkingTargetLocationIsSet;

	/** The target point to which the enemy should go. */
	UPROPERTY(Replicated)
	FVector WalkingTargetLocation;

	/**
	 * Number of points that must be reached during walking
	 * before switching the current mode to flying one.
	 */
	UPROPERTY(Replicated)
	int32 MaxWalkingTargets;
	/**
	 * Current number of points that were reached during
	 * current walk.
	 */
	UPROPERTY(Replicated)
	int32 WalkingTargetsCounter;

	/**
	 * Function for finding random walking location.
	 * @return Was the location found successfully?
	 */
	UFUNCTION()
	bool FindWalkingLocation();
	/**
	 * Function for performing the walk to chosen location. 
	 * @param DeltaSeconds Delta seconds from event Tick().
	 */
	UFUNCTION()
	void WalkToLocation(const float DeltaSeconds);

	/**
	 * Function for changing the blackboard key bIsDead.
	 * Is called on server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_EnemyIsDead();
	void Server_EnemyIsDead_Implementation();

	/**
	 * Function for changing the blackboard key bIsDead.
	 * Is called both on server and on clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EnemyIsDead();
	void Multicast_EnemyIsDead_Implementation();

	/**
	 * An array of players that are on the platform at the
	 * moment.
	 */
	UPROPERTY()
	TArray<ACPP_Character*> CharactersOnPlatform;

	/**
	 * The player character that is chased by this enemy at
	 * the moment.
	 */
	UPROPERTY(Replicated)
	ACPP_Character* CharacterToChase;

	/**
	 * The point where the chased player character is located
	 * at the moment.
	 */
	UPROPERTY(Replicated)
	FVector ChasingTargetLocation;

	/**
	 * Flag indicating if chasing is performing during the
	 * flying mode or during the walking one.
	 */
	UPROPERTY(Replicated)
	bool bIsFlyingChasing;

	/**
	 * Flag indicating if the enemy has reached the player
	 * for performing the attack.
	 */
	bool bChasingTargetReached;

public:
	/**
	 * Function for replying on appearance of the player
	 * character on the platform related to this enemy.
	 * @param PlayerCharacter Pointer to player character
	 * who stepped on the platform with this enemy.
	 */
	UFUNCTION()
	void PlayerSteppedOnThePlatform(ACPP_Character* PlayerCharacter);

	/**
	 * Function for replying on the player character's
	 * leaving from the platform related to this enemy.
	 * @param PlayerCharacter Pointer to player character
	 * who left the platform with this enemy.
	 */
	UFUNCTION()
	void PlayerLeftThePlatform(ACPP_Character* PlayerCharacter);

	/**
	 * Function for finding the chased player character's
	 * location.
	 * @param DeltaSeconds Delta seconds from event Tick().
	 * @return Was the location found successfully?
	 */
	UFUNCTION()
	bool FindChasingTargetLocation(const float DeltaSeconds);
	/**
	 * Function for walking of flying to the chased player
	 * character. 
	 * @param DeltaSeconds Delta seconds from event Tick().
	 */
	void ChaseCharacter(const float DeltaSeconds);
};
