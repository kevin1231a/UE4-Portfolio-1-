// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondWeaponSkill.h"
#include "Components/SphereComponent.h"
#include "Monster.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Monster.h"

// Sets default values
ASecondWeaponSkill::ASecondWeaponSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	skillRange = CreateDefaultSubobject<USphereComponent>(TEXT("SkillRangeSphere"));
	RootComponent = skillRange;
	skillAuraParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AuraParticle"));
	skillAuraParticle->SetupAttachment(GetRootComponent());

	skillDamage = 50.f;
	checkTime = 0.f;

	bIsHitSecondSkill = false;
}

// Called when the game starts or when spawned
void ASecondWeaponSkill::BeginPlay()
{
	Super::BeginPlay();
	
	skillRange->OnComponentBeginOverlap.AddDynamic(this, &ASecondWeaponSkill::BeginOverlapSkillSphere);
	skillRange->OnComponentEndOverlap.AddDynamic(this, &ASecondWeaponSkill::EndOverlapSkillSphere);
}

// Called every frame
void ASecondWeaponSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHitSecondSkill)
	{
		HitSkill(DeltaTime);
	}
}

void ASecondWeaponSkill::BeginOverlapSkillSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		hitMonster = Cast<AMonster>(OtherActor);

		if (hitMonster)
		{
			hitMonsterArray.Add(hitMonster);

			bIsHitSecondSkill = true;
		}
	}
}

void ASecondWeaponSkill::EndOverlapSkillSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		hitMonster = Cast<AMonster>(OtherActor);

		if (hitMonster)
		{
			hitMonster->PlayHitBySkillLastAnim();

			bIsHitSecondSkill = false;

			for (int i = 0; i < hitMonsterArray.Num(); i++)
			{
				hitMonsterArray[i]->SecondSkillLastLaunch();

				if (hitMonsterArray[i]->monsterHp <= 0.f)
				{
					hitMonsterArray[i]->MonsterDie();
				}
			}
		}
	}
}

void ASecondWeaponSkill::HitSkill(float DeltaTime)
{
	if (hitMonsterArray[0] != nullptr)
	{
		checkTime += DeltaTime;

		if (checkTime > 0.12f)
		{			
			for (int i = 0; i < hitMonsterArray.Num(); i++)
			{
				hitMonsterArray[i]->HitAnimation();
				hitMonsterArray[i]->monsterHp -= skillDamage;
				hitMonsterArray[i]->SecondSkillLaunch();

				PlaySkillHitSound();
				
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), skillHitParticle, hitMonsterArray[i]->GetActorLocation(), FRotator(0.f), true);
			}

			checkTime = 0.f;
		}
	}
}

void ASecondWeaponSkill::PlaySkillHitSound()
{
	UGameplayStatics::PlaySound2D(this, skillSound);
}