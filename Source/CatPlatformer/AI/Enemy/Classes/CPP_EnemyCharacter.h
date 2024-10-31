// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#ifndef CPP_GAMESTATE_H
#define CPP_GAMESTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_GameState.h"
#endif
class ACPP_GameState;

#ifndef CPP_CHARACTER_H
#define CPP_CHARACTER_H
#include "CatPlatformer/GameMode/Classes/CPP_Character.h"
#endif
class ACPP_Character;

#include "CPP_EnemyCharacter.generated.h"

/** Enumeration for the current enemy's state. */
UENUM(Blueprintable)
enum class EEnemyState : uint8
{
	Walking,
	Flying,
	ChasingCharacter,
	Attacking,
	Dying
};

DECLARE_DELEGATE(FEnemyIsDead);

/**
 * Parent Class for the enemy character controlled by AI.
 */
UCLASS()
class CATPLATFORMER_API ACPP_EnemyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** The constructor to set default variables. */
	ACPP_EnemyCharacter();

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
	 * Returns the properties used for network replication.
	 * @param OutLifetimeProps Lifetime properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/**
	 * Function for changing current flying speed.
	 * @param bApplyBasicSpeed Should basic or fast speed be
	 * applied?
	 */
	void ChangeFlyingSpeed(const bool bApplyBasicSpeed);
	/**
	 * Function for changing current walking speed.
	 * @param bApplyBasicSpeed Should basic or fast speed be
	 * applied?
	 */
	void ChangeWalkingSpeed(const bool bApplyBasicSpeed);

private:
	/** Reference to the instance of ACPP_GameState class. */
	UPROPERTY()
	ACPP_GameState* GameStateRef;

	/** Current enemy's state. */
	UPROPERTY(Replicated)
	EEnemyState EnemyState;

protected:
	/** The start value for the enemy's flying speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy | Speed | Flying")
	float BasicFlyingSpeed;
	/** The enemy's flying speed while chasing the player. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy | Speed | Flying")
	float ChasingFlyingSpeed;
	/** The start value for the enemy's walking speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy | Speed | Walking")
	float BasicWalkingSpeed;
	/** The enemy's walking speed while chasing the player. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy | Speed | Walking")
	float ChasingWalkingSpeed;

public:
	/**
	 * That's how many points the player will get for killing
	 * this enemy.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy | Score")
	int32 PointsToAddToCharacter;

	/** Start NPC's transform (location, rotation and scale). */
	FTransform StartTransform;

	/** Delegate for notifying that NPC died. */
	FEnemyIsDead EnemyIsDeadDelegate;

	/** Is the enemy character dead? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | Enemy State")
	bool bIsDead;

	/** Is the enemy character attacking someone now? */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | Enemy State")
	bool bIsAttacking;

private:
	/** AnimInstance for applying animation montages. */
	UPROPERTY()
	UAnimInstance* AnimInstance;

protected:
	/**
	 * Animation Montage with the attack animation during
	 * flying mode.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Enemy State")
	UAnimMontage* FlyingAttackMontage;

private:
	/**
	 * Delegate for storing function that should be called
	 * after the attack animation montage is ended.
	 */
	FOnMontageEnded AttackMontageEndedDelegate;

	/**
	 * Function that should be called after the attack
	 * animation montage is ended.
	 * @param AnimMontage Animation montage that was ended.
	 * @param bInterrupted Was the animation interrupted?
	 */
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	/**
	 * Apply damage to the enemy character.
	 * @param DamageAmount How much damage to apply
	 * @param DamageEvent Data package that fully describes
	 * the damage received.
	 * @param EventInstigator The Controller responsible for
	 * the damage.
	 * @param DamageCauser The Actor that directly caused
	 * the damage (e.g. the projectile that exploded, the
	 * rock that landed on you).
	 * @return The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;

public:
	/**
	 * Function to start the attack.
	 * Is called on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_Attack(ACPP_Character* CharacterToAttack);

private:
	void Server_Attack_Implementation(ACPP_Character* CharacterToAttack);

	/**
	 * Function to start the attack.
	 * Is called both on the server and on all clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Attack(ACPP_Character* CharacterToAttack);
	void Multicast_Attack_Implementation(ACPP_Character* CharacterToAttack);

	/**
	 * Timer handle for calling Multicast_CallSelfDestroying
	 * function.
	 */
	FTimerHandle TH_CallSelfDestroying;

public:
	/**
	 * Function for destroying the NPC's Character and the
	 * AI Controller.
	 * Is called on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_CallSelfDestroying();

private:
	void Server_CallSelfDestroying_Implementation();

	/**
	 * Function for destroying the NPC's Character and the
	 * AI Controller.
	 * Is called both on the server and on all clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CallSelfDestroying();
	void Multicast_CallSelfDestroying_Implementation();

public:
	/**
	 * Setter for the bIsDead variable.
	 * @param bNewValue Value to set.
	 */
	UFUNCTION(Server, Reliable)
	void SetIsDead(const bool bNewValue);

private:
	void SetIsDead_Implementation(const bool bNewValue);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetIsDead(const bool bNewValue);
	void Multicast_SetIsDead_Implementation(const bool bNewValue);

public:
	/**
	 * Getter for the bIsDead variable.
	 * @return bIsDead variable.
	 */
	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const;

	/**
	 * Setter for the EnemyState variable.
	 * @param NewState Value to set.
	 */
	UFUNCTION(Server, Reliable)
	void SetEnemyState(const EEnemyState NewState);

private:
	void SetEnemyState_Implementation(const EEnemyState NewState);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetEnemyState(const EEnemyState NewState);
	void Multicast_SetEnemyState_Implementation(const EEnemyState NewState);

public:
	/**
	 * Getter for the EnemyState variable.
	 * @return EnemyState variable.
	 */
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; };
};
