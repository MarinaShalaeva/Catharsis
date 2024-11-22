// (c) M. A. Shalaeva, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"

#ifndef CPP_PLAYERSTATE_H
#define CPP_PLAYERSTATE_H
#include "CatPlatformer/GameMode/Classes/CPP_PlayerState.h"
#endif
class ACPP_PlayerState;
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimInstance;
class UAnimMontage;
class UMaterialInstance;
class USoundBase;
class UNiagaraSystem;
class USlateBrushAsset;

#include "CPP_Character.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStateChanged, ACPP_PlayerState* /* New Player State */);

DECLARE_DELEGATE_ThreeParams(FCollectBuff,
                             const int32 /* Buff Type ID */,
                             USlateBrushAsset* /* Image */,
                             const float /* Effect Duration */);

DECLARE_DELEGATE_OneParam(FResetBuffEffect,
                          const int32 /* Buff Type ID */);

/** Enumeration for the current cat's sound. */
UENUM(Blueprintable)
enum class ECatSoundState : uint8
{
	None,
	BasicWalk,
	BasicRun,
	GrassWalk,
	GrassRun,
	Hiss,
	Attack,
	HitGround
};

/**
 * C++ parent class for the player character.
 */
UCLASS()
class CATPLATFORMER_API ACPP_Character : public ACharacter
{
	GENERATED_BODY()

	/** The constructor to set default variables. */
	ACPP_Character();

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

protected:
	/** Camera boom positioning the camera behind the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Camera",
		meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Camera",
		meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/**
	 * Niagara Component for correct working with particle
	 * system.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Niagara",
		meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;

	/**
	 * Collision sphere for checking intersections with
	 * objects during attacking.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Collision",
		meta = (AllowPrivateAccess = "true"))
	USphereComponent* LeftPawCollision;

	//==Personal variables that aren't available to other players==

protected:
	/** If character uses the sprint mode. */
	UPROPERTY(Replicated)
	bool bSprintNow;

public:
	/** Getter for the bSprintNow variable. */
	FORCEINLINE bool GetSprintNow() const { return bSprintNow; }

protected:
	/** The character's initial speed. */
	float BaseSpeed;

	/** The character's speed during his sprint period. */
	float SprintSpeed;

	/**
	 * Base turn rate, in deg/sec. Other scaling may affect
	 * final turn rate.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/**
	 * Base look up/down rate, in deg/sec.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/** Base jump's height. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump Variables")
	float BaseJumpZVelocity;

	/** Jump's height after applying the High Jump Buff. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump Variables")
	float HighJumpZVelocity;

	/** AnimInstance for applying animation montages. */
	TWeakObjectPtr<UAnimInstance> AnimInstance;

	/** Animation Montage with the attack animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat's State")
	TSoftObjectPtr<UAnimMontage> AttackMontage;

	/**
	 * Delegate for storing function that should be called
	 * after the Attack Animation Montage is ended.
	 */
	FOnMontageEnded AttackMontageEndedDelegate;

	/**
	 * Function that should be called after the Attack
	 * Animation Montage is ended.
	 * @param AnimMontage Animation montage that was ended.
	 * @param bInterrupted Was the animation interrupted?
	 */
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Animation Montage with the attack animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat's State")
	UAnimMontage* ReceiveDamageMontage;

	/**
	 * Delegate for storing function that should be called
	 * after the Receive Damage Animation Montage is ended.
	 */
	FOnMontageEnded ReceiveDamageMontageEndedDelegate;

	/**
	 * Function that should be called after the Receive
	 * Damage Animation Montage is ended.
	 * @param AnimMontage Animation montage that was ended.
	 * @param bInterrupted Was the animation interrupted?
	 */
	UFUNCTION()
	void OnReceiveDamageMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

public:
	/** Function for stopping active movements sounds. */
	UFUNCTION(BlueprintImplementableEvent)
	void StopAllMovementSounds();

	/**
	 * Function for starting playing the cat's sound.
	 * @param CatSound Sound state.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound(ECatSoundState CatSound);

	/**
	 * Flag indicating if the player is on the platform with
	 * grass. Is needed for correct sound spawning.
	 */
	bool bIsOnGrass;

	/** Setter for the bIsOnGrass variable. */
	FORCEINLINE void SetIsOnGrass(const bool bNewValue) { bIsOnGrass = bNewValue; }

	/** Getter for the bIsOnGrass variable. */
	FORCEINLINE bool GetIsOnGrass() const { return bIsOnGrass; }
	
	/** Is the character jumping (or falling) now? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cat's State")
	bool bIsJumping;

	/** Is the character attacking someone now? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cat's State")
	bool bIsAttacking;

	/** Is the character receiving damage now? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cat's State")
	bool bIsReceivingDamage;

	/** Is the character waiting for the player's input now? */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Cat's State")
	bool bIsWaiting;

