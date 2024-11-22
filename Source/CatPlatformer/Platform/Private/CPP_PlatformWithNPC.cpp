// (c) M. A. Shalaeva, 2024

#include "../Classes/CPP_PlatformWithNPC.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#ifndef CPP_ENEMYAICONTROLLER_H
#define CPP_ENEMYAICONTROLLER_H
#include "CatPlatformer/AI/Enemy/Classes/CPP_EnemyAIController.h"
#endif
class ACPP_EnemyAIController;
class ACPP_Character;

ACPP_PlatformWithNPC::ACPP_PlatformWithNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	Target_For_NPC_Spawning = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Target Point To Spawn NPC")));
	Target_For_NPC_Spawning->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Collision Box")));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->InitBoxExtent(FVector(150.0f, 150.0f, 3.0f));
	CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 12.0f));

	PlatformBase = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM Platform Base")));
	PlatformBase->SetRelativeLocation(FVector(0.0f, 0.0f, -12.0f));
	PlatformBase->SetupAttachment(RootComponent);
}

void ACPP_PlatformWithNPC::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_PlatformWithNPC::CollisionBoxOverlapBegin);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACPP_PlatformWithNPC::CollisionBoxOverlapEnd);

		GetWorld()->GetTimerManager().SetTimer(
			TH_SpawnCrow,
			this,
			&ACPP_PlatformWithNPC::SpawnCrow,
			1.5f,
			false);
	}
}

void ACPP_PlatformWithNPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.RemoveDynamic(this, &ACPP_PlatformWithNPC::CollisionBoxOverlapBegin);
		CollisionBox->OnComponentEndOverlap.RemoveDynamic(this, &ACPP_PlatformWithNPC::CollisionBoxOverlapEnd);

		if (GetWorld()->GetTimerManager().TimerExists(TH_SpawnCrow))
		{
			GetWorld()->GetTimerManager().ClearTimer(TH_SpawnCrow);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ACPP_PlatformWithNPC::CollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACPP_Character* CharacterRef = Cast<ACPP_Character>(OtherActor))
	{
		if (IsValid(EnemyRef))
		{
			if (ACPP_EnemyAIController* Controller = Cast<ACPP_EnemyAIController>(EnemyRef->GetController()))
			{
				Controller->PlayerSteppedOnThePlatform(CharacterRef);
			}
		}
	}
}

void ACPP_PlatformWithNPC::CollisionBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACPP_Character* CharacterRef = Cast<ACPP_Character>(OtherActor))
	{
		if (IsValid(EnemyRef))
		{
			if (ACPP_EnemyAIController* Controller = Cast<ACPP_EnemyAIController>(EnemyRef->GetController()))
			{
				Controller->PlayerLeftThePlatform(CharacterRef);
			}
		}
	}
}

void ACPP_PlatformWithNPC::SpawnCrow()
{
	if (GetWorld()->GetTimerManager().TimerExists(TH_SpawnCrow))
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_SpawnCrow);
	}
	if (IsValid(NPC_Class) && IsValid(Target_For_NPC_Spawning))
	{
		const FTransform Transform = Target_For_NPC_Spawning->GetComponentTransform();

		EnemyRef = GetWorld()->SpawnActorDeferred<ACPP_EnemyCharacter>(
			NPC_Class,
			FTransform::Identity,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

		EnemyRef->StartTransform = Transform;

		UGameplayStatics::FinishSpawningActor(EnemyRef, Transform);
	}
}
