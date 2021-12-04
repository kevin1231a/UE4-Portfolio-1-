// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainPlayer.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletal"));
	weaponEquipSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EquipSphere"));
	weaponComboParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponIdleParticle"));
	attackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponAttackBox"));
	idleWeaponParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponStayParticle"));

	RootComponent = weaponSkeletal;
	weaponEquipSphere->SetupAttachment(GetRootComponent());
	weaponComboParticle->SetupAttachment(GetRootComponent());
	attackBox->SetupAttachment(GetRootComponent());
	idleWeaponParticle->SetupAttachment(GetRootComponent());

	weaponEquipSphere->InitSphereRadius(85.f);

	weaponEquipSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	randomPlayNum = 0;
	launchMonsterCount = 0;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	weaponEquipSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlapEquipSphere);
	weaponEquipSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::EndOverlapEquipSphere);
	attackBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlapAttackBox);

	weaponComboParticle->Deactivate();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::DeActivateFirstNiagara()
{
	firstWeaponNiagara->Deactivate();
}

void AWeapon::BeginOverlapEquipSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			mainPlayerToCameraShake = mainPlayer;

			mainPlayer->SetOverlapWeapon(this);
		}
	}
}

void AWeapon::EndOverlapEquipSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		mainPlayer = Cast<AMainPlayer>(OtherActor);

		if (mainPlayer)
		{
			mainPlayer->SetOverlapWeapon(nullptr);
		}
	}
}

void AWeapon::BeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		attackMonster = Cast<AMonster>(OtherActor);

		if (attackMonster)
		{
			skillLastHitMonster = attackMonster;
			
			if (mainPlayer->bIsUsingFirstSkill)
			{
				UGameplayStatics::PlaySound2D(this, electricSound);

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, firstWeaponHitNiagara, attackMonster->GetActorLocation(), FRotator(0.f));

				skillLastHitMonster = attackMonster;
			}
			else if (mainPlayer->bIsUsingFirstCSkill)
			{
				attackMonster->LaunchCharacter(FVector(0.f, 0.f, 700.f), true, true);
			}

			if (mainPlayer->bIsUsingThirdSkill && launchMonsterCount == 0)
			{
				skillLastHitMonster = attackMonster;

				attackMonster->LaunchCharacter(FVector(0.f, 0.f, 500.f), true, true);

				launchMonsterCount++;

				attackMonster->monsterMovementComponent->GravityScale = 0.7;
			}
			else if (mainPlayer->bIsUsingThirdSkill && launchMonsterCount > 0)
			{
				attackMonster->LaunchCharacter(FVector(0.f, 0.f, 170.f), true, true);
				mainPlayer->LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			}

			randomPlayNum = FMath::RandRange(0, 2);

			UGameplayStatics::PlaySound2D(this, hitSoundArray[randomPlayNum]);

			mainPlayerToCameraShake->AttackCameraShake();

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), weaponHitParticle, GetActorLocation(), FRotator(0.f), true);

			attackMonster->MonsterHitByPlayer(weaponDamage);
		}
	}
}

void AWeapon::HitFunction()
{
	randomPlayNum = FMath::RandRange(0, 2);

	UGameplayStatics::PlaySound2D(this, hitSoundArray[randomPlayNum]);

	mainPlayerToCameraShake->AttackCameraShake();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), weaponHitParticle, skillLastHitMonster->GetActorLocation(), FRotator(0.f), true);
}

void AWeapon::CollisionOffEquipSphere()
{
	weaponEquipSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::CollisionOffAttackBox()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::CollisionOnAttackBox()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}