	/**
	 * The list of available materials, that can be applied
	 * to the character's skeletal mesh.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat's State")
	TArray<UMaterialInstance*> CatColors;

	/**
	 * Material that is applied to the character's skeletal
	 * mesh at this moment.
	 */
	UPROPERTY(Replicated)
	UMaterialInstance* CurrentColor;

	//===================Buffs Variables===========================

	/**
	 * Delegate for notifying that some buff's effect was
	 * applied to the character.
	 */
	FCollectBuff CollectBuffDelegate;
	/**
	 * Delegate for notifying that some buff's effect was
	 * resetted.
	 */
	FResetBuffEffect ResetBuffEffectDelegate;

	/** Timer handle for resetting the double jump buff. */
	FTimerHandle TH_DoubleJump;
	/** Timer handle for resetting the high jump buff. */
	FTimerHandle TH_HighJump;
	/** Timer handle for resetting the decreasing speed buff. */
	FTimerHandle TH_SlowBuff;
	/** Timer handle for resetting the increasing speed buff. */
	FTimerHandle TH_FastBuff;
	/** Timer handle for resetting the shield buff. */
	FTimerHandle TH_ShieldBuff;

	/**
	 * Flag indicating if double jump buff is active at the
	 * moment.
	 */
	bool bDoubleJumpBuffIsActive;
	/**
	 * Flag indicating if high jump buff is active at the
	 * moment.
	 */
	bool bHighJumpBuffIsActive;
	/**
	 * Flag indicating if decreasing speed buff is active
	 * at the moment.
	 */
	bool bSlowBuffIsActive;
	/**
	 * Flag indicating if increasing speed buff is active
	 * at the moment.
	 */
	bool bFastBuffIsActive;
	/**
	 * Flag indicating if shield buff is active at the
	 * moment.
	 */
	bool bShieldBuffIsActive;

	/** Particle system for shield spawning. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ShieldNiagara;

	//======================Player State===========================
private:
	/** Reference to the associated player state instance. */
	UPROPERTY(Replicated)
	ACPP_PlayerState* PlayerStateRef;

public:
	/**
	 * Delegate for notifying about current player state
	 * changing.
	 */
	FPlayerStateChanged PlayerStateChangedDelegate;

private:
	/** Is called after the SetPlayerState event. */
	virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;

	//=====================Buffs Functions=========================
public:
	/** Function for resetting all active buffs' effects. */
	UFUNCTION()
	void ResetAllActiveBuffs();

	/**
	 * Function for applying the double jump buff's effect
	 * to the character.
	 */
	UFUNCTION()
	void LaunchDoubleJumpBuff(const float Duration, USlateBrushAsset* Image);
	/** Function for resetting the double jump buff's effect. */
	UFUNCTION()
	void ResetDoubleJumpBuff();

	/**
	 * Function for applying the high jump buff's effect to
	 * the character.
	 */
	UFUNCTION()
	void LaunchHighJumpBuff(const float Duration, USlateBrushAsset* Image);
	/** Function for resetting the high jump buff's effect. */
	UFUNCTION()
	void ResetHighJumpBuff();

	/**
	 * Function for applying the decreasing speed buff's
	 * effect to the character.
	 */
	UFUNCTION()
	void LaunchSlowBuff(const float Duration, USlateBrushAsset* Image);
	/**
	 * Function for resetting the decreasing speed buff's
	 * effect.
	 */
	UFUNCTION()
	void ResetSlowBuff();

	/**
	 * Function for applying the increasing speed buff's
	 * effect to the character.
	 */
	UFUNCTION()
	void LaunchFastBuff(const float Duration, USlateBrushAsset* Image);
	/**
	 * Function for resetting the increasing speed buff's
	 * effect.
	 */
	UFUNCTION()
	void ResetFastBuff();

	/**
	 * Function for applying the shield buff's effect to the
	 * character.
	 */
	UFUNCTION()
	void LaunchShieldBuff(const float Duration, USlateBrushAsset* Image);
	/** Function for resetting the shield buff's effect. */
	UFUNCTION()
	void ResetShieldBuff();

	//======================Buffs Effects==========================

	/** Function for spawning the shield. */
	UFUNCTION()
	void SpawnShield() const;

	/** Function for destroying the shield. */
	UFUNCTION()
	void DestroyShield() const;

	/**
	 * A function for changing the sliding properties of the
	 * character.
	 * @param bShouldSlide Should the character start (true)
	 * or stop (false) sliding?
	 */
	UFUNCTION()
	void ChangeCharactersSliding(const bool bShouldSlide) const;

	//=================Cat's color functions=======================

	/**
	 * Function for setting new cat's color material to the
	 * CurrentColor variable by ColorIndex.
	 * @param ColorIndex Index of the color to set.
	*/
	UFUNCTION(Server, Reliable)
	void SetCurrentColorVariableByIndex(const int32 ColorIndex);

private:
	void SetCurrentColorVariableByIndex_Implementation(const int32 ColorIndex);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCurrentColorVariableByIndex(const int32 ColorIndex);
	void Multicast_SetCurrentColorVariableByIndex_Implementation(const int32 ColorIndex);

public:
	/**
	 * Function to change current skeletal mesh material to
	 * the new one by ColorIndex.
	 * @param ColorIndex Index of the color to set.
	 */
	UFUNCTION()
	void SetCurrentColorMaterialByIndex(const int32 ColorIndex);

