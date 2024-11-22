// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_RespawnPoint.h"
#include "Kismet/GameplayStatics.h"
class ACPP_Character;

#ifndef CPP_ENEMYCHARACTER_H
#define CPP_ENEMYCHARACTER_H
#include "CatPlatformer/AI/Enemy/Classes/CPP_EnemyCharacter.h"
#endif
class ACPP_EnemyCharacter;

ACPP_RespawnPoint::ACPP_RespawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	GameStateRef = nullptr;
	GameModeRef = nullptr;

	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Collision")));
	CollisionBox->SetupAttachment(Root);
}

void ACPP_RespawnPoint::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<ACPP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_RespawnPoint::CollisionBoxOverlapBegin);
}

void ACPP_RespawnPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GameStateRef = nullptr;
	GameModeRef = nullptr;

	CollisionBox->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_RespawnPoint::CollisionBoxOverlapBegin);

	Super::EndPlay(EndPlayReason);
}

void ACPP_RespawnPoint::CollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;
	
	if (const ACPP_Character* Character = Cast<ACPP_Character>(OtherActor))
	{
		ACPP_PlayerState* PS = nullptr;
		if (IsValid(Character->GetPlayerStateRef()))
		{
			PS = Character->GetPlayerStateRef();
		}
		if (PS == nullptr || !IsValid(PS))
		{
			PS = Character->GetPlayerState<ACPP_PlayerState>();
		}
		if (IsValid(PS))
		{
			if (!GameStateRef.IsValid())
			{
				GameStateRef = Cast<ACPP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
			}
			if (GameStateRef.IsValid())
			{
				GameStateRef->AddUserScore(PS, -15);
			}
			PS->IncrementDeathsNumber();
		}
		if (!GameModeRef.IsValid())
		{
			GameModeRef = Cast<ACPP_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		}
		if (GameModeRef.IsValid())
		{
			GameModeRef->RespawnPlayer(Character->GetInstigatorController());
		}
	}
	else if (ACPP_EnemyCharacter* Enemy = Cast<ACPP_EnemyCharacter>(OtherActor))
	{
		Enemy->TeleportTo(Enemy->StartTransform.GetLocation(),
						  FRotator(Enemy->StartTransform.GetRotation()));
	}
}