	/**
	 * Function to change current skeletal mesh material to
	 * the new one by the Material Instance.
	 * @param Material Material Instance to set.
	*/
	UFUNCTION(Server, Reliable)
	void ChangeCatsColor(UMaterialInstance* Material);
	void ChangeCatsColor_Implementation(UMaterialInstance* Material);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeCatsColor(UMaterialInstance* Material);
	void Multicast_ChangeCatsColor_Implementation(UMaterialInstance* Material);

	//================Control character functions==================

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate This is a normalized rate, i.e. 1.0 means
	 * 100% of desired turn rate.
	 */
	UFUNCTION()
	void TurnAtRate(const float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate This is a normalized rate, i.e. 1.0 means
	 * 100% of desired turn rate.
	 */
	UFUNCTION()
	void LookUpAtRate(const float Rate);

	/** Function for start the character's jumping. */
	UFUNCTION(Server, Reliable)
	void CustomStartJumping();
private:
	void CustomStartJumping_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CustomStartJumping();
	void Multicast_CustomStartJumping_Implementation();

public:
	/** Function for stop the character's jumping. */
	UFUNCTION(Server, Unreliable)
	void CustomStopJumping();
private:
	void CustomStopJumping_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CustomStopJumping();
	void Multicast_CustomStopJumping_Implementation();

public:
	/**
	 * Timer handle for checking if the player hit the ground
	 * after jumping.
	 */
	FTimerHandle TH_CheckForHittingTheGround;

	/**
	 * Function for checking if the player hit the ground
	 * after jumping.
	 */
	UFUNCTION()
	void CheckForHittingTheGround();

	/**
	 * Function to change the current speed depending on
	 * bIsSprint variable.
	 * Is called on the server.
	 * @param bIsSprint Should we change current speed
	 * to the sprint one?
	 */
	UFUNCTION(Server, Unreliable)
	void ChangeCurrentSpeed(bool bIsSprint);

private:
	void ChangeCurrentSpeed_Implementation(bool bIsSprint);

	/**
	 * Function to change the current speed depending on
	 * bIsSprint variable.
	 * Is called both on the server and on all clients.
	 * @param bIsSprint Should we change current speed
	 * to the sprint one?
	 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ChangeCurrentSpeed(const bool bIsSprint);
	void Multicast_ChangeCurrentSpeed_Implementation(const bool bIsSprint);

public:
	/**
	 * Function to start waiting mode.
	 * Is called on the server.
	 */
	UFUNCTION(Server, Unreliable)
	void SetIsWaiting(bool bNewValue);

private:
	void SetIsWaiting_Implementation(const bool bNewValue);

public:
	/**
	 * Function to start the attack.
	 * Is called on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_Attack();

private:
	void Server_Attack_Implementation();

	/**
	 * Function to start the attack.
	 * Is called both on the server and on all clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Attack();
	void Multicast_Attack_Implementation();

	/** Function for receiving damage. */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	/**
	 * Timer handle for calling Turn Off Post Process
	 * Damage Material function.
	 */
	FTimerHandle TH_TurnOff_PP_DamageMaterial;

	/**
	 * Function for starting the turning off of the damage
	 * post process material.
	 */
	UFUNCTION()
	void TurnOffPostProcessDamageMaterial();

protected:
	/**
	 * Blueprint function for changing the post process
	 * damage material's state.
	 * @param bTurnOn Should the post process material be
	 * applied or reset?
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeEnablingOfPostProcessDamageMaterial(const bool bTurnOn);

private:
	/**
	 * Timer handle for calling Multicast_CallApplyingDamage
	 * function.
	 */
	FTimerHandle TH_CallApplyingDamage;

	/**
	 * Calling the applying damage function in the needed
	 * frame of the attack animation.
	 * Is called both on the server and on all clients.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CallApplyingDamage();
	void Multicast_CallApplyingDamage_Implementation();

public:
	/** Function for incrementing the number of killed NPCs. */
	UFUNCTION()
	void NPC_WasKilled() const;

	//===============FORCE IN LINE Functions=======================

	/** Returns CameraBoom subobject. */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject. */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/**
	 * Getter for the PlayerStateRef variable.
	 * @return Reference to the associated player state
	 * instance.
	 */
	UFUNCTION()
	FORCEINLINE ACPP_PlayerState* GetPlayerStateRef() const { return PlayerStateRef; }

	/**
	 * Setter for the PlayerStateRef variable.
	 * @param PS Reference to the Player State.
	 */
	UFUNCTION()
	FORCEINLINE void SetPlayerStateRef(ACPP_PlayerState* PS) { PlayerStateRef = PS; }
};
